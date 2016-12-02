/*| headers |*/
#include <stdint.h>
#include <stddef.h>

/*| object_like_macros |*/

/*| types |*/

/*| structures |*/

/*| extern_declarations |*/

/*| function_declarations |*/

/*| state |*/
{{#tasks}}
/* We need to align stack addresses on their size to create a valid MPU region */
{{#memory_protection}}
static uint32_t stack_{{idx}}[{{stack_size}}]
    __attribute__(( aligned({{stack_size}}*sizeof(uint32_t)) ))
    __attribute__(( section (".bss.stack.task_{{idx}}") ));
{{/memory_protection}}
{{^memory_protection}}
static uint32_t stack_{{idx}}[{{stack_size}}] __attribute__((aligned(8)));
{{/memory_protection}}
{{/tasks}}

/*| function_like_macros |*/

/*| functions |*/

/*| public_functions |*/
