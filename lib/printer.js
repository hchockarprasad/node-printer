var printer = require('../build/Release/node-printer.node')

printer.getPrinters().then((result) => {
  console.log(result);
});

module.exports = {
  getPrinters: function () {
    printer.getPrinters();
  }
}