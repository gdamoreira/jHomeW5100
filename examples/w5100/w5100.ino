#include "Device.h"

#include <SPI.h>
#include <Ethernet.h>
#include "Network.h"

Device homeDevice=Device("central-device");
EthernetServer server(80);
byte mac[] = {0x54,0x55,0x58,0x10,0x00,0x24};
IPAddress ip(192, 168, 1, 177);
Network network = Network(&homeDevice, &server);

void setup() {
	homeDevice.add("buzz", PWM, 6);
	homeDevice.add("fan", PWM, 9);
	homeDevice.add("pwm-aux1", PWM, 3);
	homeDevice.add("pwm-aux2", PWM, 5);

	homeDevice.add("speaker", DIGITAL, 4);
	homeDevice.add("relay1", DIGITAL, 7);
	homeDevice.add("relay2", DIGITAL, 8);
	homeDevice.add("relay3", DIGITAL, 15);
	homeDevice.add("relay4", DIGITAL, 14);
	homeDevice.add("led", DIGITAL, 13);

	homeDevice.add("light", ANALOG, 3);
	homeDevice.add("temp", ANALOG, 2);

	Serial.begin(115200);

	network.startNetwork(ip, mac);
	Serial.print("server online at ");
	Serial.println(Ethernet.localIP());
}

void loop() {
	homeDevice.loop();
	network.loop();
}
