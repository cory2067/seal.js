#ifndef DECRYPTOR_H
#define DECRYPTOR_H

#include <napi.h>
#include "seal/seal.h"

class Decryptor : public Napi::ObjectWrap<Decryptor> {
 public:
  static Napi::Object init(Napi::Env env, Napi::Object exports);
  Decryptor(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::Decryptor> getInternalInstance();
  
 private:
  static Napi::FunctionReference constructor;
  std::shared_ptr<seal::Decryptor> _decryptor;
};

#endif
