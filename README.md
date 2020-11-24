# Logger

### Definition

`template<class T> class Logger;`

This is a **static class** implemented as an alternative to the conventional `Arduino.Serial `output method. This class encapsulates output operations such as printing to COM port and logging to the MQTT broker.

#### Requirements

* [NTPClient](https://github.com/arduino-libraries/NTPClient)
* If you're logging to MQTT, I recommend:
  * [PubSubClient](https://github.com/knolleary/pubsubclient)
  * If it is an ESP8266 Broker: [uMQTTBroker](https://github.com/martin-ger/uMQTTBroker)

#### Usage

You must redefine the class type passing the correct class template `T`. This template should be a valid MQTT Client class like `PubSubClient` or `uMQTTBroker`. Use `typedef` to simplify its definition at the global scope of your code:  
`typedef Logger<PubSubClient> Log;`

You can also define a logging level, `LOG_LEVEL_WARNING` will be defined as default.  
`Log::setLevel(Log::LOG_LEVEL_INFO);`

To print out to serial port, on your Arduino Setup scope, call:  
`Log::setSerial(&Serial);`

To activate MQTT logging, use the method below:  
`Logger<T>::setMQTT(T * mqtt, String topic = "log");`

To log a message, use one of the logging methods (described below) *e.g.*  
`Log::error("Some error message.");`  
`Log::warning("Some warning message.");`  
`Log::info("Some info message.");`

**Note:** remember that this is a static templated class; so different template definitions will result in duplicated static class construction.

### Macros

* `LOG_LEVEL_SILENT`
* `LOG_LEVEL_ERROR`
* `LOG_LEVEL_WARNING` (default)
* `LOG_LEVEL_INFO`

### Static Public Methods

#### `static void setSerial(Print * serial)`

Defines and activates a serial output class. You would normally pass Arduino's Serial as argument `Logger<T>::setSerial(&Serial)`.

#### `static void setMQTT(T * mqtt, String topic = "log")`

Defines and activates MQTT logging. Note that type `T` must correspond to your MQTT client class. This MQTT client class **should** have a publish method like below:  
`publish(const char* topic, const uint8_t * payload, unsigned int plength);`  
If it doesn't, you can wrap your class by using inheritance and redefining it's publish method. It was tested with `PubSubClient` and `uMQTTBroker`.

#### `static void setNTP(NTPClient * ntp)`

Defines and activates date time prefix logging. **Requires Internet connection**. Example of usage:

```c++
#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"pool.ntp.org");
const char* UTC_TIME = -3;

void setup{
    // ... //
    connect_to_internet();
    timeClient.begin();
    timeClient.setTimeOffset(UTC_TIME*3600);
    timeClient.update();
    Log::setNTP(&timeClient);
}
```

#### `static void setPrefix(String pref)`

Defines a logging prefix string for the whole application.

#### `static void setLevel(unsigned level)`

Sets a logging level. See **macros** definitions above.

#### `static unsigned getLevel()`

Returns current logging level.

#### `static void error(String str)`

Logs an error message.

#### `static void warning(String str)`

Logs a warning message.

#### `static void info(String str)`

Logs an info message.
