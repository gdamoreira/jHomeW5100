#ifndef DEVICE_H
#define DEVICE_H

#include <EEPROM.h>
#include "Component.h"
#include "Arduino.h"
#include "Vector.h"

class Device {

private:
	bool found;
	bool serial;

	void split(char*);
	int findComponent(char*);
	char* getCharPointer(String);

	String serialize();
	void deserialize(String);
public:
	Device();
	Device(char*);
	Device(char*, boolean);

	char* name;
	Vector<Component> components;

	void add(char*, int, int, int);
	void remove(char*);
	char* execute(char *);
	void loop();

	void serialServer();
	void discoverySerial();

    void save(int);
	void load(int);
};
#endif
