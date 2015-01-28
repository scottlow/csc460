/**
 * \file	packet.h
 * \brief	Data structures defining the packets used for radio communication
 *
 *			Originally written by Neil MacMillan.  Modified by the authors.
 *
 *	@date 08/21
 *
 *	@author	Gordon Meyer
 *	@author Daniel McIlvaney
 *	@author Cale McNulty
 */ 

#ifndef PACKET_H_
#define PACKET_H_

#include <avr/io.h>

/*****					Add labels for the packet types to the enumeration					*****/

typedef enum _pt
{
	MESSAGE
} PACKET_TYPE;

typedef enum _ec
{
	BAD_STATE
} ERROR_CODE;

/*****							Construct payload format structures							*****/

// structures must be 29 bytes long or less.

typedef struct _msg
{
	uint8_t address[5];
	uint8_t messagecontent[24];
} pf_message_t;


/*****							Add format structures to the union							*****/

/// The application-dependent packet format.  Add structures to the union that correspond to the packet types defined
/// in the PACKET_TYPE enumeration.  The format structures may not be more than 29 bytes long.  The _filler array must
/// be included to ensure that the union is exactly 29 bytes long.
typedef union _pf
{
	uint8_t _filler[29];	// make sure the packet is exactly 32 bytes long - this array should not be accessed directly.
	pf_message_t message;
} payloadformat_t;

/*****						Leave the radiopacket_t structure alone.						*****/

typedef struct _rp
{
	uint8_t type;
	uint16_t sender;
	payloadformat_t payload;
} radiopacket_t;

#endif /* PACKET_H_ */
