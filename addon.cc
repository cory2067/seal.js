#include <napi.h>
#include "sealcontext.h"
#include "seal/seal.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    return SEALContext::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)
