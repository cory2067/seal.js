#include "sealcontext.h"
#include "keygenerator.h"
#include "decryptor.h"
#include <iostream>

Napi::FunctionReference Decryptor::constructor;

Napi::Object Decryptor::init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Decryptor", {
	// no instance methods for now
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Decryptor", func);
  return exports;
}

Decryptor::Decryptor(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Decryptor>(info)  {
    Napi::Env env = info.Env();

    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected context and secret key").ThrowAsJavaScriptException();
		return;
    }

    // extract seal::SEALContext from parameter 1
    Napi::Object obj = info[0].As<Napi::Object>();
    auto context = Napi::ObjectWrap<SEALContext>::Unwrap(obj)->getInternalInstance();
    
    // extract seal::SecretKey from parameter 2
    obj = info[1].As<Napi::Object>();
    auto sk = Napi::ObjectWrap<SecretKey>::Unwrap(obj)->getInternalInstance();

    this->_decryptor = std::make_shared<seal::Decryptor>(context, *sk);
}

std::shared_ptr<seal::Decryptor> Decryptor::getInternalInstance() {
    return this->_decryptor;
}
