#include "napi.h"
#include "printer.h"

#include <iostream>
#include <string>

GetPrintersWorker:: GetPrintersWorker(const Napi::Env &env) : AsyncWorker(env), deferred(Napi::Promise::Deferred::New(env)) { }

GetPrintersWorker:: ~GetPrintersWorker() {}

Napi::Promise GetPrintersWorker::Promise() {
  return deferred.Promise();
}

void GetPrintersWorker::Execute() {
  std::string error;
  const int result = GetPrinters();
  if (result == 1) success = true;
  if (result == 0) success = false;
}

void GetPrintersWorker::OnOK() {
  const Napi::Env env = Env();
  Napi::Array arr = Napi::Array::New(Env());
  Napi::Object obj = Napi::Object::New(Env());
  for (int i = 0; i < printers.size(); i++) {      
    obj.Set("printerName", printers[i].printerName);
    obj.Set("portName", printers[i].portName);
    arr[i] = obj;
  }
  deferred.Resolve(arr);
}

int GetPrintersWorker::GetPrinters() {
  for (int i = 0; i < 5; i++) {      
    Printer printer;
    printer.printerName = "Hello";
    printer.portName = "Abcd";
    printers.push_back(printer);
  }
  return 1;  
}

void GetPrintersWorker::OnError(Napi::Error const &error) {
  deferred.Reject(error.Value());
}
