#ifndef PLAINTEXT_H
#define PLAINTEXT_H

#include <napi.h>
#include "seal/seal.h"

class Plaintext : public Napi::ObjectWrap<Plaintext> {
 public:
  static Napi::Object init(Napi::Env env, Napi::Object exports);
  Plaintext(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::Plaintext> getInternalInstance();
  
 private:
  static Napi::FunctionReference constructor;
  std::shared_ptr<seal::Plaintext> _plaintext;
};

#endif
