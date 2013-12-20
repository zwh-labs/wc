#ifndef _WCERROR_H_
#define _WCERROR_H_


typedef enum WCError
{
	WCERROR_NOERROR     =      0, ///< no error detected
	WCERROR_UNKNOWN     = (1<<0), ///< unknown error
	WCERROR_WHEELSIGNAL = (1<<1)  ///< error decoding wheel signal - values may be incorrect
} WCError;


#endif
