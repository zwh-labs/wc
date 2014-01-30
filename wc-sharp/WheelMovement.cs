using System;
using System.Runtime.InteropServices;


namespace WC
{

[StructLayout(LayoutKind.Sequential)]
public struct WheelMovement
{
	public uint index { get; private set; }
	public int increments { get; private set; }
	public Error error { get; private set; }

	public WheelMovement( uint index ) : this() { this.index = index; }
	public void reset() { this.increments = 0; this.error = Error.NoError; }
	public void accumulate( int increment, Error error ) { this.increments += increment; this.error |= error; }
}

}
