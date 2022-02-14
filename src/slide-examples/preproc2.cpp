#define MY_PI 3.1415926			//!< MY_PI is replaced by 3.1415926
#define SQUARE(a) ((a)*(a))		//!< macro: replace before compilation

#ifdef GTCMT_WIN32				// check if GTCMT_WIN32 is defined
	// do this 
	// for Win32 compiles
#else
	// do something else 
	// in all other cases
#endif //GTCMT_WIN32
