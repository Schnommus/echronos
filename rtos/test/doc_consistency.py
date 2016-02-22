#
# eChronos Real-Time Operating System
# Copyright (C) 2015  National ICT Australia Limited (NICTA), ABN 62 102 206 173.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, version 3, provided that these additional
# terms apply under section 7:
#
#   No right, title or interest in or to any trade mark, service mark, logo or
#   trade name of of National ICT Australia Limited, ABN 62 102 206 173
#   ("NICTA") or its licensors is granted. Modified versions of the Program
#   must be plainly marked as such, and must not be distributed using
#   "eChronos" as a trade mark or product name, or misrepresented as being the
#   original Program.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# @TAG(NICTA_AGPL)
#

import os
import sys
import re
import pystache
import x

from subprocess import check_output
from collections import namedtuple
from pylib.utils import get_executable_extension, BASE_DIR

documented_variants = []
callgraphs = {}


class testDocConsistency:

    @classmethod
    def setUpClass(cls):
        # Build all documentation
        #r = os.system(sys.executable + " ./x.py build docs")
        #assert r == 0

        packages_dir = os.path.join(BASE_DIR, "packages")

        # Find rtos variants that are documented
        for dirpath, subdirs, files in os.walk(packages_dir):
            if "docs.md" in files:
                variant = namedtuple('Variant', ['arch', 'rtos', 'name'])
                (variant.arch, variant.rtos) = tuple(os.path.relpath(dirpath, packages_dir).split('/'))
                variant.name = variant.arch + '-' + variant.rtos
                documented_variants.append(variant)

        print("Documented variants found:", [v.name for v in documented_variants])

        # Set up directories for creating prx stubs
        doctest_dir = os.path.join(packages_dir, "unittest/doc_consistency")
        prx_stub_dir = os.path.join(doctest_dir, "stubs")
        if not os.path.exists(prx_stub_dir):
            os.makedirs(prx_stub_dir)

        # Build rtos variants that are documented
        for variant in documented_variants:
            minimal_prx_stub = None
            with open(os.path.join(doctest_dir, variant.rtos + ".prx"), 'r') as prx_stub:
                minimal_prx_stub = prx_stub.read()
            # exception? No doc_consistency prx stub available for the variant!

            this_prx_stub = pystache.render(minimal_prx_stub,
                                            {'architecture': variant.arch})

            with open(os.path.join(prx_stub_dir, variant.name + ".prx"), 'w') as stub:
                stub.write(this_prx_stub)

            r = os.system(sys.executable +
                          " ./prj/app/prj.py build unittest.doc_consistency.stubs." + variant.name)
            assert r == 0

        cls.generate_callgraphs(cls.build_aliases())
        pass

    # Finds aliases for rtos functions used in macros
    @classmethod
    def build_aliases(cls):
        aliases = {}
        for variant in documented_variants:
            variant_header_dir = "out/unittest/doc_consistency/stubs/{}/{}.h"\
                                 .format(variant.name, variant.rtos)
            # Works for multi & single line macros also definitions without an alias
            api_alias = "#define (rtos_[a-z_]*)(?:.*(?:\\\n)?.*(rtos_[a-z_]*))?"
            aliases[variant] = {}
            with open(variant_header_dir, 'r') as header:
                for match in re.finditer(api_alias, header.read()):
                    if match.group(1) in aliases[variant].keys():
                        aliases[variant][match.group(1)].append(match.group(2))
                    else:
                        aliases[variant][match.group(1)] = [match.group(2)]

        return aliases

    # Fill up callgraphs dictionary
    @classmethod
    def generate_callgraphs(cls, aliases):
        for variant in documented_variants:
            # Pull out a call heirarchy from the clang dump, starting with existing aliases
            callgraphs[variant] = aliases[variant]

            variant_dir = "out/unittest/doc_consistency/stubs/{}/{}.c"\
                          .format(variant.name, variant.rtos)
            try:
                llvm_out = check_output(["clang", variant_dir, "-S", "-emit-llvm", "-o", "-",
                                         "-Ipackages/" + variant.arch])
                callgraph_raw = check_output(["opt", "-analyze", "-basiccg"],
                                             input=llvm_out).decode()
            except:
                assert False, "Failed to create call graph. Make sure clang & opt are available."

            # 2 linebreaks delineate a definition, 1 linebreak delineates subdefinitions
            definitions = [x.split("\n") for x in callgraph_raw.split("\n\n")][1:]

            for definition in definitions:
                matches = re.match(".*node.*'(.*)'", definition[0])
                if not matches:
                    continue
                this_function = matches.group(1)
                calls = []
                for subdefinition in definition[1:]:
                    matches = re.match(".*calls.*'(.*)'", subdefinition)
                    if not matches:
                        continue
                    calls.append(matches.group(1))
                callgraphs[variant][this_function] = calls

    # Searches for functions that can indirectly call a target function
    # (Given the callgraph has already been constructed)
    def find_callers_of(self, variant, target):

        all_targets = []

        def descend(depth, callers, functions):

            if depth > 15:
                print("Exceeded maximum depth..." + "->".join(callers))
                return

            if any([re.match(target, f) for f in filter(None, functions)]):
                if not callers[0] in all_targets:
                  # print(target + " call: " + "->".join(callers) +
                  #      " (Depth: " + str(depth) + ")")
                    all_targets.append(callers[0])
                return

            for function in functions:
                if function in self.all_functions(variant):
                    descend(depth + 1, callers + [function],
                            callgraphs[variant][function])

        for function in self.all_functions(variant):
            descend(0, [function], callgraphs[variant][function])

        return self.filter_api_functions(all_targets)

    def filter_api_functions(self, functions):
        return [function for function in functions
                if function.startswith("rtos") and not function.startswith("rtos_internal")]

    def all_functions(self, variant):
        return list(callgraphs[variant].keys())

    def test_context_switch_consistency(self):
        non_preemptive_documented_variants = \
            [variant for variant in documented_variants
                if "context-switch-preempt" not in
             [c.name for c in x.CORE_SKELETONS[variant.rtos.split('-')[1]]]]

        for variant in non_preemptive_documented_variants:
            context_switchers = self.find_callers_of(variant, "[a-z_]*context_switch")
            print("For {}, context switches are triggered by:".format(variant.name))
            print(context_switchers)

    def get_docs_for(self, variant):
        packages_dir = os.path.join(BASE_DIR, "packages")
        doc_path = os.path.join(packages_dir, variant.arch, variant.rtos, "docs.md")
        with open(doc_path, 'r') as doc:
            return doc.read()
        return None

    def test_all_api_functions_documented(self):
        for variant in documented_variants:
            implemented_functions = \
                self.filter_api_functions(self.all_functions(variant))

            print("Analyzing {} for documentation exhaustiveness...".format(variant.name))
            docs = self.get_docs_for(variant)

            # Get API functions that are documented
            # Criteria: correct header size, within api tags
            # and with a following codebox that represents a function
            documented_functions = []
            api_function_regex = '###.*"api">\s*([a-z_]*)\s*<.*\n\n.*"codebox">[a-zA-Z _]*\(.*\)'
            for match in re.finditer(api_function_regex, docs):
                documented_functions.append("rtos_" + match.group(1))

            undocumented_functions = set(implemented_functions) - set(documented_functions)
            if len(undocumented_functions) != 0:
                print("Undocumented API functions found: {} "
                      "whilst analyzing {} docs with respect to implementation."
                      .format(undocumented_functions, variant.name))

            irrelevant_functions = set(documented_functions) - set(implemented_functions)
            if len(irrelevant_functions) != 0:
                print("Irrelevant API documentation found: {} "
                      "whilst analyzing {} docs with respect to implementation."
                      .format(irrelevant_functions, variant.name))
