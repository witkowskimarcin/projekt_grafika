#include <iostream>
#include "OBJ.h"

int main()
{
	OBJ * A = new OBJ("ludzik.obj",1);

	std::cout << "Koniec";
	std::cin.get();
	return 0;
}