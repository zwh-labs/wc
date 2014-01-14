using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;


[assembly: InternalsVisibleTo("WCThread")]


public class WCConnection
{
	internal IntPtr handle;

	[DllImport("libwc")] private static extern IntPtr WCConnection_open( IntPtr file );
	[DllImport("libwc")] private static extern bool WCConnection_close( IntPtr connection );

	public WCConnection( string file )
	{
		handle = WCConnection_open( Marshal.StringToHGlobalAnsi(file) );
		if( handle == IntPtr.Zero )
			throw new FileNotFoundException( "Failed to open connection to \"" + file + "\"" );
	}
	~WCConnection() { WCConnection_close( handle ); }
}
