#pragma once

#include <Arduino.h>
#include <PID_v1.h>

#include "measurements.h"

double zone_a_input, zone_a_output, zone_a_setpoint;

PID zone_a_pid(&zone_a_input, &zone_a_output, &zone_a_setpoint, msm_p, msm_i, msm_d, DIRECT);

void pid_init()
{
    zone_a_pid.SetOutputLimits(0, 100);
    zone_a_pid.SetMode(AUTOMATIC);
}

void pid_update_power()
{
    zone_a_input = msm_zone_a_current_temperature;
    zone_a_setpoint = msm_zone_a_set_point;
    zone_a_pid.Compute();
    msm_zone_a_output_power = floor(zone_a_output);
}