#pragma once
#include "SFML/Graphics.hpp"
#include "curses.h"
#include <memory>
#include <future>
#include <mutex>
#include <atomic>
#include <vector>
#include "Consts.h"
#include <iostream>
#include <thread>


class ConsoleIO
{
public:
	std::vector<std::string> PopInputLines();

	void Write(std::string & str);

	ConsoleIO();
	~ConsoleIO();
private:
	WINDOW* Console;

	std::thread InputTh;

	std::vector<std::string> OutputLines;
	unsigned OutputLinesStart;

	void RefreshConsole();
	void WriteInputBuffer();
	void StartInputThread();

	std::mutex Lock;
	std::atomic<bool> ConsoleHasBeenClosed;

	std::vector<std::string> AwaitningInputLines;
	std::string InputBuffer;
};

ConsoleIO & operator<<(ConsoleIO & Console, std::string & str);
ConsoleIO & operator<<(ConsoleIO & Console, const char * str);

