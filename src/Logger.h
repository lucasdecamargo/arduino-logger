#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <NTPClient.h>

template<class T>
class Logger
{
public:
  enum LogLevel
  {
    LOG_LEVEL_SILENT,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO
  };

  static void setSerial(Print * serial) {_serial = serial;}
  static void setMQTT(T * mqtt, String topic = "log") {_mqtt = mqtt; _mqtt_topic = topic;}
  static void setNTP(NTPClient * ntp) {_ntp = ntp;}

  static void setPrefix(String pref) { _prefix = pref; }

  static void setLevel(unsigned level) {_level = level;}
  static unsigned getLevel() {return _level;}

  static void error(String str) {return log(String("[ERROR]: ") + str, LOG_LEVEL_ERROR);}
  static void warning(String str) {return log(String("[WARNING]: ") + str, LOG_LEVEL_WARNING);}
  static void info(String str){return log(String("[INFO]: ") + str, LOG_LEVEL_INFO);}

protected:
  static void log(const String str, unsigned logLvl)
  {
    if(logLvl <= _level)
    {
      String output = str;
      if(_ntp)
      {
        unsigned long epochTime = _ntp->getEpochTime();
        struct tm *ptm = gmtime((time_t *)&epochTime);
        output = String(ptm->tm_mday) + "/" + String(ptm->tm_mon) + "/" 
          + String(ptm->tm_year + 1900) + " " + _ntp->getFormattedTime() + " "  + _prefix + output;
      }
      else
      {
        output = _prefix + output;
      }

      if(_serial)
        _serial->print(output+"\n");
      if(_mqtt)
      {
        _mqtt->publish(_mqtt_topic.c_str(), output.c_str(), output.length());
      }
    }
  }

private:
  static Print * _serial;
  static T * _mqtt;
  static String _mqtt_topic;
  static NTPClient * _ntp;
  static unsigned _level;
  static String _prefix;
};

template<class T>
Print * Logger<T>::_serial = nullptr;
template<class T>
T * Logger<T>::_mqtt = nullptr;
template<class T>
NTPClient * Logger<T>::_ntp = nullptr;
template<class T>
unsigned Logger<T>::_level = Logger<T>::LOG_LEVEL_WARNING;
template<class T>
String Logger<T>::_mqtt_topic = "";
template<class T>
String Logger<T>::_prefix = "";

#endif
