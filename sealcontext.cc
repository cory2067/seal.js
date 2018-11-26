#include "sealcontext.h"
#include <iostream>

Napi::FunctionReference SEALContext::constructor;

Napi::Object SEALContext::init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "SEALContext", {
	// no instance methods for now
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("SEALContext", func);
  return exports;
}

SEALContext::SEALContext(const Napi::CallbackInfo& info) : Napi::ObjectWrap<SEALContext>(info)  {
    Napi::Env env = info.Env();

    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected 3 arguments").ThrowAsJavaScriptException();
		return;
    }

    if (!info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()) {
        Napi::TypeError::New(env, "Expected numeric arguments").ThrowAsJavaScriptException();
        return;
    }

    uint32_t poly_mod = info[0].As<Napi::Number>().Uint32Value();
    uint32_t coeff_mod = info[1].As<Napi::Number>().Uint32Value();
    uint32_t plain_mod = info[2].As<Napi::Number>().Uint32Value();

    seal::EncryptionParameters params(seal::scheme_type::BFV);
    params.set_poly_modulus_degree(poly_mod);
    params.set_plain_modulus(plain_mod);

    switch (coeff_mod) {
        case 128:
            params.set_coeff_modulus(seal::coeff_modulus_128(poly_mod));
            break;
        case 192:
            params.set_coeff_modulus(seal::coeff_modulus_192(poly_mod));
            break;
        case 256:
            params.set_coeff_modulus(seal::coeff_modulus_256(poly_mod));
            break;
        default:
            Napi::Error::New(env, "Invalid coeff modulus").ThrowAsJavaScriptException();
            return;
    }

    this->_context = seal::SEALContext::Create(params);
}

std::shared_ptr<seal::SEALContext> SEALContext::getInternalInstance() {
    return this->_context;
}
