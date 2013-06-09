/*
  Device using W5100 Ethernet Shield example
 
 This example shows how to create a device that controls using ethernet many components.
 
 The circuit:
 ** EthernetShield W5100
 
 created   Jun 2013
 by Guilherme D'Amoreira
 
 This example code is in the public domain.
 	 
 */
#include <SPI.h>
#include <Ethernet.h>
#include "Device.h"
#include "Network.h"

// Create a device instance
Device device = Device("device-name");
// Create a ethernet server on port 80
EthernetServer server(80);
// Create a network interface for control the device components
Network network = Network(&device, &server);

void setup() {
	/*
	 * Here you put your devices that will be controlled
	 */
	device.add("buzz", PWM, 6, OUTPUT);
	device.add("fan", PWM, 9, OUTPUT);
	device.add("pwm-aux1", PWM, 3, OUTPUT);
	device.add("pwm-aux2", PWM, 5, OUTPUT);

	device.add("speaker", DIGITAL, 4, OUTPUT);
	device.add("relay1", DIGITAL, 7, OUTPUT);
	device.add("relay2", DIGITAL, 8, OUTPUT);
	device.add("relay3", DIGITAL, 15, OUTPUT);
	device.add("relay4", DIGITAL, 14, OUTPUT);
	device.add("led", DIGITAL, 13, OUTPUT);

	/*
	 * Here you put your devices that will be monitored
	 */
	device.add("light", ANALOG, 3, INPUT);
	device.add("temp", ANALOG, 2, INPUT);

	// Open serial communications and wait for port to open:
	Serial.begin(115200);
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}

	// Use only this mac address if there is no one using this on your LAN
	byte mac[] = {0x54,0x55,0x58,0x10,0x00,0x24};
	// Modify for use on your LAN
	IPAddress ip(192, 168, 1, 177);

	// Start network funcionality
	network.startNetwork(ip, mac);

	// Shows the IP used on EthernetShield
	Serial.print("server online at ");
	Serial.println(Ethernet.localIP());
}

void loop() {
	device.loop();
	network.loop();
}