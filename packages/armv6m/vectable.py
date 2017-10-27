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

from prj import SystemBuildError, Module
import ply.cpp


class EntryModule(Module):
    # SVCall and PendSV are specified as optional in the schema, but are implemented in the asm to default to 'reset'.
    # They are not available for config (attempts will raise an assembler error) if preemption support is enabled.
    xml_schema = """
<schema>
    <entry name="flash_addr" type="int" default="0" />
    <entry name="flash_load_addr" type="int" optional="true"/>
    <entry name="flash_size" type="int" />
    <entry name="code_addr" type="int" default="0" />
    <entry name="sram_addr" type="int" default="0x20000000" />
    <entry name="sram_size" type="int" />
    <entry name="stack_size" type="int" default="0x1000" />

    <entry name="preemption" type="bool" optional="true" default="false" />

    <entry name="nmi" type="c_ident" default="reset" />
    <entry name="hardfault" type="c_ident" default="reset" />
    <entry name="svcall" type="c_ident" optional="true" />
    <entry name="pendsv" type="c_ident" optional="true" />
    <entry name="systick" type="c_ident" default="reset" />
    <entry name="external_irqs" type="list" default="[]">
        <entry name="external_irq" type="dict">
          <entry name="number" type="int"/>
          <entry name="handler" type="c_ident" default="reset" />
        </entry>
    </entry>
</schema>"""

    files = [
        {'input': 'vectable.s', 'render': True, 'type': 'asm'},
        {'input': 'default.ld', 'render': True, 'type': 'linker_script', 'stage': 'post_prepare'},
    ]

    def configure(self, xml_config):
        config = {}
        config['external_irqs'] = []

        config.update(super().configure(xml_config))
        # Fill in external IRQ vector list
        xirqs = [{'handler': 'reset'}] * 240
        for xirq in config['external_irqs']:
            xirqs[xirq['number']] = xirq
        config['external_irqs'] = xirqs

        # flash_load_addr defaults to flash_addr if we don't set it
        if config['flash_load_addr'] is None:
            config['flash_load_addr'] = config['flash_addr']

        return config

module = EntryModule()  # pylint: disable=invalid-name
