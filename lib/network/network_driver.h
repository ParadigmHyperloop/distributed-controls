#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>

#ifndef MAC_ADDRESS
#define MAC_ADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
#endif

#ifndef IP_ADDRESS
#define IP_ADDRESS 192, 168, 1, 177
#endif

#ifndef LISTEN_PORT
#define LISTEN_PORT 8888 // UDP
#endif


class NetworkDriver{
private:
  IPAddress ip;
  byte *mac;
  int port;
  char packet_buffer[UDP_TX_PACKET_MAX_SIZE];
  EthernetUDP UDP;

public:
  NetworkDriver(byte mac_address[], int p){
    //ip = Ethernet.localIP();

    //UDP.begin(port);
  }

  ~NetworkDriver(){};

  //Getters and setters
  IPAddress getIP(){
    return ip;
  }

//  byte getMACAddress(){
//    return *mac;
//  }

  char* ReceivePacket(){
    UDP.read(packet_buffer, UDP_TX_PACKET_MAX_SIZE);
    return packet_buffer;
  }

  void SendPacket(){
    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write("Response");
    UDP.endPacket();
  }
};
