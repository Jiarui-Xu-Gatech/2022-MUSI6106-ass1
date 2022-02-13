#include <iostream> 				//!< preprocessor directive

using namespace std; 				//!< compiler directive
const float kPi = 3.1415926F; 		//!< definition of a constant

int main(void) 						//!< function returns integer
{
	float fX = kPi,		 			//!< definitions of variables
	fY = 1, 
	fZ;								//!< uninitialized variable
	cout << "Hello world!" << endl; //!< output
	fZ = fX + fY;    				//!< assignment statement
	
	cout << "Y is: " << fY << endl;
	cout << "Z is: " << fZ << endl;
	
	return 0; 						//!< return statement
} 									//!< end of the function block