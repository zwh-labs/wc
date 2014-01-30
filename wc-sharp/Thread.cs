using System;
using System.Runtime.InteropServices;


namespace WC
{

public class Thread
{
	[DllImport("libwc")] private static extern IntPtr wcThread_start( IntPtr connection );
	[DllImport("libwc")] private static extern bool wcThread_stop( IntPtr thread );

	[DllImport("libwc")] private static extern WheelMovement wcThread_retrieveWheelMovement( IntPtr thread, uint index );
	[DllImport("libwc")] private static extern uint wcThread_getWheelCount( IntPtr thread );

	internal IntPtr handle;

	public uint wheelCount
	{
		get { return wcThread_getWheelCount( handle ); }
	}

	public Thread( Connection connection ) { handle = wcThread_start( connection.handle ); }
	~Thread() { wcThread_stop( handle ); }

	public WheelMovement retrieveWheelMovement( uint index ) { return wcThread_retrieveWheelMovement( handle, index ); }
}

}
