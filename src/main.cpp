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
    process_timers.every(HEATER_PERIOD, timer_heater_control);
}

void loop() {
    comm_rail_loop();
    heater_tick();
    process_timers.tick();
}