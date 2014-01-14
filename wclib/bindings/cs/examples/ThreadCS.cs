using System;
using System.Threading;


public class ThreadCS
{
	static public void Main()
	{
		WCConfiguration configuration = new WCConfiguration();
//		configuration.setDevicePath("COM3");
		configuration.setDevicePath("/dev/pts/4");
		configuration.setWheel( 0, 4096 );
		configuration.setWheel( 1, 4096 );
		configuration.print();

		WCConnection connection = new WCConnection( configuration.getDevicePath() );
		WCThread thread = new WCThread( connection );

		Console.WriteLine("Running for 10 seconds");
		int cnt = 0;
		while( cnt < 10 )
		{
			Thread.Sleep( 1000 );
			for( uint i = 0; i < thread.getWheelCount(); i++ )
			{
				WCWheelMovement wm = thread.retrieveWheelMovement( i );
				Console.WriteLine("Wheel:\tchannel=" + i + "\terror=" + wm.error + "\tvalue=" + wm.increments + "\tturn=" + wm.getIncrementsAsTurns( configuration ) );
			}
			cnt++;
		}
	}
}
