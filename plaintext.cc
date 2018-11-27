#include "plaintext.h"
#include "ciphertext.h"
#include "encoder.h"
#include "decryptor.h"
#include <iostream>

Napi::FunctionReference Plaintext::constructor;

Napi::Object Plaintext::init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Plaintext", {
	// no instance methods for now
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Plaintext", func);
  return exports;
}

Plaintext::Plaintext(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Plaintext>(info)  {
    Napi::Env env = info.Env();

    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected encoder/value or decryptor/ciphertext").ThrowAsJavaScriptException();
		return;
    }

    // this is kinda janky
    if (info[1].IsNumber()) {
        // extract seal::IntegerEncoder from parameter 1
        Napi::Object obj = info[0].As<Napi::Object>();
        auto encoder = Napi::ObjectWrap<Encoder>::Unwrap(obj)->getInternalInstance();
        
        // extract integer to encode from parameter 2
        int val = info[1].As<Napi::Number>().Int32Value();
        this->_plaintext = std::make_shared<seal::Plaintext>(encoder->encode(val));
    } else {
        // extract seal::Decryptor from parameter 1
        Napi::Object obj = info[0].As<Napi::Object>();
        auto decryptor = Napi::ObjectWrap<Decryptor>::Unwrap(obj)->getInternalInstance();
        
        // extract seal::Plaintext from parameter 1
        obj = info[0].As<Napi::Object>();
        auto ciphertext = Napi::ObjectWrap<Ciphertext>::Unwrap(obj)->getInternalInstance();

        this->_plaintext = std::make_shared<seal::Plaintext>();
        decryptor->decrypt(*ciphertext, *this->_plaintext);
    }
}

std::shared_ptr<seal::Plaintext> Plaintext::getInternalInstance() {
    return this->_plaintext;
}
