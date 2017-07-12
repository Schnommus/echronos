#include <rtos-gatria.h>

#ifdef __cplusplus
extern "C" {
#endif

    void task_a();
    void task_b();
    void task_c();

#ifdef __cplusplus
}
#endif

// the setup function runs once when you press reset or power the board
void setup() {

    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.println("Starting eChronos RTOS...");
    rtos_start();
}

// No longer used
void loop() {}

void task_a() {
    for(;;) {
        Serial.println("Task A");
        rtos_yield();
    }
}

void task_b() {
    for(;;) {
        Serial.println("Task B");
        rtos_yield();
    }
}

void task_c() {
    for(;;) {
        Serial.println("Task C");
        rtos_yield();
    }
}

