#ifndef ENCODER_H
#define ENCODER_H

#include <napi.h>
#include "seal/seal.h"

class Encoder : public Napi::ObjectWrap<Encoder> {
 public:
  static Napi::Object init(Napi::Env env, Napi::Object exports);
  Encoder(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::IntegerEncoder> getInternalInstance();
  
 private:
  static Napi::FunctionReference constructor;
  Napi::Value decode(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::IntegerEncoder> _encoder;
};

#endif
