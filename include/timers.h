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

#include "comm_rail.h"
#include "measurements.h"
#include "thermocouple.h"


bool timer_process_update(void *)
{
    comm_send_pv_update(msm_zone_a_current_temperature);
    comm_send_sp_update(msm_zone_a_set_point);
    comm_send_op_update(msm_zone_a_output_power);
    comm_send_pid_p_update(msm_p);
    comm_send_pid_i_update(msm_i);
    comm_send_pid_d_update(msm_d);
    return true;
}

bool timer_read_temperatures(void *)
{
    msm_zone_a_current_temperature = tc_read_zone_a_temperature();
    return true;
}

bool timer_adjust_set_points(void *)
{
    if (abs(msm_zone_a_set_point - msm_zone_a_target_set_point)) {
        return true; // HB
    }
    if (msm_zone_a_set_point < msm_zone_a_target_set_point) {
        msm_zone_a_set_point += msm_zone_a_ramp;
    }
    if (msm_zone_a_set_point > msm_zone_a_target_set_point) {
        msm_zone_a_set_point -= msm_zone_a_ramp;
    }

    return true;
}