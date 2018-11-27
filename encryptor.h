#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <napi.h>
#include "seal/seal.h"

class Encryptor : public Napi::ObjectWrap<Encryptor> {
 public:
  static Napi::Object init(Napi::Env env, Napi::Object exports);
  Encryptor(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::Encryptor> getInternalInstance();
  
 private:
  static Napi::FunctionReference constructor;
  std::shared_ptr<seal::Encryptor> _encryptor;
};

#endif
