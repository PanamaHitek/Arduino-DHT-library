#include "DHT.h"

#define pin 9
#define model DHT11

// Instance object
DHT dht(pin,model);

void setup() {
  Serial.begin(9600);
  dht.init();
}

void loop() {
  // Wait few seconds
  delay(2000);

  float T = dht.readTemperature();

  if (isnan(T)) {
    Serial.println(F("Something goes wrong... =("));
    return;
  }

  Serial.print(F("Temperature: "));
  Serial.print(T);
  Serial.print(F(" *C" ));
  Serial.println();
}