int main ()
{
    int a = 1;
    func1(a);	// a remains 1
    func2(&a);  // a remains 1, memory leakage
    func3(&a);  // a is 2
    a = 1;
    func4(a);   // a is 2
    return 0;
}
