#include <iostream>

using namespace std;

void printText (char * str)
{
    cout << str << endl;
}

int main ()
{
    const char *pText = "Text"; // do not use this old school char...

    printText (pText); // compiler error

    printText (const_cast<char*>(pText)); // this is dangerous
    
    return 0;
}
