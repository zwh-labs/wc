namespace WC
{

public enum Error : byte
{
	NoError     =      0, ///< no error detected
	Unknown     = (1<<0), ///< unknown error
	WheelSignal = (1<<1)  ///< error decoding wheel signal - values may be incorrect
};

}
