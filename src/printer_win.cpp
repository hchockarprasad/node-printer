#include "napi.h"
#include "printer.h"
#include "windows.h"
#include "winspool.h"

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
  for (UINT i = 0; i < printers.size(); i++) {      
    obj.Set("printerName", printers[i].printerName);
    obj.Set("portName", printers[i].portName);
    arr[i] = obj;
  }
  deferred.Resolve(arr);
}

int GetPrintersWorker::GetPrinters() {
  DWORD flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;
  int level = 2;
  PRINTER_INFO_2A* printer_info;
  DWORD memsize, printer_count;
  EnumPrinters(flags, NULL, level, NULL, 0, &memsize, &printer_count);

  if (memsize < 1) return 0;

  BYTE* bytes = new BYTE[memsize];

  if (EnumPrinters(flags, NULL, level, bytes, memsize, &memsize, &printer_count))
  {
    printer_info = (PRINTER_INFO_2A*)bytes;
    for (UINT i = 0; i < printer_count; i++) {      
      Printer printer;
      printer.printerName = printer_info->pPrinterName;
      printer.portName = printer_info->pPortName;
      printers.push_back(printer);
      printer_info++;
    }
    return 1;  
  }
  return 0;
}

void GetPrintersWorker::OnError(Napi::Error const &error) {
  deferred.Reject(error.Value());
}
