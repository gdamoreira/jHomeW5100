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
    
    server->print(device->numberOfComponents);
    server->print("|");
    for (int x = 0; x < device->numberOfComponents; x++) {
        server->print(device->components[x].name);
        server->print("|");
        server->print(device->components[x].getTypeName());
        server->print("|");
        server->print(device->components[x].port);
        server->print("|");
        server->print(device->components[x].getValue());
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
			
			server->println("HTTP/1.1 200 OK");
			server->println("Content-Type: text/html");
			server->println("Connection: close");
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
	boolean eof = false;
	boolean lb = true;
	while (client->connected()) {
		if (client->available()) {
			char c = client->read();
			
			if (!eof) param.concat(c);
			
			if (c == '\n' && lb) {
				break;
			} else if (c == '\n') {
				if (!eof) eof = true;
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