#ifndef SRC_PRINTER_H_
#define SRC_PRINTER_H_

#include "napi.h"

#include <string>

using namespace std;

struct Printer {
  string printerName;
  string portName;
};

class GetPrintersWorker : public Napi::AsyncWorker {
  
  public:
    GetPrintersWorker(const Napi::Env &env);

    ~GetPrintersWorker();

    void Execute();
    void OnOK();
    void OnError(Napi::Error const &error);
    Napi::Promise Promise();

  private:
    vector<Printer> printers;
    int GetPrinters();
    bool success;
    const Napi::Promise::Deferred deferred;
};

#endif
