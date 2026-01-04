#include "device.h"

device_state_t g_device_state = {
    .ble_initialized = false,
    .driver_initialized = false,

    .last_command_authorization = 0,
};