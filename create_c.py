#!/bin/python3

import re
import argparse

def print_regs(prefix, filename, width):
    with open(filename, 'r') as infile:
        text = infile.read()
        m = re.compile(r"^([0-9:]+)\n([A-Za-z][A-Za-z0-9]+)\n", re.MULTILINE)
        last_bit = None
        for match in m.findall(text):
            bitstring = match[0]
            reg_name = match[1]

            if len(reg_name) < 2:
                continue

            bit_hi = bit_lo = None
            if ':' in bitstring:
                bit_hi, bit_lo = map(int, tuple(bitstring.split(':')))
            else:
                bit_hi = bit_lo = int(bitstring)

            if last_bit == None or last_bit - 1 == bit_hi:
                if reg_name == 'Reserved':
                    if bit_hi == bit_lo:
                        print("/* Bit {} - Reserved */".format(bitstring))
                    else:
                        print("/* Bits {} - Reserved */".format(bitstring))
                else:
                    mask = define = None
                    if bit_hi == bit_lo:
                        mask = "(1 << {})".format(bitstring)
                    else:
                        ones = 1 + bit_hi - bit_lo
                        n = 2 ** ones - 1
                        mask = "({} << {})".format(hex(n), str(bit_lo))
                        reg_name += "_MASK"
                    define = "#define {}_{}".format(prefix, reg_name)
                    define = define.ljust(width)
                    print(define + mask)

                last_bit = bit_lo

parser = argparse.ArgumentParser()

parser.add_argument("prefix", help="the register name")
parser.add_argument("filename", help="file to read register values from")
parser.add_argument("--width", help="how wide to let declarations become, default 40", default=40)

args = parser.parse_args()

print_regs(args.prefix, args.filename, int(args.width))
