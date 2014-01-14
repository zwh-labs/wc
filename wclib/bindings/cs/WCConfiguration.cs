using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;


[assembly: InternalsVisibleTo("WCWheelMovement_Configuration")]


public class WCConfiguration
{
	internal IntPtr handle;


	[DllImport("libwc")] private static extern IntPtr WCConfiguration_new();
	[DllImport("libwc")] private static extern bool WCConfiguration_delete( IntPtr configuration );

	[DllImport("libwc")] private static extern IntPtr WCConfiguration_getDevicePath( IntPtr configuration );
	[DllImport("libwc")] private static extern uint WCConfiguration_getWheelCount( IntPtr configuration );
	[DllImport("libwc")] private static extern uint WCConfiguration_getWheelIncrementsPerTurn( IntPtr configuration, uint wheelIndex );
	[DllImport("libwc")] private static extern void WCConfiguration_setDevicePath( IntPtr configuration, IntPtr devicePath );
	[DllImport("libwc")] private static extern bool WCConfiguration_setWheel( IntPtr configuration, uint wheelIndex, uint incrementsPerTurn );

	[DllImport("libwc")] private static extern int WCConfiguration_print( IntPtr configuration );


	public WCConfiguration() { handle = WCConfiguration_new(); }
	~WCConfiguration() { WCConfiguration_delete( handle ); }

	public string getDevicePath() { return Marshal.PtrToStringAnsi( WCConfiguration_getDevicePath( handle ) ); }
	public uint getWheelCount() { return WCConfiguration_getWheelCount( handle ); }
	public uint getWheelIncrementsPerTurn( uint wheelIndex ) { return WCConfiguration_getWheelIncrementsPerTurn( handle, wheelIndex ); }

	public void setDevicePath( string devicePath ) { WCConfiguration_setDevicePath( handle, Marshal.StringToHGlobalAnsi( devicePath ) ); }
	public bool setWheel( uint wheelIndex, uint incrementsPerTurn ) { return WCConfiguration_setWheel( handle, wheelIndex, incrementsPerTurn ); }

	public int print() { return WCConfiguration_print( handle ); }
}
