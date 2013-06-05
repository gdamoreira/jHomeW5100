#ifndef ETHERNET_HOME_H
#define ETHERNET_HOME_H

#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>
#include "Device.h"

class Network  {
  private:

  public:    
    Device* device;
    EthernetServer* server;
	Network(Device* device, EthernetServer* server1);
    void startNetwork(IPAddress ip, byte *mac);
    void loop();
	String serviceRequest(EthernetClient *client);
    void discoveryNetwork();
};
#endif
