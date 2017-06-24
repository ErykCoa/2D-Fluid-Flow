#pragma once

#include <future>
#include <numeric>
#include <array>
#include <map>
#include <regex>
#include <fstream>
#include "Consts.h"
#include "StructsAndEnums.h"

namespace stde
{
	template<unsigned NumberOfThreads, class Function, class Iterator>
	int For_each(Iterator Begin, Iterator End, Function Function)
	{
		std::array<std::future<int>, NumberOfThreads> handle;

		auto Length = std::distance(Begin, End) / NumberOfThreads;

		for (unsigned Index = 0; Index < NumberOfThreads; ++Index)
		{
			handle[Index] = std::async([&](unsigned Number) {

				auto LocalBegin = Begin + std::distance(Begin, End) * Number / NumberOfThreads;

				for (auto Iter = LocalBegin; std::distance(LocalBegin, Iter) < Length; ++Iter)
				{
					Function(Iter);
				}
				return 0;

			}, Index);
		}

		return
			std::accumulate(handle.begin(), handle.end(), 0, [&](int a, std::future<int> & b) {
			return a + b.get();
		});
	}

	template<typename T, typename T2, typename T3>
	inline auto Trim(T What, T2 LowerBoundary, T3 UpperBoundary)
	{
		return What < LowerBoundary ? LowerBoundary : (What > UpperBoundary ? UpperBoundary : What);
	}

	template<typename T, typename T2>
	inline auto Trim(sf::Vector2<T> & What, Boundaries<T2> & Boundaries)
	{
		return std::move(sf::Vector2<T>{Trim(What.x, Boundaries.LowerX, Boundaries.UpperX), Trim(What.y, Boundaries.LowerY, Boundaries.UpperY)});
	}

	template<class T>
	std::map<std::string, T> LoadDictionary(const std::string & FromFile)
	{	
		std::map<std::string, T> Res;

		std::ifstream Input{ FromFile };
		if (Input.is_open())
		{
			std::string Key;
			T Value;
			char CurrentSeparator;

			unsigned LineCounter{};

			try {
				for (;!Input.eof(); ++LineCounter)
				{
					Input >> Key >> CurrentSeparator >> Value;

					if (CurrentSeparator != DictionarySeparator)
						throw std::ios_base::failure("");

					if (Res.find(Key) != Res.end())
						throw std::runtime_error("Key repetition in file: " + FromFile + " at line: " + std::to_string(LineCounter));

					Res.emplace(Key, Value);
				}
			}
			catch (std::ios_base::failure & E)
			{
				throw std::ios_base::failure("Line: " + std::to_string(LineCounter) + " of file: " + FromFile + " is corrupted!");
			}
					
			Input.close();
		}
		else
			throw std::runtime_error("Failed to open file: " + FromFile);

		return std::move(Res);
	}

	template<class T>
	std::vector<T> LoadVector(const std::string & FromFile)
	{
		std::vector<T> Res;

		std::ifstream Input{ FromFile };
		if (Input.is_open())
		{
			T Value;

			unsigned LineCounter{};

			try {
				for (; !Input.eof(); ++LineCounter)
				{
					Input >> Value;

					Res.push_back(Value);
				}
			}
			catch (std::ios_base::failure & E)
			{
				throw std::ios_base::failure("Line: " + std::to_string(LineCounter) + " of file: " + FromFile + " is corrupted!");
			}

			Input.close();
		}
		else
			throw std::runtime_error("Failed to open file: " + FromFile);

		return std::move(Res);
	}

	template<class T>
	T GetValue(std::map<std::string, T> & From, std::string & Key)
	{
		try {
			return From.at(Key);
		}
		catch (std::out_of_range & E)
		{
			throw std::runtime_error("No key \"" + Key + "\" has been found!");
		}		
	}

	template<class T>
	T GetValue(std::map<std::string, T> & From, const char* Key)
	{
		try {
			return From.at(Key);
		}
		catch (std::out_of_range & E)
		{
			throw std::runtime_error("No key \"" + std::string(Key) + "\" has been found!");
		}
	}

}


/*
namespace stde
{
template<unsigned NumberOfThreads, class Function, class Iterator>
int For_each(Iterator Begin, Iterator End, Function Function)
{
std::array<std::future<int>, NumberOfThreads> handle;

auto Lenght = std::distance(Begin, End);

for (unsigned Index = 0; Index < NumberOfThreads; ++Index)
{
handle[Index] = std::async([&](unsigned Number) {


for (auto Iter = Begin * ((NumberOfThreads - Number) / NumberOfThreads); std::distance(Begin, Iter) < Lenght; )
{
Function(Iter);
#ifdef NDEBUG
Iter += NumberOfThreads;
#else
for (int i = 0; i < NumberOfThreads && std::distance(Begin, Iter) < Lenght; ++i)
{
++Iter;
}
#endif
}
return 0;

}, Index);
}

return
std::accumulate(handle.begin(), handle.end(), 0, [&](int a, std::future<int> & b) {
return a + b.get();
});
}
}





*/