/*| headers |*/

/*| object_like_macros |*/

/*| types |*/

/*| structures |*/

/*| extern_declarations |*/
{{#memory_protection}}
extern void rtos_internal_elevate_privileges(void);
extern void rtos_internal_drop_privileges(void);
{{/memory_protection}}

/*| function_declarations |*/

/*| state |*/

/*| function_like_macros |*/
{{^memory_protection}}
#define rtos_internal_api_begin()
#define rtos_internal_api_end()
{{/memory_protection}}

{{#memory_protection}}
#define rtos_internal_api_begin() rtos_internal_elevate_privileges()
#define rtos_internal_api_end() rtos_internal_drop_privileges()
{{/memory_protection}}
/*| functions |*/

/*| public_functions |*/

/*| public_privileged_functions |*/
