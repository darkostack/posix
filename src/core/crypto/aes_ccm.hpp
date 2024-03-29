#ifndef AES_CCM_HPP_
#define AES_CCM_HPP_

#include "gobo-core-config.h"

#include "utils/wrap_stdint.h"

#include <gobo/error.h>

#include "crypto/aes_ecb.hpp"

namespace go {
namespace Crypto {

class AesCcm
{
public:
    goError SetKey(const uint8_t *aKey, uint16_t aKeyLength);

    goError Init(uint32_t    aHeaderLength,
                 uint32_t    aPlainTextLength,
                 uint8_t     aTagLength,
                 const void *aNonce,
                 uint8_t     aNonceLength);

    void Header(const void *aHeader, uint32_t aHeaderLength);

    void Payload(void *aPlainText, void *aCipherText, uint32_t aLength, bool aEncrypt);

    void Finalize(void *aTag, uint8_t *aTagLength);

private:
    enum
    {
        kTagLengthMin = 4,
    };

    AesEcb   mEcb;
    uint8_t  mBlock[AesEcb::kBlockSize];
    uint8_t  mCtr[AesEcb::kBlockSize];
    uint8_t  mCtrPad[AesEcb::kBlockSize];
    uint8_t  mNonceLength;
    uint32_t mHeaderLength;
    uint32_t mHeaderCur;
    uint32_t mPlainTextLength;
    uint32_t mPlainTextCur;
    uint16_t mBlockLength;
    uint16_t mCtrLength;
    uint8_t  mTagLength;
};

} // namespace Crypto
} // namespace go

#endif // AES_CCM_HPP_
