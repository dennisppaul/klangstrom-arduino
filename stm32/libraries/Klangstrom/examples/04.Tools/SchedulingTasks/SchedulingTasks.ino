#include "Arduino.h"
#include "TaskScheduler.h"

// @TODO merge with `ApplicationTemplate`

using namespace klangstrom;

TaskScheduler fTaskScheduler;

void setup() {
    Serial.begin(115200);
    Serial.println("---------------");
    Serial.println("SchedulingTasks");
    Serial.println("---------------");

    fTaskScheduler.schedule_priority_task(exec_priority_task);
    fTaskScheduler.schedule_task(exec_normal_task);
    fTaskScheduler.schedule_task(exec_normal_task);
    fTaskScheduler.schedule_task(exec_normal_task);
    fTaskScheduler.schedule_repeated_task(exec_repeated_task);
}

void exec_priority_task() {
    static uint8_t counter = 0;
    Serial.print("exec_priority_task: ");
    Serial.println(counter);
    counter++;
}

void exec_normal_task() {
    static uint8_t counter = 0;
    Serial.print("exec_normal_task: ");
    Serial.println(counter);
    counter++;
}

void exec_repeated_task() {
    static uint8_t counter = 0;
    Serial.print("exec_repeated_task: ");
    Serial.println(counter);
    counter++;
    if (counter > 5) {
        fTaskScheduler.remove_repeated_task(exec_repeated_task);
    }
}

void loop() {
    fTaskScheduler.process();
    delay(500);
}
