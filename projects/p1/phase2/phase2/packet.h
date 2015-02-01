/*
 * packet.h
 *
 *  Created on: 26-Apr-2009
 *	Updated on: 26-Jan
 *      Author: Neil MacMillan, Daniel McIlvaney
 */

#ifndef PACKET_H_
#define PACKET_H_


#include <avr/io.h>

#include "sensor_struct.h"
#include "cops_and_robbers.h"

/*****					Add labels for the packet types to the enumeration					*****/

typedef enum _pt
{
	COMMAND,			/// a command from the client station to the Roomba
	SENSOR_DATA,		/// a reply from the Roomba, containing its sensor status data
	IR_COMMAND,			/// a command to the Roomba's IR transmitter
	IR_DATA,			/// a signal received from the Roomba's IR receiver
	MESSAGE				/// a generic message (not implemented by the Roomba)
} PACKET_TYPE;



/*****							Construct payload format structures							*****/
//								structures must be 29 bytes long or less.


/* Each Roomba is aware of its number and will always attach it to any
	reply back to a caller. The Roombas will reply to any command with
	data of the associated type (Roomba -> sensor data, IR -> IR signal) sent
	back to the calling radio.
	*/
typedef struct _cmd
{
	uint8_t sender_address[5];	/// The return address of the station sending this packet
	uint8_t command;			/// The command byte for the Roomba to execute (128 to 143).  This is passed
								///		directly into the Roomba.
	uint8_t num_arg_bytes;		/// The number of bytes taken up by arguments in the arguments array below (0 to 16)
	uint8_t arguments[16];		///	The arguments to the command.  The bytes in here are sent to the Roomba in order
								///		starting at arguments[0], after the command byte is sent
} pf_command_t;

typedef struct _sensors
{
	uint8_t roomba_number;
	roomba_sensor_data_t sensors;	/// The Roomba sensor packet.  This will be sent from the Roomba station
									///		in response to any COMMAND packet.  It is 26 bytes. (see sensor_struct.h)
} pf_sensors_t;

typedef struct _ir_command {
	uint8_t sender_address[5];
	uint8_t ir_command;				/// The IR transmitter or servo will execute this command. (See Cops_and_robbers.h)
	uint8_t ir_data;				/// The data transmitted by the SEND_BYTE command.
	uint8_t servo_angle;			/// The angle to aim the servo at.
} pf_ir_command_t;

typedef struct _ir_data {
	uint8_t roomba_number;			
	uint8_t ir_data;				/// The last received IR signal
} pf_ir_data_t;

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
	uint8_t _filler[29];	// makes sure the packet is exactly 32 bytes long - this array should not be accessed directly.
	pf_command_t command;
	pf_sensors_t sensors;
	pf_ir_data_t ir_data;
	pf_ir_command_t ir_command;
	pf_message_t message;
} payloadformat_t;

/*****						Leave the radiopacket_t structure alone.						*****/

typedef struct _rp
{
	uint8_t type;
	uint16_t timestamp;
	payloadformat_t payload;
} radiopacket_t;

#endif /* PACKET_H_ */
