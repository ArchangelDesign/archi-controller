#pragma once

#include <Arduino.h>
#include <arduino-timer.h>

void heater_init() {
    pinMode(ZONE_A_PIN, OUTPUT);
}