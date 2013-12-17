#ifdef _WIN32 // if compiling for Windows
	// This value is also defined by the 64-bit compiler for backward compatibility.
	// http://msdn.microsoft.com/en-us/library/ff540443.aspx
	#include "WCConnection_Windows.c"
#else // if compiling for anything non-Windows
	#include "WCConnection_Unix.c"
#endif
