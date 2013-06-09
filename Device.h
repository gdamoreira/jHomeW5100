#ifndef DEVICE_H
#define DEVICE_H
#include "Component.h"
#include "Arduino.h"
#include "ComponentList.h"

class Device {

private:
	bool found;
	bool serial;

	void split(char*);
	int findComponent(char*) ;

public:
	Device();
	Device(char*);
	Device(char*, boolean);

	char* name;
	ComponentList components;

	void add(char*, int, int, int);
	void remove(char*);
	char* execute(char *);
	void loop();

	void serialServer();
	void discoverySerial();
};
#endif