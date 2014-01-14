using System;
using System.Runtime.InteropServices;


public class WCThread
{
	internal IntPtr handle;


	[DllImport("libwc")] private static extern IntPtr WCThread_start( IntPtr connection );
	[DllImport("libwc")] private static extern bool WCThread_stop( IntPtr thread );

	[DllImport("libwc")] private static extern WCWheelMovement WCThread_retrieveWheelMovement( IntPtr thread, uint index );
	[DllImport("libwc")] private static extern uint WCThread_getWheelCount( IntPtr thread );


	public WCThread( WCConnection connection ) { handle = WCThread_start( connection.handle ); }
	~WCThread() { WCThread_stop( handle ); }

	public WCWheelMovement retrieveWheelMovement( uint index ) { return WCThread_retrieveWheelMovement( handle, index ); }
	public uint getWheelCount() { return WCThread_getWheelCount( handle ); }
}
