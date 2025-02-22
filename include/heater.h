#pragma once

#include <Arduino.h>
#include <arduino-timer.h>
#include "measurements.h"
/*
ZONE A - Top heater
ZONE B - bottom heater in the middle
ZONE C - bottom heater on the sides
*/

Timer<1, millis> t;

void heater_init() {
    pinMode(ZONE_A_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

void heater_tick() {
    t.tick();
}

bool heater_turn_on() {
    digitalWrite(ZONE_A_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    return false;
}

bool heater_turn_off(void *) {
    digitalWrite(ZONE_A_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    return false;
}

bool timer_heater_control(void *)
{
    heater_turn_on();
    uint16_t turn_off_time = floor((msm_zone_a_output_power * HEATER_PERIOD) / 100);
    t.in(turn_off_time - 1, heater_turn_off);
    return true;
}