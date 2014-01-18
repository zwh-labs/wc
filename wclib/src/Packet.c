#include <wc/Packet.h>

#include <string.h>


uint16_t wcPacket_size( const wcPacket * packet )
{
	return sizeof(wcPacket_Header) + (uint16_t)(packet->header.length);
}


void wcPacket_Message_create( wcPacket_Message * message, const char * text )
{
	message->header.type = WC_PACKET_MESSAGE_TYPE;
	message->header.length = strlen(text) + 1;
	if( message->header.length > WC_PACKET_MESSAGE_MAXLENGTH )
		message->header.length = WC_PACKET_MESSAGE_MAXLENGTH;
	strncpy( message->text, text, message->header.length );
}


void wcPacket_Wheel_create( wcPacket_Wheel * wheel, uint8_t channel, uint8_t error, int16_t value )
{
	wheel->header.type = WC_PACKET_WHEEL_TYPE;
	wheel->header.length = WC_PACKET_WHEEL_LENGTH;
	wheel->channel = channel;
	wheel->error = error;
	wheel->value = value;
}


void wcPacket_RequestInfo_create( wcPacket_RequestInfo * requestInfo )
{
	requestInfo->header.type = WC_PACKET_REQUESTINFO_TYPE;
	requestInfo->header.length = 0;
}
