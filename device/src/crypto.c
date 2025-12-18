#include "crypto.h"
#include "mbedtls/psa_util.h"

error_t crypto_init()
{
    psa_crypto_init();
}