using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;


namespace WC
{

public class Connection
{
	[DllImport("libwc")] private static extern IntPtr wcConnection_open( IntPtr file );
	[DllImport("libwc")] private static extern bool wcConnection_close( IntPtr connection );

	internal IntPtr handle;

	public Connection( string file )
	{
		handle = wcConnection_open( Marshal.StringToHGlobalAnsi(file) );
		if( handle == IntPtr.Zero )
			throw new FileNotFoundException( "Failed to open connection to \"" + file + "\"" );
	}

	public Connection( Configuration configuration ) : this( configuration.devicePath ) {}

	~Connection() { wcConnection_close( handle ); }
}

}
