#ifndef HMAC_SHA256_HPP_
#define HMAC_SHA256_HPP_

#include "gobo-core-config.h"

#include "utils/wrap_stdint.h"

#include <mbedtls/md.h>

namespace go {
namespace Crypto {

class HmacSha256
{
public:
    enum
    {
        kHashSize = 32, // SHA-256 hash size (bytes)
    };

    HmacSha256();

    ~HmacSha256();

    void Start(const uint8_t *aKey, uint16_t aKeyLength);

    void Update(const uint8_t *aBuf, uint16_t aBufLength);

    void Finish(uint8_t aHash[kHashSize]);

private:
    mbedtls_md_context_t mContext;
};

} // namespace Crypto
} // namespace go

#endif // HMAC_SHA256_HPP_
