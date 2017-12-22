#include "network.h"
/////////////////////// NETWORK SETUP /////////////////////

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 91);

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; // buffer to hold incoming packet,
char ReplyBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to build outgoing packet,

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

int net_recv_request(req_packet_t *pkt) {
  debug("Requesting Packet");
  int packetSize = Udp.parsePacket();
  debug("Packet Back");
  if (packetSize) {
    debug("Received packet of size ");
    debug(packetSize);
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++)
    {
      SerialUSB.print(remote[i], DEC);
      if (i < 3)
      {
        SerialUSB.print(".");
      }
    }
    SerialUSB.print(", port ");
    SerialUSB.println(Udp.remotePort());

    Udp.read((uint8_t *)pkt, sizeof(req_packet_t));
    debug("Contents:");
    debug((char *)pkt);
  }
  return (packetSize == sizeof(req_packet_t) ? 1 : 0);
}

int net_send_response(resp_packet_t *pkt) {
  debug("Sending Response: ");
  debug((char *)pkt);
  // send a reply to the IP address and port that sent us the packet we
  // received
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write((uint8_t *)pkt, sizeof(resp_packet_t));
  Udp.endPacket();
  return 1;
}

int net_setup(void) {
  // start the Ethernet and UDP
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);

  return 0;
}
