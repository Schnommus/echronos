/*| headers |*/
#include "rtos-gatria.h"

/*| object_like_macros |*/

/*| types |*/

/*| structures |*/

/*| extern_declarations |*/

/*| function_declarations |*/

/*| state |*/

/*| function_like_macros |*/

/*| functions |*/

/*| public_functions |*/
void
{{prefix_func}}yield_to(const {{prefix_type}}TaskId to) {{prefix_const}}REENTRANT
{
    rtos_internal_api_begin();
    const {{prefix_type}}TaskId from = get_current_task();
    current_task = to;
    context_switch(get_task_context(from), get_task_context(to));
    rtos_internal_api_end();
}

void
{{prefix_func}}yield(void) {{prefix_const}}REENTRANT
{
    rtos_internal_api_begin();
    const {{prefix_type}}TaskId to = sched_get_next();
    {{prefix_func}}yield_to(to);
    rtos_internal_api_end();
}

void
{{prefix_func}}block(void) {{prefix_const}}REENTRANT
{
    rtos_internal_api_begin();
    sched_set_blocked(get_current_task());
    {{prefix_func}}yield();
    rtos_internal_api_end();
}

void
{{prefix_func}}unblock(const {{prefix_type}}TaskId task)
{
    rtos_internal_api_begin();
    sched_set_runnable(task);
    rtos_internal_api_end();
}

/*| public_privileged_functions |*/
void
{{prefix_func}}start(void)
{
    {{#tasks}}
    context_init(get_task_context({{idx}}), {{function}}, stack_{{idx}}, {{stack_size}});
    {{/tasks}}

    {{#mpu_enabled}}
    mpu_initialize();
    {{/mpu_enabled}}

    context_switch_first(get_task_context({{prefix_const}}TASK_ID_ZERO));
}
