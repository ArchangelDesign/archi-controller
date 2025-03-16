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
    process_timers.every(330, timer_read_temperatures);
}

void loop() {
    comm_rail_loop();
    heater_tick();
    process_timers.tick();
}