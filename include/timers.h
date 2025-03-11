#pragma once

#include "comm_rail.h"
#include "measurements.h"


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