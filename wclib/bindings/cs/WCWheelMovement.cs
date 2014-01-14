using System;
using System.Runtime.InteropServices;


[StructLayout(LayoutKind.Sequential)]
public struct WCWheelMovement
{
	public uint index { get; private set; }
	public int increments { get; private set; }
	public WCError error { get; private set; }

	public WCWheelMovement( uint index ) : this() { this.index = index; }
	public void reset() { this.increments = 0; this.error = WCError.NoError; }
	public void accumulate( int increment, WCError error ) { this.increments += increment; this.error |= error; }
}
