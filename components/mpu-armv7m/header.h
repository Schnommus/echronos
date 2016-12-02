/*| public_headers |*/
{{#memory_protection}}
#include <stdbool.h>
{{/memory_protection}}

/*| public_types |*/

/*| public_structures |*/

/*| public_object_like_macros |*/

/*| public_function_like_macros |*/

/*| public_state |*/

/*| public_function_declarations |*/
{{#memory_protection}}
bool {{prefix_func}}handle_memmanage(void);
{{/memory_protection}}
