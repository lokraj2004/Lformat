#ifndef LFORMAT_H
#define LFORMAT_H

#include <Arduino.h>

String Lformat(String name = "NULL", double value = -1, String unit = "NULL", int id = 0);
String LBFormat(const String &label, double value, const String &unit, int sensorId);
#endif
