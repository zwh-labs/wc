#ifndef _WCPACKET_H_
#define _WCPACKET_H_

#include <stdint.h>


#define WCPACKET_MAXSIZE 255

#define WCPACKET_MESSAGE_TYPE      0
#define WCPACKET_MESSAGE_MAXLENGTH ( WCPACKET_MAXSIZE - sizeof(WCPacket_Header) )

#define WCPACKET_WHEEL_TYPE        1
#define WCPACKET_WHEEL_LENGTH      ( sizeof(WCPacket_Wheel) - sizeof(WCPacket_Header) )

#define WCPACKET_REQUESTINFO_TYPE  2


#pragma pack(push,1)


typedef struct
{
	uint8_t type;
	uint8_t length;
} WCPacket_Header;


typedef struct
{
	WCPacket_Header header;
} WCPacket;


typedef WCPacket WCPacket_RequestInfo;


typedef struct
{
	WCPacket_Header header;
	char text[WCPACKET_MESSAGE_MAXLENGTH];
} WCPacket_Message;


typedef struct
{
	WCPacket_Header header;
	uint8_t channel;
	uint8_t error;
	int16_t value;
} WCPacket_Wheel;


#pragma pack(pop)


uint16_t WCPacket_size( WCPacket_Header * packet );

void WCPacket_Message_create( WCPacket_Message * message, const char * string );
void WCPacket_Wheel_create( WCPacket_Wheel * wheel, uint8_t channel, uint8_t error, int16_t value );
void WCPacket_RequestInfo_create( WCPacket_RequestInfo * requestInfo );


#endif
