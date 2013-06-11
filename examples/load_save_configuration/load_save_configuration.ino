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
#include <EEPROM.h>
#include "Device.h"
#include "Network.h"

// Create a device instance
Device device = Device("device-name");
// Create a ethernet server on port 80
EthernetServer server(80);
// Create a network interface for control the device components
Network network = Network(&device, &server);

void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(115200);    
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}

	/*
	 * Here you load your devices configuration that will be controlled
	 * The address on EEPROM is 0x3E8 (HEX) == 1000 (DEC)
	 */
	device.add("led", DIGITAL, 13, OUTPUT);
	device.save(0x3E8);
	device.load(0x3E8);
	device.discoverySerial();

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