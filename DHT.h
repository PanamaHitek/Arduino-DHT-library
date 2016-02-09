/* DHT library by Panama Hitek

MIT license

This library is based on the DHT sensor library by
Adafruit.
Take a look to this awesome library: 
https://github.com/adafruit/DHT-sensor-library

*/

#ifndef DHT_H
#define DHT_H


// Include library depending of Arduino IDE version
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

// Define types of sensors.
#define DHT11 11
#define DHT22 22

class DHT {
	private:
		// Store data comming from the sensor
		uint8_t data[5];

		/* Define the pin where the sensor is connected
		* and sensor type
		*/
		uint8_t _pin, _type;

		// Keep track of the bit and port
		uint8_t _bit, _port;
		uint32_t _lastReadTime, _maxCycles;

		bool _lastResult;

		uint32_t countPulse(bool level);
		bool verifyChecksum();

	public:
		DHT(uint8_t pin, uint8_t type);
		float readTemperature();
		float readHumidity();
		boolean readData();

};

#endif
