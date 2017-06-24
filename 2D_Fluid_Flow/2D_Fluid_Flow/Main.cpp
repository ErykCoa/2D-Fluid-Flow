#include "SFML/Graphics.hpp"
#include "PipeMap.h"
#include <memory>
#include <iostream>

#include "stde.h"


int main()
{
	try {
		std::unique_ptr<PipeMap> Map{ new PipeMap("example") };

		while (!Map->WindowHasBeenClosed())
		{
			Map->SingleTick();
		}


		return 0;
	}
	catch (std::exception & E)
	{
		std::cerr << std::endl << E.what();
	}
}
