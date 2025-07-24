#include "Lformat.h"

String Lformat(String name, double value, String unit, int id) {
  // Error handling
  if (id < 1 || id > 10) {
    Serial.println("[ERROR] Invalid ID. Must be between 1 and 10.");
    Serial.println("Syntax: Lformat(\"name\", value, \"unit\", id);");
    while (true);  // Stop execution
  }

  if (name.length() == 0) {
    Serial.println("[ERROR] Name cannot be empty.");
    Serial.println("Syntax: Lformat(\"name\", value, \"unit\", id);");
    while (true);
  }

  if (unit.length() == 0) {
    Serial.println("[ERROR] Unit cannot be empty.");
    Serial.println("Syntax: Lformat(\"name\", value, \"unit\", id);");
    while (true);
  }

  // Valid input: generate and print batch
  String Batch = name + ";" + String(value) + ";" + unit + ";" + String(id);
  Serial.println(Batch);  // Print inside the function
  return Batch;
}
