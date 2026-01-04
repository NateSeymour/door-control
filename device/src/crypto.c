#include "crypto.h"

#include <stdlib.h>
#include <string.h>
#include "mbedtls/ecdsa.h"
#include "device.h"
#include "generated/pka.h"

static mbedtls_ecdsa_context ctx;
static mbedtls_ecp_point q;
static mbedtls_ecp_group_id group_id = MBEDTLS_ECP_DP_CURVE448; // TODO: change to appropriate curve.

error_t crypto_verify_authorization_token(char const* buffer, size_t size)
{
    char authorization_token[DOORCTL_AUTHORIZATION_TOKEN_LENGTH];
    memcpy(authorization_token, buffer, size);

    char const *command = strtok(authorization_token, "@");
    char const *timestamp = strtok(nullptr, "@");
    char const *signature = strtok(nullptr, "@");

    if (strcmp(command, "authorized") != 0)
    {
        error_t error = {
            .code = ERROR_AUTH_TOKEN_INVALID_COMMAND,
            .message = "Invalid command in authorization token. Expected 'authorized'.",
        };
        return error;
    }

    size_t timestamp_ms = strtol(timestamp, nullptr, 10);
    if (timestamp_ms <= g_device_state.last_command_authorization)
    {
        error_t error = {
            .code = ERROR_AUTH_TOKEN_EXPIRED,
            .message = "Supplied authorization token is expired.",
        };
        return error;
    }

    return error_ok;
}

error_t crypto_init()
{
    mbedtls_ecdsa_init(&ctx);
    mbedtls_ecp_point_init(&q);

    return error_ok;
}