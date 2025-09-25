#include "Arduino.h"
#include "System.h"
#include "Console.h"
#include "TaskScheduler.h"

using namespace klangstrom;

TaskScheduler fTaskScheduler;

void exec_priority_task() {
    static uint8_t counter = 0;
    console_println("exec_priority_task: %i",counter);
    counter++;
}

void exec_normal_task() {
    static uint8_t counter = 0;
    console_println("exec_normal_task: %i",counter);
    counter++;
}

void exec_repeated_task() {
    static uint8_t counter = 0;
    console_println("exec_repeated_task: %i",counter);
    counter++;
    if (counter > 5) {
        fTaskScheduler.remove_repeated_task(exec_repeated_task);
    }
}

void setup() {
    fTaskScheduler.schedule_priority_task(exec_priority_task);
    fTaskScheduler.schedule_task(exec_normal_task);
    fTaskScheduler.schedule_task(exec_normal_task);
    fTaskScheduler.schedule_task(exec_normal_task);
    fTaskScheduler.schedule_repeated_task(exec_repeated_task);
}

void loop() {
    fTaskScheduler.process();
    delay(500);
}
