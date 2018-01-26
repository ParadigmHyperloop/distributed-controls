/**
 * Paradigm Hyperloop Prototype Node Control Firmware
 *
 * ABOUT:
 * This file defines the controls logic for a single Brake Node.  A Brake Node
 * is special from a generic node in that it is directly connected to all
 * the required sensors and actuators to adequately control the brake Module
 * in the event of an emergency, a crucial element to building a fail-safe
 * vehicle.
 *
 * HARDWARE:
 * This module is designed to control Pod 2 Brakes, with a few changes. The
 * following hardware is expected to be connected to this node:
 *   Actuators
 *    - 1 5/3 Solenoid (2 coils, 3 states - Engage, Closed, Release)
 *   Sensors
 *    - 2 Analog 5V press transducers (Honeywell 150PSI) 1 Tank, 1 Piston
 *    - 2 Analog 5V Sharp Distance Sensors (2-10cm)
 *    - 3 0-5V Cold Comp Thermocouple Amplifiers
 *
 * COMMUNICATIONS:
 * This module assumes an Arduino Compatible Ethernet IC is connected.
 * This module acquires its MAC and IP at compile time.
 *   Defaults:
 *    - MAC_ADDRESS: 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
 *    - IP_ADDRESS: 192, 168, 1, 177
 *   Note:
 *    Static Assignment of MAC and IP address should be investigated, there are
 *    Possibly better alternatives.
 *    - Determine if a MAC can be burned into the NIC once during assembly
 *        -> Then we could avoid compile time and use DHCP.
 *    - Determine if a default MAC is already burned into the NIC
 *    - Determine if we can generate a unique MAC based on the uC SerialUSB #
 *
 * DESIGN:
 * See the infamous KeynoteCAD for design notes on the design of this Prototype
 * controller.
 */

/////////////////////// HEADERS /////////////////////
#include <Arduino.h>
#include <sensor.h>
#include "node.h"
#include "tests.h"

#include <filters/bias.h>
#include <converters/cubic.h>
#include <detectors/high_low.h>
#include <sensor_drivers/internal.h>

#ifndef UNIT_TEST  // IMPORTANT LINE!

BiasFilter Fi(0.5);
CubicConverter C(0,0,1,0);
HighLowDetector D(8, 4086);
InternalSensorDriver SD(1);
Sensor S("test_sensor", &SD, &C, &Fi, &D);
Node N("Test", RUN);
IPAddress ip(IP_ADDRESS);
byte mac[6] = {MAC_ADDRESS};
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
EthernetUDP UDP;

void setup() {
  Ethernet.begin(mac, ip);
  UDP.begin(8888);
  SerialUSB.begin(115200);

  while (!SerialUSB) {
    ; // Waiting for SerialUSB Connection
  }

  info("SerialUSB Connected!");
  test();
  info("==== ALL TESTS PASSED! ====");
}

void loop() {

  // UDP.read(packet_buffer, UDP_TX_PACKET_MAX_SIZE);
  // UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
  // UDP.write("Response");
  // UDP.endPacket();

  int packetSize = UDP.parsePacket();
  if (packetSize)
  {
    SerialUSB.print("Received packet of size ");
    SerialUSB.println(packetSize);
    SerialUSB.print("From ");
    IPAddress remote = UDP.remoteIP();
    for (int i = 0; i < 4; i++)
    {
      SerialUSB.print(remote[i], DEC);
      if (i < 3)
      {
        SerialUSB.print(".");
      }
    }
    SerialUSB.print(", port ");
    SerialUSB.println(UDP.remotePort());

    // read the packet into packetBufffer
    UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    SerialUSB.println("Contents:");
    SerialUSB.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write(packetBuffer);
    UDP.endPacket();
  }

  // S.addValue(1000);
  // info(S.getValue());

  // N.dumpNetworkInfo();

  // info("Completed Loop");
  // delay
  delay(10);
}


#else
///////////////////////////////////////////////////////////////////////////////
///                                  TESTS
///////////////////////////////////////////////////////////////////////////////
#endif
