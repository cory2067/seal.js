#include "encoder.h"
#include "plaintext.h"
#include <iostream>

Napi::FunctionReference Encoder::constructor;

Napi::Object Encoder::init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Encoder", {
    InstanceMethod("decode", &Encoder::decode)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Encoder", func);
  return exports;
}

Encoder::Encoder(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Encoder>(info)  {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected plaintext modulus").ThrowAsJavaScriptException();
		return;
    }

    // only supports IntegerEncoder right now
    uint32_t plain_mod = info[0].As<Napi::Number>().Uint32Value();
    this->_encoder = std::make_shared<seal::IntegerEncoder>(plain_mod);
}

std::shared_ptr<seal::IntegerEncoder> Encoder::getInternalInstance() {
    return this->_encoder;
}

Napi::Value Encoder::decode(const Napi::CallbackInfo& info) {
    // extract seal::Plaintext from parameter 1
    Napi::Object obj = info[0].As<Napi::Object>();
    auto plaintext = Napi::ObjectWrap<Plaintext>::Unwrap(obj)->getInternalInstance();
    int out = this->_encoder->decode_int32(*plaintext);

    return Napi::Number::New(info.Env(), out);
}
