#include "driver.h"
#include <stdlib.h>
#include "device.h"

driver_state_t g_driver_state = {
    .step_index = 0,
    .angle = 0.f,
};

error_t driver_set_power(driver_power_state_t power_state)
{
    return error_ok;
}

error_t driver_step(driver_direction_t direction)
{
    if (abs(direction) != 1)
    {
        error_t error = {
            .code = ERROR_DRIVER_BAD_STEP,
            .message = "Step direction must be equal to -1 or +1",
        };
        return error;
    }



    g_driver_state.step_index = (g_driver_state.step_index + 1) % k_driver_step_count;

    return error_ok;
}

error_t driver_autosense_angle(bool update_global, float *out_angle)
{
    float angle = 0.f;

    if (out_angle != nullptr)
    {
        *out_angle = angle;
    }

    if (update_global)
    {
        g_driver_state.angle = angle;
    }

    error_t error = {
        .code = ERROR_ROUTINE_NOT_IMPLEMENTED,
        .message = "Driver position autosense is not implemented.",
    };

    return error;
}

error_t driver_init()
{
    error_t error = {
        .code = ERROR_ROUTINE_NOT_IMPLEMENTED,
        .message = "Driver initialization is not implemented.",
    };
    return error;

    // TODO: implement
    g_device_state.driver_initialized = true;

    return error_ok;
}
