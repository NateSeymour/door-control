#ifndef DOORCTL_CRYPTO_H
#define DOORCTL_CRYPTO_H

#include "pico/stdlib.h"
#include "error.h"

error_t crypto_verify_authorization_token(char const *buffer, size_t size);

error_t crypto_init();

#endif //DOORCTL_CRYPTO_H
