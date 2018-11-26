#include <napi.h>
#include "sealcontext.h"
#include "keygenerator.h"
#include "seal/seal.h"

Napi::Object initAll(Napi::Env env, Napi::Object exports) {
    KeyGenerator::init(env, exports);
    PublicKey::init(env, exports);
    SecretKey::init(env, exports);
    return SEALContext::init(env, exports);
}

NODE_API_MODULE(addon, initAll)
