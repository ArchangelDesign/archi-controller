/*
    ** Archi Controller - BGA Rework Station Controller **
    copyright (c) 2025 Black Horse Repairs LLC and contributors.
    All rights reserved.
    https://blackhorserepairs.com

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <Arduino.h>
#include <PID_v1.h>

#include "measurements.h"
#include "persistence.h"

double zone_a_input, zone_a_output, zone_a_setpoint;

PID zone_a_pid(&zone_a_input, &zone_a_output, &zone_a_setpoint, msm_p, msm_i, msm_d, DIRECT);

void pid_init()
{
    zone_a_pid.SetOutputLimits(0, 100);
    zone_a_pid.SetMode(AUTOMATIC);
    msm_p = ps_get_or_store_p(msm_p);
    msm_i = ps_get_or_store_i(msm_i);
    msm_d = ps_get_or_store_d(msm_d);
}

void pid_update_power()
{
    zone_a_input = msm_zone_a_current_temperature;
    zone_a_setpoint = msm_zone_a_set_point;
    zone_a_pid.SetTunings(msm_p, msm_i, msm_d);
    zone_a_pid.Compute();
    msm_zone_a_output_power = floor(zone_a_output);
}