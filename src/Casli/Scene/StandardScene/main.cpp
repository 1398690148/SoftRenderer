#include "App.h"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc > 1)
		return App{ argv[1] }.Go();
	else
	{
		std::cerr << "Can't Found Input File, start Defaut Scene" << std::endl;
		return App{}.Go();
		return 0;
	}
}