#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <napi.h>
#include "seal/seal.h"

class KeyGenerator : public Napi::ObjectWrap<KeyGenerator> {
 public:
  static Napi::Object init(Napi::Env env, Napi::Object exports);
  KeyGenerator(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::KeyGenerator> getInternalInstance();
  
 private:
  static Napi::FunctionReference constructor;
  std::shared_ptr<seal::KeyGenerator> _generator;
};

class PublicKey : public Napi::ObjectWrap<PublicKey> {
 public:
  static Napi::Object init(Napi::Env env, Napi::Object exports);
  PublicKey(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::PublicKey> getInternalInstance();
  
 private:
  static Napi::FunctionReference constructor;
  Napi::Value save(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::PublicKey> _key;
};

class SecretKey : public Napi::ObjectWrap<SecretKey> {
 public:
  static Napi::Object init(Napi::Env env, Napi::Object exports);
  SecretKey(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::SecretKey> getInternalInstance();
  
 private:
  static Napi::FunctionReference constructor;
  Napi::Value save(const Napi::CallbackInfo& info);
  std::shared_ptr<seal::SecretKey> _key;
};

#endif
