# ardoducky

A [Rubber Ducky](http://usbrubberducky.com/) clone for Pro Micro/Leonardo.

### Steps
0. Get a Pro Micro ([AliExpress link](https://www.aliexpress.com/item/Mini-Leonardo-Pro-Micro-ATmega32U4-5V-16MHz-Module-For-Arduino-Best-Quality/32284746884.html)). Leonardo works as well.
1. Compile an [ardoducky script](https://github.com/jerwuqu/ardoducky/blob/master/example/hello_world.ads) using the `adscompiler.py` with [Python 3](https://www.python.org/downloads/).
2. Open the [Arduino IDE](https://www.arduino.cc/en/Main/Software), make sure it's including the `script.h` you just compiled, and compile & upload the project to your Pro Micro or Leonardo.
3. Watch stuff happen.

### Compared to original Rubber Ducky
* Way cheaper. Only $3 from AliExpress.
* No SD-card slot unless you add one yourself.

### License
[MIT](https://opensource.org/licenses/MIT)
