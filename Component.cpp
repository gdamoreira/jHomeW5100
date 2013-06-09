#include <Component.h>
#include <Arduino.h>

//very high cost to have each state in a string, this var is for all objects
char readValue[15];

Component::Component(char* name1, int type1, int port1, int typeio1) {
	name = name1;
	type = type1;
	port = port1;
	typeio = typeio1;

	if (type == DIGITAL) {
		pinMode(port, typeio);
	}

	emptyReadValue();
	state = 0;
}

char* Component::getValue() {
	itoa(state, readValue, 10);
	return readValue;
}

char* Component::getTypeName() {
	static char* typeNames[] = {"DIGITAL", "ANALOG", "PWM", "RELAY", "LIGHT", "TEMPERATURE", "SERIAL"};
	return typeNames[type];
}

char* Component::write(char* c1) {
	if (typeio == OUTPUT) {
		if (type == DIGITAL || type == RELAY) {
			state = atoi(c1);
			digitalWrite(port, state);
			return c1;
		} else if (type == PWM) {
			state = atoi(c1);
			analogWrite(port, atoi(c1));
			return c1;
		} else if (type == SERIAL) {
			Serial.print(c1);
			return c1;
		}
	}
}

void Component::emptyReadValue() {
	for (int i = 0; i < 15; i++) {
		readValue[i] = '\0';
	}
}

char* Component::read() {
	if (typeio == INPUT) {
		emptyReadValue();
		if (type == ANALOG || type == LIGHT || type == TEMP) {
			state = analogRead(port);
			return getValue();
		} else if (type == DIGITAL || type == RELAY || type == PWM) {
			state = digitalRead(port);
			return getValue();
		} else if (type == SERIAL) {
			int counter = 0;
			while (Serial.available() > 0 && counter < 15) {
				char c = Serial.read(); 
				delay(5);
				readValue[counter++] = c;
			}
			return readValue;
		}
	}
	return "\0";
}