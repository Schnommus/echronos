/*| headers |*/

/*| object_like_macros |*/

/*| types |*/

/*| structures |*/

/*| extern_declarations |*/

/*| function_declarations |*/

/*| state |*/

/*| function_like_macros |*/
#define rtos_internal_api_begin() \
{{#memory_protection}}if(mpu_is_enabled()) { \
    mpu_disable(); \
}{{/memory_protection}}

#define rtos_internal_api_end() \
{{#memory_protection}}if(!mpu_is_enabled()) { \
    mpu_enable(); \
}{{/memory_protection}}

{{^memory_protection}}
#define rtos_internal_api_end_with(x) \
    rtos_internal_api_end(); \
    return x
{{/memory_protection}}
{{#memory_protection}}
/* Put the return value on our local stack before turning on the
 * MPU again so that we are guaranteed that we have permission to
 * read the value we are returning. This is needed if, for example,
 * a function returns an rtos-internal API call or variable directly. */
#define rtos_internal_api_end_with(x) \
    __typeof__ (x) return_value = x; \
    rtos_internal_api_end(); \
    return return_value
{{/memory_protection}}

/*| functions |*/

/*| public_functions |*/
