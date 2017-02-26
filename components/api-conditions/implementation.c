/*| headers |*/

/*| object_like_macros |*/

/*| types |*/

/*| structures |*/

/*| extern_declarations |*/
{{#memory_protection}}
inline void rtos_internal_elevate_privileges(void) __attribute__((always_inline));
inline void rtos_internal_drop_privileges(void) __attribute__((always_inline));
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
void rtos_internal_elevate_privileges(void) {
    asm volatile ("svc #1");
}
void rtos_internal_drop_privileges(void) {
    asm volatile (
                  "mrs r0, control\n"
                  "orr r0, r0, #1\n"
                  "msr control, r0"
                 : : : "r0" );
}

/*| public_functions |*/

/*| public_privileged_functions |*/
