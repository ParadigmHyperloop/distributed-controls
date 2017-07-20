#ifndef PARADIGM_NETWORK_H
#define PARADIGM_NETWORK_H

#include "cdefs.h"
#include "config.h"
#include <Ethernet.h>
#include <EthernetUdp.h> // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <SPI.h>         // needed for Arduino versions later than 0018

/////////////////////// NETWORK PACKETS /////////////////////

/**
 * Represents a v0.1 Pod Bus request Packet
 *
 * These request packets are simple, there is an 8 bit mode argument that is
 * used by the master to tell the node what mode is should be in.  Optionally,
 * higher order bits may be used to flag the packet type and thus tell the node
 * how to interpret arg0 and arg1
 */
typedef struct req_packet {
  uint8_t mode;  // Mode to set node to
  int32_t arg0;  // First 32 bit Ardument
  int32_t arg1;  // Second 32 bit Argument
} req_packet_t;

/**
 * Represents a v0.1 Pod Bus response Packet for brake modules
 */
typedef struct resp_packet {
  uint8_t mode; // The current mode the node is in
  int32_t tank_press; // Pressure in the tank
  int32_t piston_press; // Pressure in the piston
  int32_t dist_front; // Front pad distance sensor
  int32_t dist_rear; // Rear pad distance sensor
  int32_t valve_state; // State of the valve
  int32_t temp_front; // Temp on the front of the pad
  int32_t temp_rear;  // Temp on the rear of the pad
  int32_t force_setpoint;  // Current force setpoint
} resp_packet_t;

/**
 * Recieve a packet and write it to the buffer
 *
 * @return number of packets sent (-1 failure, 0 for no packets, 1 for success)
 */
int net_recv_request(req_packet_t *pkt);

/**
 * Send the given response packet to the sender
 *
 * @return number of packets sent (-1 failure, 0 for invalid packet, 1 for success)
 */
int net_send_response(resp_packet_t *pkt);


/**
 * Setup the "networking interface" for communications.  This networking API
 * is completely agnositic of the network medium up to layer 4. For the
 * purposes of this application, the network medium could be RS485, Ethernet,
 * CANBus, or any other bus.  The API makes no guarentees about delivery
 * of packets or recipt of packets.  Congestion control may be implemented
 * depending on the implementaion.
 *
 * @return 0 on success, -1 on failure
 */
int net_setup(void);

#endif
