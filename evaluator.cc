#include "sealcontext.h"
#include "evaluator.h"
#include "ciphertext.h"
#include <iostream>

Napi::FunctionReference Evaluator::constructor;

Napi::Object Evaluator::init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Evaluator", {
    InstanceMethod("addInPlace", &Evaluator::addInPlace)
	// no instance methods for now
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Evaluator", func);
  return exports;
}

Evaluator::Evaluator(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Evaluator>(info)  {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected context as arg").ThrowAsJavaScriptException();
		return;
    }

    // extract seal::SEALContext from parameter 1
    Napi::Object obj = info[0].As<Napi::Object>();
    auto context = Napi::ObjectWrap<SEALContext>::Unwrap(obj)->getInternalInstance();
    
    this->_evaluator = std::make_shared<seal::Evaluator>(context);
}

std::shared_ptr<seal::Evaluator> Evaluator::getInternalInstance() {
    return this->_evaluator;
}

Napi::Value Evaluator::addInPlace(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected two ciphertexts").ThrowAsJavaScriptException();
		return env.Null();
    }
    
    Napi::Object obj = info[0].As<Napi::Object>();
    auto x = Napi::ObjectWrap<Ciphertext>::Unwrap(obj)->getInternalInstance();
    
    obj = info[1].As<Napi::Object>();
    auto y = Napi::ObjectWrap<Ciphertext>::Unwrap(obj)->getInternalInstance();

    this->_evaluator->add_inplace(*x, *y);

    return env.Null();
}
