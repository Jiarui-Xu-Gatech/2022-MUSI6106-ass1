/*! A test class */
class Test
{
public:
	/** An enum type.
	* The documentation block cannot be put after the enum!
	*/
	enum EnumType
	{
		int EVal1, /**< enum value 1 */
		int EVal2 /**< enum value 2 */
	};
	
	/*!
	\sa testMe()
	\param c1 the first argument.
	\param c2 the second argument.
	*/
	virtual void testMeToo(char c1,char c2) = 0;
private:
	int value; //!< Detailed description after the member
};
		 