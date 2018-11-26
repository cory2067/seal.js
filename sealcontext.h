#ifndef SEALCONTEXT_H
#define SEALCONTEXT_H

#include <napi.h>
#include "seal/seal.h"

class SEALContext : public Napi::ObjectWrap<SEALContext> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  SEALContext(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::SEALContext> GetInternalInstance();
  
 private:
  static Napi::FunctionReference constructor;
  std::shared_ptr<seal::SEALContext> _context;
};

#endif
