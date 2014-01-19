#ifndef _WC_WHEELMOVEMENT_CONFIGURATION_H_
#define _WC_WHEELMOVEMENT_CONFIGURATION_H_


#include "wc_export.h"
#include <wc/WheelMovement.h>
#include <wc/Configuration.h>


#ifdef __cplusplus
extern "C" {
#endif

WC_EXPORT double wcWheelMovement_getTurns( const wcWheelMovement * wm, const wcConfiguration * c );

#ifdef __cplusplus
}
#endif


#endif
