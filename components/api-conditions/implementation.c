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
#define rtos_internal_api_begin() \
if(mpu_is_enabled()) { \
    mpu_disable(); \
}

#define rtos_internal_api_end() \
if(!mpu_is_enabled()) { \
    mpu_enable(); \
}
{{/memory_protection}}
/*| functions |*/

/*| public_functions |*/
