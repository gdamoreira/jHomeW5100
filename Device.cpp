#include <EEPROM.h>
#include "Device.h"
#include "Arduino.h"
#include "Component.h"
#include "Vector.h"

Device::Device() {
	serial = true;
	found = false;
	name = "noname1";
}

Device::Device(char* name1) {
	serial = true;
	found = false;
	name = name1;
}

Device::Device(char* name1, boolean serial1) {
	serial = serial1;
	found = false;
	name = name1;
}

char command1[10];
char parameter1[10];

void Device::save(int addr) {
    String serialized = this->serialize();
	for (int i = 0, s = serialized.length(); i < s; i++) {
        EEPROM.write(addr++, serialized.charAt(i));
	}
}

void Device::load(int addr) {
    while (this->components.size() > 0) {
        this->components.remove(0);
    }
    String deserialized = "";
    boolean eof = false;
    char last;
    do {
        char c = EEPROM.read(addr++);
        if (c == '\n' && last == '\n') {
            eof = true;
        } else {
            last = c;
            deserialized.concat(c);
        }
    } while (!eof);
    this->deserialize(deserialized);
}

void Device::add(char* name, int type, int port, int typeio) {
	Component c1 = Component(name, type, port, typeio);
	components.add(c1);
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
	for (int i = 0, s = this->components.size(); i < s; i++) {
		if (strcmp(name, components[i].name) == 0) {
			n = i;
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
	this->split(command);
	Component c = this->components[findComponent(command1)];
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
		for (int x = 0, s = this->components.size(); x < s; x++) {
			Serial.print(this->components[x].name);
			Serial.print("|");
			Serial.print(this->components[x].getTypeName());
			Serial.print("|");
			Serial.print(this->components[x].port);
			Serial.print("|");
			Serial.print(this->components[x].getValue());
			Serial.print("|");
		}
		Serial.print("\n");
	}
}

String Device::serialize() {
    String serialized = "";
    for (int i = 0, s = this->components.size(); i < s; i++) {
        serialized.concat(this->components[i].name);
        serialized.concat('|');
        serialized.concat(this->components[i].port);
        serialized.concat('|');
        serialized.concat(this->components[i].state);
        serialized.concat('|');
        serialized.concat(this->components[i].type);
        serialized.concat('|');
        serialized.concat(this->components[i].typeio);
        serialized.concat('|');
        serialized.concat('\n');
    }
    serialized.concat('\n');
    return serialized;
}

void Device::deserialize(String serialized) {
    String auxName;
    int port;
    int state;
    int type;
    int typeio;

    int count = 0;
    String aux = "";
    int l = serialized.length();
    for (int i = 0; i < l; i++) {
        if (serialized.charAt(i) == '|') {
            if (count == 0) { //name
                auxName = aux;
            } else if (count == 1) { //port
                port = aux.toInt();
            } else if (count == 2) { //state
                state = aux.toInt();
            } else if (count == 3) { //type
                type = aux.toInt();
            } else if (count == 4) { //typeio
                typeio = aux.toInt();

                //save component on vector
                this->add(this->getCharPointer(auxName), type, port, typeio);
            }
            count++;
            aux = "";
        } else if (serialized.charAt(i) == '\n') {
            count = 0;
            aux = "";
        } else {
            aux.concat(serialized.charAt(i));
        }
    }
}

char* Device::getCharPointer(String aux) {
    char* str = (char*) malloc(aux.length() + 1);
    aux.toCharArray(str, aux.length() + 1);
    str[aux.length()] = '\0';
    return str;
}
