#include "sealcontext.h"
#include "keygenerator.h"
#include "encryptor.h"
#include <iostream>

Napi::FunctionReference Encryptor::constructor;

Napi::Object Encryptor::init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Encryptor", {
	// no instance methods for now
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Encryptor", func);
  return exports;
}

Encryptor::Encryptor(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Encryptor>(info)  {
    Napi::Env env = info.Env();

    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected context and public key").ThrowAsJavaScriptException();
		return;
    }

    // extract seal::SEALContext from parameter 1
    Napi::Object obj = info[0].As<Napi::Object>();
    auto context = Napi::ObjectWrap<SEALContext>::Unwrap(obj)->getInternalInstance();
    
    // extract seal::PublicKey from parameter 2
    obj = info[1].As<Napi::Object>();
    auto pk = Napi::ObjectWrap<PublicKey>::Unwrap(obj)->getInternalInstance();

    this->_encryptor = std::make_shared<seal::Encryptor>(context, *pk);
}

std::shared_ptr<seal::Encryptor> Encryptor::getInternalInstance() {
    return this->_encryptor;
}
