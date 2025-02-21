#pragma once

#include "comm_rail.h"
#include "measurements.h"


bool timer_process_update(void *)
{
    comm_send_pv_update(msm_current_temperature);
    return true;
}