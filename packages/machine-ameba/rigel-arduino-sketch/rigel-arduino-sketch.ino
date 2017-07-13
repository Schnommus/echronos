#include <rtos-rigel.h>

#ifdef __cplusplus
extern "C" {
#endif

void fatal(RtosErrorId error_id);

void task_a();
void task_b();
void task_c();

void __wrap_xPortSysTickHandler();
extern void __real_xPortSysTickHandler();

#ifdef __cplusplus
}
#endif


bool init_done = false;

// the setup function runs once when you press reset or power the board
void setup() {

    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.println("Starting eChronos RTOS...");
    init_done = true;
    rtos_start();
}

#define EXISTING_TICK_RATE_HZ 1000
#define RTOS_TICK_RATE_HZ     100

void __wrap_xPortSysTickHandler() {
    static uint32_t fast_ticks = 0;

    if(init_done && fast_ticks % (EXISTING_TICK_RATE_HZ/RTOS_TICK_RATE_HZ) == 0) {
        rtos_timer_tick();
    }
    __real_xPortSysTickHandler();

    ++fast_ticks;
}

void fatal(RtosErrorId error_id) {
    Serial.println("FATAL RTOS ERROR");
    for(;;) { }
}

// No longer used
void loop() {}

void task_a() {
    for(;;) {
        Serial.println("Task A");
        rtos_sleep(100);
    }
}

void task_b() {
    for(;;) {
        Serial.println("Task B");
        rtos_sleep(200);
    }
}

void task_c() {
    for(;;) {
        Serial.println("Task C");
        rtos_sleep(500);
    }
}

