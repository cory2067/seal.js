#include "myobject.h"
#include <iostream>

Napi::FunctionReference MyObject::constructor;

Napi::Object MyObject::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "MyObject", {
    InstanceMethod("plusOne", &MyObject::PlusOne),
    InstanceMethod("value", &MyObject::GetValue),
    InstanceMethod("multiply", &MyObject::Multiply),
    InstanceMethod("lookAt", &MyObject::LookAt)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("MyObject", func);
  return exports;
}

MyObject::MyObject(const Napi::CallbackInfo& info) : Napi::ObjectWrap<MyObject>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
  }

  Napi::Number value = info[0].As<Napi::Number>();
  this->value_ = value.DoubleValue();
}

Napi::Value MyObject::GetValue(const Napi::CallbackInfo& info) {
  double num = this->value_;

  return Napi::Number::New(info.Env(), num);
}

double MyObject::GetInternalValue() {
    return this->value_;
}

Napi::Value MyObject::PlusOne(const Napi::CallbackInfo& info) {
  this->value_ = this->value_ + 1;

  return MyObject::GetValue(info);
}

Napi::Value MyObject::Multiply(const Napi::CallbackInfo& info) {
  Napi::Number multiple;
  if (info.Length() <= 0 || !info[0].IsNumber()) {
    multiple = Napi::Number::New(info.Env(), 1);
  } else {
    multiple = info[0].As<Napi::Number>();
  }

  Napi::Object obj = constructor.New({ Napi::Number::New(info.Env(), this->value_ * multiple.DoubleValue()) });

  return obj;
}

Napi::Value MyObject::LookAt(const Napi::CallbackInfo& info) {
    //std::cout << info[0].As<Napi::Object>().Get("meme").As<Napi::String>().Utf8Value() << std::endl;

    Napi::Object obj = info[0].As<Napi::Object>();
    MyObject * target = Napi::ObjectWrap<MyObject>::Unwrap(obj);

    return Napi::Number::New(info.Env(), target->GetInternalValue());
}
