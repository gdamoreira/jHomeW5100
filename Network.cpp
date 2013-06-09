#include <SPI.h>
#include <Ethernet.h>
#include "Network.h"
#include "Arduino.h"

Network::Network(Device* device1, EthernetServer* server1) {
	device = device1;
	server = server1;
}

void Network::startNetwork(IPAddress ip, byte *mac) {
	Ethernet.begin(mac, ip);
	server->begin();
}

void Network::discoveryNetwork() {
	Serial.println("discovering network...");
	server->print(device->name);
	server->print("|");

	server->print(device->components.size());
	server->print("|");
	for (int x = 0; x < device->components.size(); x++) {
		server->print(device->components.get(x)->name);
		server->print("|");
		server->print(device->components.get(x)->getTypeName());
		server->print("|");
		server->print(device->components.get(x)->port);
		server->print("|");
		server->print(device->components.get(x)->getValue());
		server->println("|");
	}
}

void Network::loop() {
	EthernetClient client = server->available();
	if (client) {
		String str = serviceRequest(&client);
		if (str) {
			char param[str.length()];
			str.toCharArray(param, str.length() - 1);
			
			server->println(PSTR("HTTP/1.1 200 OK"));
			server->println(PSTR("Content-Type: text/html"));
			server->println(PSTR("Connection: close"));
			server->println();

			if (param[0] == '\0') {
				discoveryNetwork();
			} else if (strcmp("discovery", param) == 0) {
				discoveryNetwork();
			} else {
				Serial.print("trying ");
				Serial.print(param);
				Serial.println("...");
				server->write(device->execute(param));
			}
		}
		delay(1);
        client.stop();
	}
}

String Network::serviceRequest(EthernetClient *client) {
	String param = "";
	boolean eol = false;
	boolean lb = true;
	while (client->connected()) {
		if (client->available()) {
			char c = client->read();
			
			if (!eol) param.concat(c);
			
			if (c == '\n' && lb) {
				break;
			} else if (c == '\n') {
				if (!eol) eol = true;
				lb = true;
			} else if (c != '\r') {
				lb = false;
			}
		}
	}

	param.replace("POST /", "");
	param.replace("GET /", "");
	param.replace(" HTTP/1.1", "");
	
	return param;
}