#include "Device.h"
#include "Arduino.h"
#include "Component.h"
#include <vector>

Device::Device() {
	components = ComponentList();
	serial = true;
	found = false;
	name = "noname1";
}

Device::Device(char* name1) {
	components = ComponentList();
	serial = true;
	found = false;
	name = name1;
}

Device::Device(char* name1, boolean serial1) {
	components = ComponentList();
	serial = serial1;
	found = false;
	name = name1;
}

char command1[10];
char parameter1[10];

void Device::add(char* name, int type, int port, int typeio) {
	Component c1 = Component(name, type, port, typeio);
	components.add(&c1);
}


void Device::remove(char* name) {
	int index = this->findComponent(name);
	if (found) {
		this->components.remove(index);	
	}
}

int Device::findComponent(char* name) {
	int n = 0;
	found = false;
	for (int x = 0; x < this->components.size(); x++) {
		if (strcmp(name, components.get(x)->name) == 0) {
			n = x;
			found = true;
			break;
		}
	}
	return n;
}

void Device::loop() {
	if (serial && Serial.available()) {
		serialServer();
	}
}

void Device::split(char* command) {
	for (int x = 0; x < 15; x++) {
		command1[x] = '\0';
		parameter1[x] = '\0';
	}
	int cv = 0;
	int cp = 0;
	for (int x = 0; x < 15; x++) {
		if (command[x] != '?' && command[x] != '\0') {
			command1[cv++] = command[x];
		} else {
			cp = command[x] == '?' ? x : 0;
			break;
		}
	}

	if (cp > 0) {
		for (int x = cp + 1; x < 15; x++) {
			parameter1[x-cp-1] = command[x];
			if (command[x] == '\0') break;
		}
	} 
}

char* Device::execute(char* command) {
	split(command);
	Component &c = *this->components.get(findComponent(command1));
	if (!found) {
		return "\n";
	}
	if (parameter1[0] == '\0') {
		char* r = c.read();
		return r;
	} else {
		Serial.println(parameter1);
		return c.write(parameter1);
	}
}

void Device::serialServer() {
	char command[15];
	int counter = 0;
	while (Serial.available() > 0) {
		char c = Serial.read(); 
		delay(10);
		command[counter++] = c;
	}
	if (counter > 0) {
		command[counter] = '\0';

		if (strcmp("discovery", command) == 0) {
			discoverySerial();
			return;
		}

		char* r = execute(command);
		if (r[0] != '\0') {
			Serial.print(r);
			Serial.flush();
		}
	}
}

void Device::discoverySerial() {
	if (serial) {
		Serial.print(name);
		Serial.print("|");
		Serial.print(this->components.size());
		Serial.print("|");
		for (int x = 0; x < this->components.size(); x++) {
			Serial.print(this->components.get(x)->name);
			Serial.print("|");
			Serial.print(this->components.get(x)->getTypeName());
			Serial.print("|");
			Serial.print(this->components.get(x)->port);
			Serial.print("|");
			Serial.print(this->components.get(x)->getValue());
			Serial.print("|");
		}
	}
}