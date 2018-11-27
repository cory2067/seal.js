#include "ciphertext.h"
#include "encryptor.h"
#include "plaintext.h"
#include <iostream>

Napi::FunctionReference Ciphertext::constructor;

Napi::Object Ciphertext::init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Ciphertext", {
	// no instance methods for now
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Ciphertext", func);
  return exports;
}

Ciphertext::Ciphertext(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Ciphertext>(info)  {
    Napi::Env env = info.Env();

    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected encryptor and plaintext").ThrowAsJavaScriptException();
		return;
    }

    // extract seal::Encryptor from parameter 1
    Napi::Object obj = info[0].As<Napi::Object>();
    auto encryptor = Napi::ObjectWrap<Encryptor>::Unwrap(obj)->getInternalInstance();
    
    // extract seal::Plaintext from parameter 2
    obj = info[1].As<Napi::Object>();
    auto plaintext = Napi::ObjectWrap<Plaintext>::Unwrap(obj)->getInternalInstance();
    
    this->_ciphertext = std::make_shared<seal::Ciphertext>();
    encryptor->encrypt(*plaintext, *this->_ciphertext);
}

std::shared_ptr<seal::Ciphertext> Ciphertext::getInternalInstance() {
    return this->_ciphertext;
}
