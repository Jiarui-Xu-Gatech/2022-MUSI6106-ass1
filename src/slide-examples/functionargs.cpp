//!< pass by value
void func1 (int a)
{
    a   = 2;
}
//!< pass by value
void func2 (int *a)
{
    a   = new int;
}
//!< pass by pointer
void func3 (int *a)
{
    *a  = 2;
}
//!< pass by reference
void func4 (int &a)
{
    a	= 2;
}
