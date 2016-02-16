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
                    sections =  _parse_sectioned_file( os.path.join( dirpath, "docs.md" ), None, [] )
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
                callgraphs[variant] = check_output(["opt", "-analyze", "-std-link-opts", "-basiccg"],
                                                   input=llvm_out).decode()
            except:
                assert False, "Failed to create call graph. Make sure clang & opt are available."

    def find_callers_of(self, variant, target_regex):
        this_callgraph = callgraphs[variant]

        definitions = [x.split("\n") for x in this_callgraph.split("\n\n")][1:]

        all_definitions = {}

        for definition in definitions:
            matches = re.match(".*node.*'(.*)'", definition[0])
            if not matches:
                continue
            this_function = matches.group(1)
            print("Found top-level function:", this_function)
            calls = []
            for subdefinition in definition[1:]:
                matches = re.match(".*calls.*'(.*)'", subdefinition)
                if not matches:
                    print ("Non match!",  subdefinition )
                    continue
                calls.append(matches.group(1))
                print ( matches.group(1) )
            all_definitions[this_function] = calls

        all_context_switchers = []

        def descend(depth, callers, functions):

            if depth > 15:
                print("Exceeded maximum depth..." + "->".join(callers))
                return

            if 'rtos_yield' in functions:
                if not callers[0] in all_context_switchers:
                    print(
                        "Context switch call: " +
                        "->".join(callers) +
                        " (Depth: " +
                        str(depth) +
                        ")")
                    all_context_switchers.append(callers[0])
                return

            for function in functions:
                if function in all_definitions.keys():
                    descend(
                        depth + 1,
                        callers + [function],
                        all_definitions[function])

        for function in all_definitions.keys():
            descend(0, [function], all_definitions[function])

        print("== RTOS API functions that trigger context switches ==")
        print('\n'.join(
            [function for function in all_context_switchers if function[:4] == "rtos"]))

    def test_context_switch_consistency(self):
        self.generate_callgraphs()
        self.find_callers_of("rigel", "^rtos_internal_context_switch.*")
