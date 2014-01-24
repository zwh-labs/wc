using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;


namespace WC
{

public class WheelPIDController
{
	[DllImport("libwc")] private static extern IntPtr wcWheelPIDController_new();
	[DllImport("libwc")] private static extern bool wcWheelPIDController_delete( IntPtr wheelPIDController );

	[DllImport("libwc")] private static extern void wcWheelPIDController_reset( IntPtr wheelPIDController );

	[DllImport("libwc")] private static extern double wcWheelPIDController_getProportionalGain( IntPtr wheelPIDController );
	[DllImport("libwc")] private static extern double wcWheelPIDController_getIntegralGain( IntPtr wheelPIDController );
	[DllImport("libwc")] private static extern double wcWheelPIDController_getDerivativeGain( IntPtr wheelPIDController );
	[DllImport("libwc")] private static extern double wcWheelPIDController_getWindupGuard( IntPtr wheelPIDController );

	[DllImport("libwc")] private static extern void wcWheelPIDController_setProportionalGain( IntPtr wheelPIDController, double gain );
	[DllImport("libwc")] private static extern void wcWheelPIDController_setIntegralGain( IntPtr wheelPIDController, double gain );
	[DllImport("libwc")] private static extern void wcWheelPIDController_setDerivativeGain( IntPtr wheelPIDController, double gain );
	[DllImport("libwc")] private static extern void wcWheelPIDController_setWindupGuard( IntPtr wheelPIDController, double guard );

	[DllImport("libwc")] private static extern void wcWheelPIDController_update( IntPtr wheelPIDController, int targetAngleIncrements, int actualAngleIncrements, double delta );

	internal IntPtr handle;

	public WheelPIDController() { handle = wcWheelPIDController_new(); }
	~WheelPIDController() { wcWheelPIDController_delete( handle ); }

	public double proportionalGain
	{
		get { return wcWheelPIDController_getProportionalGain(); }
		set { wcWheelPIDController_setProportionalGain( handle, value ); }
	}

	public double integralGain
	{
		get { return wcWheelPIDController_getIntegralGain(); }
		set { wcWheelPIDController_setIntegralGain( handle, value ); }
	}

	public double derivativeGain
	{
		get { return wcWheelPIDController_getDerivativeGain(); }
		set { wcWheelPIDController_setDerivativeGain( handle, value ); }
	}

	public double windupGuard
	{
		get { return wcWheelPIDController_getWindupGuard(); }
		set { wcWheelPIDController_setWindupGuard( handle, value ); }
	}

	public void reset()
	{
		wcWheelPIDController_reset( handle );
	}

	public double update( int targetAngleIncrements, int actualAngleIncrements, double delta )
	{
		return wcWheelPIDController_update( handle, targetAngleIncrements, actualAngleIncrements, delta );
	}
}

}
