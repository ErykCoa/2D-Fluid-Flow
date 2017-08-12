#include "SFML/Graphics.hpp"
#include "PipeMap.h"
#include <memory>
#include <iostream>

#include "stde.h"


int main()
{
	try {
		std::unique_ptr<PipeMap> Map{ nullptr };

		do {
			Map.reset(nullptr);

			Map = PipeMap::GetPipeMapFromUserInput();

			if (Map != nullptr)
			{
				Map->StartSimulation();

				while (Map->GetMapState() == PipeMapState::Running)
				{
					Map->SingleTick();
				}
			}

		} while (Map != nullptr);
	}
	catch (std::exception & E)
	{
		std::cerr << std::endl << E.what();
	}


	return 0;
}
