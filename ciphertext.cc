#include "ciphertext.h"
#include "encryptor.h"
#include "plaintext.h"
#include <iostream>
#include <fstream>

Napi::FunctionReference Ciphertext::constructor;

Napi::Object Ciphertext::init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Ciphertext", {
    InstanceMethod("save", &Ciphertext::save)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Ciphertext", func);
  return exports;
}

Ciphertext::Ciphertext(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Ciphertext>(info)  {
    Napi::Env env = info.Env();

    if (info.Length() == 0) {
        // default construtor, empty ciphertext
        this->_ciphertext = std::make_shared<seal::Ciphertext>();
    } else if (info.Length() == 1) {
        // interpret singular arg as path
        std::string path = info[0].As<Napi::String>().Utf8Value();
        std::ifstream infile(path, std::ifstream::binary);
        this->_ciphertext = std::make_shared<seal::Ciphertext>();
        this->_ciphertext->load(infile);
    } else {
        // extract seal::Encryptor from parameter 1
        Napi::Object obj = info[0].As<Napi::Object>();
        auto encryptor = Napi::ObjectWrap<Encryptor>::Unwrap(obj)->getInternalInstance();
        
        // extract seal::Plaintext from parameter 2
        obj = info[1].As<Napi::Object>();
        auto plaintext = Napi::ObjectWrap<Plaintext>::Unwrap(obj)->getInternalInstance();
        
        this->_ciphertext = std::make_shared<seal::Ciphertext>();
        encryptor->encrypt(*plaintext, *this->_ciphertext);
    }
}

std::shared_ptr<seal::Ciphertext> Ciphertext::getInternalInstance() {
    return this->_ciphertext;
}

Napi::Value Ciphertext::save(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a path").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string path = info[0].As<Napi::String>().Utf8Value();

    std::ofstream outfile(path, std::ofstream::binary);
    this->_ciphertext->save(outfile);
    return env.Null();
}
