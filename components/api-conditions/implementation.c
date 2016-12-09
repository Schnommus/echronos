/*| headers |*/

/*| object_like_macros |*/

/*| types |*/

/*| structures |*/

/*| extern_declarations |*/

/*| function_declarations |*/

/*| state |*/
uint32_t rtos_api_depths[{{tasks.length}}] = {0};

/*| function_like_macros |*/
{{^memory_protection}}
#define rtos_internal_api_begin()
#define rtos_internal_api_end()
{{/memory_protection}}

{{#memory_protection}}
/* This is only used when we are *just* about to exit an API call */
#define rtos_internal_api_exit() \
if(!mpu_is_enabled()) { \
    mpu_enable(); \
}

/* Keep track of how deep down the RTOS API we are so that
 * we can catch exits from the RTOS API.
 *
 * Essentially, if the MPU is enabled we know that we are
 * entering from userspace and are at the outermost API level.
 *
 * We keep track of the API depth for each task as we may
 * come out of a context switch multiple levels deep.
 *
 * If the MPU is enabled when we're at the end of an api call,
 * it can only mean we're here from a context switch - so we
 * disable it and then update our state.
 *
 * TODO: look into this further to make more efficient*/
#define rtos_internal_api_begin() \
if(mpu_is_enabled()) { \
    mpu_disable(); \
    rtos_api_depths[get_current_task()] = 0; \
} \
++rtos_api_depths[get_current_task()];

#define rtos_internal_api_end() \
if(mpu_is_enabled()) { \
    mpu_disable(); \
} \
--rtos_api_depths[get_current_task()]; \
if(rtos_api_depths[get_current_task()] == 0) rtos_internal_api_exit(); \
{{/memory_protection}}

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
