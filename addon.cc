#include <napi.h>
#include "sealcontext.h"
#include "keygenerator.h"
#include "encryptor.h"
#include "encoder.h"
#include "plaintext.h"
#include "ciphertext.h"
#include "evaluator.h"
#include "seal/seal.h"

Napi::Object initAll(Napi::Env env, Napi::Object exports) {
    KeyGenerator::init(env, exports);
    PublicKey::init(env, exports);
    SecretKey::init(env, exports);
    Encryptor::init(env, exports);
    Encoder::init(env, exports);
    Plaintext::init(env, exports);
    Ciphertext::init(env, exports);
    Evaluator::init(env, exports);
    return SEALContext::init(env, exports);
}

NODE_API_MODULE(addon, initAll)
