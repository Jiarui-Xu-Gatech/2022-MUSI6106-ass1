//******************************************
//functions
float doSomething(int iParam);	//!< function declaration

float doSomething(int iParam)	//!< function definition
{
	//******************************************
	//variables
	fSumOfSquares = 0;			//!< error: undeclared identifier

	//******************************************
	float fSumOfSquares;		//!< declaration \& definition
	fSumOfSquares = 0;			//!< no error

	return iParam * 3.14F;
}
