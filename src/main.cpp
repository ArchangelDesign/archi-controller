#include <Arduino.h>

#include <arduino-timer.h>

#include "comm_rail.h"
#include "heater.h"
#include "timers.h"

Timer<4, millis> process_timers;

void setup() {
    comm_init();
    heater_init();
    process_timers.every(3000, timer_process_update);
}

void loop() {
    comm_rail_loop();
    process_timers.tick();
}