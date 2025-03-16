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
#include <arduino-timer.h>
#include "measurements.h"
#include "persistence.h"

#ifdef USE_PID_V1
    #include "controller/pid.h"
#endif

/*
ZONE A - Top heater
ZONE B - bottom heater in the middle
ZONE C - bottom heater on the sides
*/

Timer<1, millis> t;

void heater_init() {
    pinMode(ZONE_A_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    #ifdef USE_PID_V1
        pid_init();
    #endif
}

void heater_tick() {
    t.tick();
    #ifdef USE_PID_V1
        pid_update_power();
    #endif
}

bool heater_zone_a_turn_on() {
    digitalWrite(ZONE_A_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    return false;
}

bool heater_zone_a_turn_off(void *) {
    digitalWrite(ZONE_A_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    return false;
}

bool timer_heater_control(void *)
{
    if (msm_zone_a_output_power == 0 || !msm_is_running) {
        heater_zone_a_turn_off(NULL);
        return true;
    }
    heater_zone_a_turn_on();
    uint16_t turn_off_time = floor((msm_zone_a_output_power * HEATER_PERIOD) / 100);
    t.in(turn_off_time - 1, heater_zone_a_turn_off);
    return true;
}