#pragma once

#include <Arduino.h>

int16_t msm_zone_a_current_temperature = 229;
int16_t msm_zone_a_set_point = 230;
uint8_t msm_zone_a_output_power = 10;

// PID loop config, can be updated via comm rail
double msm_p = 2.5, msm_i = 4.5, msm_d = 2;

bool msm_is_running = false;