#include <iostream>

using namespace std;

int main ()
{
    int  iVar = 20;             //!< variable declaration.
    int  *piPointer;            //!< pointer variable 
    int  aiArray[3] = {3,2,1};  //!< array variable

    piPointer   = &iVar;        // store address of var in pointer variable

    cout << "Value of iVar variable: " << iVar << endl;

    cout << "Address stored in piPointer variable: " << piPointer << endl;

    cout << "Value of *piPointer variable: " << *piPointer << endl;

    *piPointer	= *piPointer + 1;
    cout << "Value of iVar variable: " << iVar << endl;

    piPointer   = &aiArray[1];
    for (int i = 0; i < 2; i++)
    {
        cout << "Value of aiArray[" << i << "]: " << aiArray[i] << endl;
        cout << "Value of piPointer[" << i << "]: " << piPointer[i] << endl;
    }

    return 0;
}
