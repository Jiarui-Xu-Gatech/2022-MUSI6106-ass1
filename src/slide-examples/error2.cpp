int main ()
{
    ErrorCode_t r = kNoErr;
    int 	*pPtr = 0;
    
	r = func1(pPtr);
    if (r != kNoErr)
        std::cout << "func1 return code not 0!" << endl;
    
	try
    {
        func2(pPtr);
    }
    catch (const char* msg) 
    {
        std::cout << msg << endl;
    }
    
	func3(pPtr);  

    return 0;
}
