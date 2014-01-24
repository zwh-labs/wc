#ifndef _WC_WHEELPIDCONTROLLER_H_
#define _WC_WHEELPIDCONTROLLER_H_


#include "wc_export.h"
#include <wc/Configuration.h>
#include <stdbool.h>


typedef struct _wcWheelPIDController wcWheelPIDController;


#ifdef __cplusplus
extern "C" {
#endif

WC_EXPORT wcWheelPIDController * wcWheelPIDController_new( const wcConfiguration * configuration, unsigned int wheelIndex );
WC_EXPORT bool wcWheelPIDController_delete( wcWheelPIDController * controller );

WC_EXPORT void wcWheelPIDController_reset( wcWheelPIDController * controller );

WC_EXPORT void wcWheelPIDController_setProportionalGain( wcWheelPIDController * controller, double gain );
WC_EXPORT void wcWheelPIDController_setIntegralGain( wcWheelPIDController * controller, double gain );
WC_EXPORT void wcWheelPIDController_setDerivativeGain( wcWheelPIDController * controller, double gain );
WC_EXPORT void wcWheelPIDController_setWindupGuard( wcWheelPIDController * controller, double guard );

WC_EXPORT double wcWheelPIDController_getProportionalGain( const wcWheelPIDController * controller );
WC_EXPORT double wcWheelPIDController_getIntegralGain( const wcWheelPIDController * controller );
WC_EXPORT double wcWheelPIDController_getDerivativeGain( const wcWheelPIDController * controller );
WC_EXPORT double wcWheelPIDController_getWindupGuard( const wcWheelPIDController * controller );

WC_EXPORT double wcWheelPIDController_update( wcWheelPIDController * controller, int targetAngleIncrements, int actualAngleIncrements, double delta );

#ifdef __cplusplus
}
#endif


#endif
