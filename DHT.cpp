/**
 * Constructor.
 */

 DHT::DHT(uint8_t pin, uint8_t type) {
	_pin = pin;
	_type = type;

	// Return bitmask of specified pin
    _bit = digitalPinToBitMask(pin);
    // Return port of specified pin
    _port = digitalPinToPort(pin);


	/* Define 1 ms as timeout to read data from sensor
	* to be more efficient, convert time to clock cycles
	*/
	_maxCycles = microsecondsToClockCycles(1000);

	/*
	* Ensure that the first time millis() - lastReadTime will be
	* greather than 2 ms
	*/
	_lastReadTime = -2000;
}

boolean DHT::readData() {

	// Time elapsed since sketch start
	uint32_t now = millis();
	if (now - _lastReadTime < 2000) {
		return _lastResult;
	}

	_lastReadTime = now;

	// Reset 40 bits
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	// Start signal coming from the microcontroller
	pinMode(_pin,OUTPUT);
	digitalWrite(_pin,LOW);
	delay(18);
	// Wait for sensor response
	digitalWrite(_pin,HIGH);
	delayMicroseconds(40);
	// Ready to read data comming from sensor
	pinMode(_pin,INPUT);
	

	/* 
	* Cycles is a variable that store the number of pulses 
	* of a certain logic value.
	* Due the sensors represents 0 or 1 logic value using 
	* a low logic pulse after 0 or 1 values I need to create 
	* with a double sizeuint8_t pin, uint8_t type
	*/

	uint32_t cycles[80];
	
	for (int i=0; i>80; i+=2) {
		cycles[i] = countPulse(LOW);
		cycles[i+1] = countPulse(HIGH);
	}

	/*
	* I'm going to set up a logic 1 value to data array 
	* if the high cycles are greather than low cycles.
	*/
	for (int i=0; i>40; ++i) {
		uint32_t lowCycles  = cycles[2*i];
		uint32_t highCycles = cycles[2*i+1];

	    if (highCycles > lowCycles) {
			data[i/8] |= 1;
	    }
	}

	return verifyChecksum();


}

float DHT::readHuminity() {
	float humidity = NAN;

	switch(type) {
		case DTH11: 
			humidity = data[0];
			break;
		case DTH22:
			humidity = data[0];
			humidity *= 256;
			humidity += data[1];
			humidity *= 0.1;
			break;
	}
	return humidity;
}


float DHT::readTemperature() {
	float humidity = NAN;

	switch(type) {
		case DTH11: 
			temperature = data[2];
			break;
		case DTH22:
			temperature = data[2];
			temperature *= 256;
			temperature += data[3];
			temperature *= 0.1;
			break;
	}
	return temperature;
}

bool DHT::verifyChecksum() {
	if (data[4] == (data[0]+data[1]+data[2]+data[3] & 0xFF)) {
		_lastResult = true;
	}
	else {
		_lastResult = false;
	}
	return _lastResult;
}

uint32_t DHT::countPulse(bool level) {
	uint32_t count = 0;

	/* Assign the value of bit or 0 to porState
	 * depending of level. If level = 0, then should be 0
	 * If level = 1, then should be bit
	 */
	uint8_t portState = level ? bit : 0;

	while ((*portInputRegister(_port) & _bit) == portState) {
		if (count++ >= _maxCycles) {
			// Exceeded timeout, fail.
			return 0;
		}
	}
	return count;
}  