#ifndef AES_ECB_HPP_
#define AES_ECB_HPP_

#include "gobo-core-config.h"

#include <mbedtls/aes.h>

namespace go {
namespace Crypto {

class AesEcb
{
public:
    enum
    {
        kBlockSize = 16, // AES-128 block size (bytes).
    };

    AesEcb();

    ~AesEcb();

    void SetKey(const uint8_t *aKey, uint16_t aKeyLength);

    void Encrypt(const uint8_t aInput[kBlockSize], uint8_t aOutput[kBlockSize]);

private:
    mbedtls_aes_context mContext;
};

} // namespace Crypto
} // namespace go

#endif // AES_ECB_HPP_
