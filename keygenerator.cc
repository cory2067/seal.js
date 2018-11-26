#include "keygenerator.h"
#include "sealcontext.h"
#include <iostream>
#include <fstream>

Napi::FunctionReference KeyGenerator::constructor;

Napi::Object KeyGenerator::init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "KeyGenerator", {
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("KeyGenerator", func);
  return exports;
}

KeyGenerator::KeyGenerator(const Napi::CallbackInfo& info) : Napi::ObjectWrap<KeyGenerator>(info)  {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 argument").ThrowAsJavaScriptException();
		return;
    }

    // extract seal::SEALContext from parameter 1
    Napi::Object obj = info[0].As<Napi::Object>();
    auto context = Napi::ObjectWrap<SEALContext>::Unwrap(obj)->getInternalInstance();

    this->_generator = std::make_unique<seal::KeyGenerator>(context);
}

std::shared_ptr<seal::KeyGenerator> KeyGenerator::getInternalInstance() {
    return this->_generator;
}

// Public Key
Napi::FunctionReference PublicKey::constructor;

Napi::Object PublicKey::init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "PublicKey", {
    InstanceMethod("save", &PublicKey::save),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("PublicKey", func);
  return exports;
}

PublicKey::PublicKey(const Napi::CallbackInfo& info) : Napi::ObjectWrap<PublicKey>(info)  {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 argument").ThrowAsJavaScriptException();
		return;
    }

    if (info[0].IsString()) {
        // load from string
        std::cout << "load from string tbd\n";
    } else { // assume passed in a keygen
        // extract seal::KeyGenerator from parameter 1
        Napi::Object obj = info[0].As<Napi::Object>();
        auto generator = Napi::ObjectWrap<KeyGenerator>::Unwrap(obj)->getInternalInstance();
        
        this->_key = std::make_shared<seal::PublicKey>(generator->public_key());
    }
}

std::shared_ptr<seal::PublicKey> PublicKey::getInternalInstance() {
    return this->_key;
}

Napi::Value PublicKey::save(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a path").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string path = info[0].As<Napi::String>().Utf8Value();

    std::ofstream outfile(path, std::ofstream::binary);
    this->_key->save(outfile);
    return env.Null();
}

// Secret Key
Napi::FunctionReference SecretKey::constructor;

Napi::Object SecretKey::init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "SecretKey", {
    InstanceMethod("save", &SecretKey::save),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("SecretKey", func);
  return exports;
}

SecretKey::SecretKey(const Napi::CallbackInfo& info) : Napi::ObjectWrap<SecretKey>(info)  {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected 1 argument").ThrowAsJavaScriptException();
		return;
    }

    if (info[0].IsString()) {
        // load from string
        std::cout << "load from string tbd\n";
    } else { // assume passed in a keygen
        // extract seal::KeyGenerator from parameter 1
        Napi::Object obj = info[0].As<Napi::Object>();
        auto generator = Napi::ObjectWrap<KeyGenerator>::Unwrap(obj)->getInternalInstance();
        
        this->_key = std::make_shared<seal::SecretKey>(generator->secret_key());
    }
}

std::shared_ptr<seal::SecretKey> SecretKey::getInternalInstance() {
    return this->_key;
}

Napi::Value SecretKey::save(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a path").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string path = info[0].As<Napi::String>().Utf8Value();

    std::ofstream outfile(path, std::ofstream::binary);
    this->_key->save(outfile);
    return env.Null();
}
