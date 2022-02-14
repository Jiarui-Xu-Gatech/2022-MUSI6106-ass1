void my_function(A* my_a)
{
	B* my_b = dynamic_cast<B*>(my_a);
 
	if (my_b != NULL)
		my_b->methodSpecificToB();
	else
		std::cerr << "This object is not of type B\n";
}