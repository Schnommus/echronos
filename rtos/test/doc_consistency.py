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
from subprocess import check_output

from pylib.utils import get_executable_extension, BASE_DIR
from pylib.components import _parse_sectioned_file
import pystache
import x

documented_variants = []
callgraphs = {}

class testDocConsistency:
    @classmethod
    def setUpClass(cls):
        components_dir = os.path.join( BASE_DIR, "components" )

        # Find rtos variants that are documented
        for dirpath, subdirs, files in os.walk(components_dir):
            if "docs.md" in files:
                try:
                    sections = _parse_sectioned_file( os.path.join( dirpath, "docs.md" ), None, [] )
                except pystache.context.KeyNotFoundError:
                    # Catch rendering dependency errors, which we dont' care about
                    continue
                # If this component is an rtos, then it's documented.
                if "rtos" in sections["provides"]:
                    documented_variants.append( os.path.basename( dirpath ) )

        print( "Documented variants found:", documented_variants )

        # Build rtos variants that are documented
        for variant in documented_variants:
            r = os.system(sys.executable + " ./prj/app/prj.py build stub." + variant)
            assert r == 0

        # Build all documentation
        #r = os.system(sys.executable + " ./x.py build docs")
        #assert r == 0
        pass

    # Fill up callgraphs dictionary
    def generate_callgraphs(self):
        for variant in documented_variants:
            variant_dir = "out/stub/{}/rtos-{}.c".format( variant, variant)
            try:
                llvm_out = check_output(["clang", variant_dir, "-S", "-emit-llvm", "-o", "-"])
                callgraph_raw = check_output(["opt", "-analyze", "-basiccg"],
                                                   input=llvm_out).decode()
            except:
                assert False, "Failed to create call graph. Make sure clang & opt are available."

            # Pull out a call heirarchy from the clang dump
            callgraphs[variant] = {}
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

    def find_callers_of(self, variant, target):

        all_targets = []

        def descend(depth, callers, functions):

            if depth > 15:
                print("Exceeded maximum depth..." + "->".join(callers))
                return

            if target in functions:
                if not callers[0] in all_targets:
                    print(
                        target + " call: " +
                        "->".join(callers) +
                        " (Depth: " +
                        str(depth) +
                        ")")
                    all_targets.append(callers[0])
                return

            for function in functions:
                if function in self.all_functions(variant):
                    descend(
                        depth + 1,
                        callers + [function],
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
        self.generate_callgraphs()

        non_preemptive_documented_variants = \
            [variant for variant in documented_variants
                if "context-switch-preempt" not in
                     [c.name for c in x.CORE_SKELETONS[variant]]]

        for variant in non_preemptive_documented_variants:
            context_switchers = self.find_callers_of(variant, "context_switch")
            print("For {}, context switches are triggered by:".format(variant))
            print(context_switchers)

    def get_docs_for(self, variant):

        all_docs = {}
        packages_dir = os.path.join( BASE_DIR, "packages" )

        for dirpath, subdirs, files in os.walk(packages_dir):
            # Variant name in it's path seems to work reliably for now
            if "docs.md" in files and variant in dirpath:
                doc_path = os.path.join(dirpath, "docs.md")
                with open(doc_path, 'r') as doc:
                    all_docs[doc_path] = doc.read()

        return all_docs

    def test_all_api_functions_documented(self):
        self.generate_callgraphs()

        for variant in documented_variants:
            implemented_functions = \
                    self.filter_api_functions(self.all_functions(variant))

            all_docs = self.get_docs_for(variant)
            for docfile in all_docs.keys():
                print("Analyzing {}...".format(docfile))

                # Get API functions that are documented
                documented_functions = []
                for line in all_docs[docfile].split("\n"):
                    matches = re.match('###.*"api">\s*([a-z_]*)\s*<', line)
                    if matches:
                        # Noting documentation does not include rtos prefix
                        documented_functions.append("rtos_"+matches.group(1))

                undocumented_functions = set(implemented_functions) - set(documented_functions)
                if len( undocumented_functions ) != 0:
                    assert False, "Undocumented API functions found: {} " \
                                  "whilst analyzing {} with respect to the implementation of {}." \
                                  .format(undocumented_functions, docfile, variant)

                irrelevant_functions = set(documented_functions) - set(implemented_functions)
                if len( irrelevant_functions ) != 0:
                    assert False, "Irrelevant API documentation found: {} " \
                                  "whilst analyzing {} with respect to the implementation of {}." \
                                  .format(irrelevant_functions, docfile, variant)
