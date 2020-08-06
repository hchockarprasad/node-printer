#include <iostream>
#include "napi.h"
#include "printer.h"

using namespace std;

Napi::Value GetPrinters(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  GetPrintersWorker* worker = new GetPrintersWorker(env);
  worker->Queue();
  return worker->Promise();
}

Napi::Object Init(Napi:: Env env, Napi:: Object exports) {
  exports.Set("getPrinters", Napi::Function::New(env, GetPrinters));
  return exports;
}

NODE_API_MODULE(printer, Init)
