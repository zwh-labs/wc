using System;
using System.Runtime.InteropServices;


public static class WheelMovement_Configuration
{
	[DllImport("libwc")] private static extern double WCWheelMovement_getIncrementsAsTurns( ref WCWheelMovement wheelMovement, IntPtr configuration );

	public static double getIncrementsAsTurns( this WCWheelMovement wheelMovement, WCConfiguration configuration )
	{
		return WCWheelMovement_getIncrementsAsTurns( ref wheelMovement, configuration.handle );
	}
}
