/*| public_headers |*/
#include <stdint.h>

/*| public_type_definitions |*/

/*| public_structure_definitions |*/

/*| public_object_like_macros |*/
{{#interrupt_events}}
#define {{prefix_const}}SIGNAL_SET_INTERRUPT_{{name|u}} {{sig_set}}
{{/interrupt_events}}

/*| public_function_like_macros |*/

/*| public_extern_definitions |*/

/*| public_function_definitions |*/
void {{prefix_func}}start(void);
