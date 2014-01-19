#ifndef _WC_PACKET_H_
#define _WC_PACKET_H_


#include "wc_export.h"
#include <stdint.h>


#define WC_PACKET_MAXSIZE 255
#define WC_PACKET_MAXLENGTH ( WC_PACKET_MAXSIZE - sizeof(wcPacket_Header) )

#define WC_PACKET_MESSAGE_TYPE      0
#define WC_PACKET_MESSAGE_MAXLENGTH ( WC_PACKET_MAXSIZE - sizeof(wcPacket_Header) )

#define WC_PACKET_WHEEL_TYPE        1
#define WC_PACKET_WHEEL_LENGTH      ( sizeof(wcPacket_Wheel) - sizeof(wcPacket_Header) )

#define WC_PACKET_REQUESTINFO_TYPE  2


#pragma pack(push,1)


typedef struct
{
	uint8_t type;
	uint8_t length;
} wcPacket_Header;


typedef struct
{
	wcPacket_Header header;
	uint8_t _data[WC_PACKET_MAXLENGTH];
} wcPacket;


typedef struct
{
	wcPacket_Header header;
} wcPacket_RequestInfo;


typedef struct
{
	wcPacket_Header header;
	char text[WC_PACKET_MESSAGE_MAXLENGTH];
} wcPacket_Message;


typedef struct
{
	wcPacket_Header header;
	uint8_t channel;
	uint8_t error;
	int16_t value;
} wcPacket_Wheel;


#pragma pack(pop)


WC_EXPORT uint16_t wcPacket_size( const wcPacket * packet );

WC_EXPORT void wcPacket_Message_create( wcPacket_Message * message, const char * string );
WC_EXPORT void wcPacket_Wheel_create( wcPacket_Wheel * wheel, uint8_t channel, uint8_t error, int16_t value );
WC_EXPORT void wcPacket_RequestInfo_create( wcPacket_RequestInfo * requestInfo );


#endif
