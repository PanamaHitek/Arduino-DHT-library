uint8_t data[5];

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

boolean readData() {
	uint32_t now = millis();
	if (now - lastReadTime < 2000) {
		return lastResult;
	}

	lastReadTime = now;

	// Reset 40 bits
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	lastReadTime = now;
	/* Start signal coming from the microcontroller */
	pinMode( DHTPIN,OUTPUT);
	digitalWrite(DHTPIN,LOW);
	delay(18);
	/* Wait for sensor response */
	digitalWrite(DHTPIN,HIGH);
	delayMicroseconds(40);
	/* Ready to read data comming from sensor */
	pinMode(DHTPIN,INPUT);
	
	uint32_t cycles[80];
	
	/* 
	* Cycles is a variable that store the number of pulses 
	* of a certain logic value.
	* Due the sensors represents 0 or 1 logic value using 
	* a low logic pulse after 0 or 1 values I need to create 
	* with a double size
	*/
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

float readHuminity() {
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


float readTemperature() {
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

bool verifyChecksum() {
	if (data[4] == (data[0]+data[1]+data[2]+data[3] & 0xFF)) {
		return true;
	}
	else {
		return false;
	}

}

uint32_t countPulse(bool level) {
	uint32_t count = 0;

	/* Assign the value of bit or 0 to porstate
	 * depending of level. If level = 0, then should be 0
	 * If level = 1, then should be bit
	 */
	uint8_t portState = level ? bit : 0;

}  