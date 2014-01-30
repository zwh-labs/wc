using System;
using System.Runtime.InteropServices;


namespace WC
{

public static class WheelMovement_Configuration
{
	[DllImport("libwc")] private static extern double wcWheelMovement_getTurns( ref WheelMovement wheelMovement, IntPtr configuration );

	public static double getTurns( this WheelMovement wheelMovement, Configuration configuration )
	{
		return wcWheelMovement_getTurns( ref wheelMovement, configuration.handle );
	}
}

}
