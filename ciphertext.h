#ifndef CIPHERTEXT_H
#define CIPHERTEXT_H

#include <napi.h>
#include "seal/seal.h"

class Ciphertext : public Napi::ObjectWrap<Ciphertext> {
 public:
  static Napi::Object init(Napi::Env env, Napi::Object exports);
  Ciphertext(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::Ciphertext> getInternalInstance();
  
 private:
  static Napi::FunctionReference constructor;
  Napi::Value save(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::Ciphertext> _ciphertext;
};

#endif
