#pragma once

#include "comm_rail.h"
#include "measurements.h"


bool timer_process_update(void *)
{
    comm_send_pv_update(msm_current_temperature);
    comm_send_sp_update(msm_current_set_point);
    comm_send_op_update(msm_zone_a_output_power);
    return true;
}