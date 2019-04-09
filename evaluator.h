#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <napi.h>
#include "seal/seal.h"

class Evaluator : public Napi::ObjectWrap<Evaluator> {
 public:
  static Napi::Object init(Napi::Env env, Napi::Object exports);
  Evaluator(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::Evaluator> getInternalInstance();
  
 private:
  static Napi::FunctionReference constructor;
  std::shared_ptr<seal::Evaluator> _evaluator;

  Napi::Value addInPlace(const Napi::CallbackInfo& info);
  Napi::Value addMany(const Napi::CallbackInfo& info);
  
  Napi::Value multiplyInPlace(const Napi::CallbackInfo& info);
  Napi::Value multiplyMany(const Napi::CallbackInfo& info);
  
  Napi::Value negate(const Napi::CallbackInfo& info);
};

#endif
