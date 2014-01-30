using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;


namespace WC
{

public class Configuration
{
	[DllImport("libwc")] private static extern IntPtr wcConfiguration_new();
	[DllImport("libwc")] private static extern bool wcConfiguration_delete( IntPtr configuration );

	[DllImport("libwc")] private static extern IntPtr wcConfiguration_getDevicePath( IntPtr configuration );
	[DllImport("libwc")] private static extern uint wcConfiguration_getWheelCount( IntPtr configuration );
	[DllImport("libwc")] private static extern uint wcConfiguration_getWheelIncrementsPerTurn( IntPtr configuration, uint wheelIndex );
	[DllImport("libwc")] private static extern void wcConfiguration_setDevicePath( IntPtr configuration, IntPtr devicePath );
	[DllImport("libwc")] private static extern bool wcConfiguration_setWheel( IntPtr configuration, uint wheelIndex, uint incrementsPerTurn );

	[DllImport("libwc")] private static extern int wcConfiguration_snprint( IntPtr s, UIntPtr n, IntPtr configuration );

	internal IntPtr handle;

	public Configuration() { handle = wcConfiguration_new(); }
	~Configuration() { wcConfiguration_delete( handle ); }

	public string devicePath
	{
		get { return Marshal.PtrToStringAnsi( wcConfiguration_getDevicePath( handle ) ); }
		set { wcConfiguration_setDevicePath( handle, Marshal.StringToHGlobalAnsi( value ) ); }
	}
	public uint wheelCount
	{
		get { return wcConfiguration_getWheelCount( handle ); }
	}

	public uint getWheelIncrementsPerTurn( uint wheelIndex ) { return wcConfiguration_getWheelIncrementsPerTurn( handle, wheelIndex ); }
	public bool setWheel( uint wheelIndex, uint incrementsPerTurn ) { return wcConfiguration_setWheel( handle, wheelIndex, incrementsPerTurn ); }

	public override string ToString()
	{
		int size = wcConfiguration_snprint( IntPtr.Zero, UIntPtr.Zero, handle );
		if( size < 0 )
			throw new Exception();
		size += 1;	// + 1 for terminating '\0'
		IntPtr buf = Marshal.AllocHGlobal( size );
		wcConfiguration_snprint( buf, (UIntPtr)size, handle );
		string ret = Marshal.PtrToStringAnsi( buf );
		Marshal.FreeHGlobal( buf );
		return ret;
	}
}

}
