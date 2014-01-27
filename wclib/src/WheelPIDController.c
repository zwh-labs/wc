#include <wc/WheelPIDController.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <float.h>

#include <stdio.h>


struct _wcWheelPIDController
{
	const wcConfiguration * configuration;
	unsigned int wheelIndex;
	int previousError;
	double integratedError;

	double windupGuard;
	double proportionalGain;
	double integralGain;
	double derivativeGain;
};


wcWheelPIDController * wcWheelPIDController_new( const wcConfiguration * configuration, unsigned int wheelIndex )
{
	wcWheelPIDController * controller = (wcWheelPIDController*) malloc( sizeof(wcWheelPIDController) );

	controller->configuration = configuration;
	controller->wheelIndex = wheelIndex;
	controller->previousError = 0;
	controller->integratedError = 0;

	controller->windupGuard = FLT_MAX;
	controller->proportionalGain = 0.0001;
	controller->integralGain =     0.00001;
	controller->derivativeGain =   0.000001;

	return controller;
}


bool wcWheelPIDController_delete( wcWheelPIDController * controller )
{
	if( !controller )
		return false;
	free( controller );
	return true;
}


void wcWheelPIDController_reset( wcWheelPIDController * controller )
{
	controller->previousError = 0;
	controller->integratedError = 0;
}


void wcWheelPIDController_setProportionalGain( wcWheelPIDController * controller, double gain )
{
	controller->proportionalGain = gain;
}


void wcWheelPIDController_setIntegralGain( wcWheelPIDController * controller, double gain )
{
	controller->integralGain = gain;
}


void wcWheelPIDController_setDerivativeGain( wcWheelPIDController * controller, double gain )
{
	controller->derivativeGain = gain;
}


void wcWheelPIDController_setWindupGuard( wcWheelPIDController * controller, double guard )
{
	controller->windupGuard = guard;
}


double wcWheelPIDController_getProportionalGain( const wcWheelPIDController * controller )
{
	return controller->proportionalGain;
}


double wcWheelPIDController_getIntegralGain( const wcWheelPIDController * controller )
{
	return controller->integralGain;
}


double wcWheelPIDController_getDerivativeGain( const wcWheelPIDController * controller )
{
	return controller->derivativeGain;
}


double wcWheelPIDController_getWindupGuard( const wcWheelPIDController * controller )
{
	return controller->windupGuard;
}


// http://stackoverflow.com/questions/12089514/real-modulo-operator-in-c-c
static inline int modulo( int a, int b )
{
	const int result = a % b;
	return result >= 0 ? result : result + b;
}


// http://nicisdigital.wordpress.com/2011/06/27/proportional-integral-derivative-pid-controller/
double wcWheelPIDController_update( wcWheelPIDController * controller, int targetAngleIncrements, int actualAngleIncrements, double delta )
{
	int incrementsPerTurn = wcConfiguration_getWheelIncrementsPerTurn( controller->configuration, controller->wheelIndex );

	// the shortest distance between both angles
	int currentError = targetAngleIncrements - actualAngleIncrements;
	int halfIncrementsPerTurn = wcConfiguration_getWheelIncrementsPerTurn( controller->configuration, controller->wheelIndex ) >> 1;
	currentError = modulo( (currentError + halfIncrementsPerTurn), incrementsPerTurn ) - halfIncrementsPerTurn;

	// integration with windup guarding
	controller->integratedError += (double)currentError * delta;
	if( controller->integratedError < -(controller->windupGuard) )
		controller->integratedError = -(controller->windupGuard);
	else if( controller->integratedError > controller->windupGuard )
		controller->integratedError = controller->windupGuard;

	// differentiation
	double diff = 0.0;
	if( delta > FLT_EPSILON )
		diff = (currentError - controller->previousError) / delta;

	// scaling
	double p_term = controller->proportionalGain * currentError;
	double i_term = controller->integralGain     * controller->integratedError;
	double d_term = controller->derivativeGain   * diff;

	// summation of terms
	double control = p_term + i_term + d_term;

	// save current error as previous error for next iteration
	controller->previousError = currentError;

	return control;
}
