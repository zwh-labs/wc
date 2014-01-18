#ifndef _WC_ERROR_H_
#define _WC_ERROR_H_


enum wcErrorTypes
{
	WC_ERROR_NOERROR     =      0, ///< no error detected
	WC_ERROR_UNKNOWN     = (1<<0), ///< unknown error
	WC_ERROR_WHEELSIGNAL = (1<<1)  ///< error decoding wheel signal - values may be incorrect
};

typedef unsigned char wcError;


#endif
