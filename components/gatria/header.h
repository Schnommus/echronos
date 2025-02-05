/* Applications do not necessarily access all RTOS APIs.
 * Therefore, they are marked as potentially unused for static analysis. */
/*| public_headers |*/

/*| public_types |*/

/*| public_structures |*/

/*| public_object_like_macros |*/

/*| public_function_like_macros |*/

/*| public_state |*/

/*| public_function_declarations |*/
/*@unused@*/
void {{prefix_func}}yield_to({{prefix_type}}TaskId) {{prefix_const}}REENTRANT;
/*@unused@*/
void {{prefix_func}}yield(void) {{prefix_const}}REENTRANT;
/*@unused@*/
void {{prefix_func}}block(void) {{prefix_const}}REENTRANT;
/*@unused@*/
void {{prefix_func}}unblock({{prefix_type}}TaskId);
void {{prefix_func}}start(void);

