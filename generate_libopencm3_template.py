#!/bin/python3

#
# eChronos Real-Time Operating System
# Copyright (c) 2017, Commonwealth Scientific and Industrial Research
# Organisation (CSIRO) ABN 41 687 119 230.
#
# All rights reserved. CSIRO is willing to grant you a licence to the eChronos
# real-time operating system under the terms of the CSIRO_BSD_MIT license. See
# the file "LICENSE_CSIRO_BSD_MIT.txt" for details.
#
# @TAG(CSIRO_BSD_MIT)
#

import os
import os.path
import subprocess

libopencm3_path = "packages/libopencm3/"
genlink_path = libopencm3_path + "scripts/genlink.awk"
ld_data_path = libopencm3_path + "ld/devices.data"
lib_path = libopencm3_path + "lib"

cortex_architectures = ['cortex-m0', 'cortex-m0plus', 'cortex-m3', 'cortex-m4', 'cortex-m7']
echronos_supported_architectures = ['cortex-m3', 'cortex-m4', 'cortex-m7']

def get_chip_details(chip, mode):
    awk_command = """awk -v PAT="{}" -v MODE="{}" -f {} {}""".format(chip, mode, genlink_path, ld_data_path)
    result = subprocess.run(awk_command,
            stdout=subprocess.PIPE, check=True, shell=True)
    return result.stdout.decode()

def memory_size_string_to_int(size):
    postfixes = {
            'K': 1024,
            'M': 1024*1024,
            }

    if size[-1] in '0123456789':
        return int(size)
    else:
        if size[-1] in postfixes.keys():
            return int(size[:-1]) * postfixes[size[-1]]
        else:
            print("Unknown postfix on memory size?!")
            return 0

def generate_prx_for_part(part):
    part_family    = get_chip_details(part, 'FAMILY')
    part_subfamily = get_chip_details(part, 'SUBFAMILY')
    part_cpu       = get_chip_details(part, 'CPU')
    part_fpu       = get_chip_details(part, 'FPU')
    part_cppflags  = get_chip_details(part, 'CPPFLAGS')
    part_defs      = get_chip_details(part, 'DEFS')

    if len(part_family) == 0:
        print("Part not in libopencm3 database")
        return

    arch_flags = ['-mcpu={}'.format(part_cpu)]

    if part_cpu not in echronos_supported_architectures:
        print("CPU architecture '{}' is not currently supported by the RTOS".format(part_cpu))
        return

    arch_flags += ['-mthumb']

    if part_fpu == "soft":
        arch_flags += ["-msoft_float"]
    elif part_fpu == "hard-fpv4-sp-d16":
        arch_flags += ["-mfloat-abi=hard", "-mfpu=fpv4-sp-d16"]
    elif part_fpu == "hard-fpv5-sp-d16":
        arch_flags += ["-mfloat-abi=hard", "-mfpu=fpv5-sp-d16"]
    else:
        print("Nonstandard FPU flag?")
        return

    part_family_lib_path = lib_path + "/libopencm3_{}.a".format(part_family)
    part_subfamily_lib_path = lib_path + "/libopencm3_{}.a".format(part_family)

    libraries = []

    if os.path.isfile(part_family_lib_path):
        libraries += [part_family_lib_path]
    elif os.path.isfile(part_subfamily_lib_path):
        libraries += [part_subfamily_lib_path]
    else:
        print(("Library variant (i.e '{}') does not exist for this device.\n"
               "If this is a clean installation, ensure libopencm3 has been built")
               .format(part_family_lib_path))
        return

    part_properties = dict([p[3:].split('=') for p in part_defs.split() if p[:3] == "-D_"])
    part_rom_size = hex(memory_size_string_to_int(part_properties['ROM']))
    part_ram_size = hex(memory_size_string_to_int(part_properties['RAM']))
    part_rom_off = part_properties['ROM_OFF']
    part_ram_off = part_properties['RAM_OFF']

    print((
        "Fetching part details for {}:\n"
        "Part family:    {}\n"
        "Part subfamily: {}\n"
        "Part CPU:       {}\n"
        "Part FPU:       {}\n"
        "Part ROM size:  {}\n"
        "Part ROM offs.: {}\n"
        "Part RAM size:  {}\n"
        "Part RAM offs.: {}\n"
        "Extra flags:    {}\n"
        "Arch flags:     {}\n"
        "Libraries:      {}\n"
        "Raw defines:    {}\n").format(
            part, part_family, part_subfamily, part_cpu, part_fpu,
            part_rom_size, part_rom_off, part_ram_size, part_ram_off,
            part_cppflags, arch_flags, libraries, part_defs))


generate_prx_for_part('stm32f407VGT6')
