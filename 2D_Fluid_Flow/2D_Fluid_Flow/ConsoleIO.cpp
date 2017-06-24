#include "ConsoleIO.h"

std::vector<std::string> ConsoleIO::PopInputLines()
{
	std::lock_guard<std::mutex> _lock{ Lock };

	std::vector<std::string> Res{};

	AwaitningInputLines.swap(Res);

	return std::move(Res);
}

void ConsoleIO::Write(std::string & str)
{
	std::lock_guard<std::mutex> _lock{ Lock };

	for (auto& c : str)
	{
		if (c == '\n')
		{
			OutputLines.push_back({});

			if (OutputLines.size() - OutputLinesStart > ConsoleY)
				++OutputLinesStart;
		}
		else
			OutputLines.back().push_back(c);
	}

	RefreshConsole();
}

ConsoleIO::ConsoleIO() :
	Console(initscr()),
	ConsoleHasBeenClosed(false),
	OutputLinesStart(0),
	OutputLines{ {} }
{
	resize_term(ConsoleY, ConsoleX);

	noecho();
	curs_set(false);

	keypad(Console, true);
	nodelay(stdscr, true);

	RefreshConsole();

	StartInputThread();
}

ConsoleIO::~ConsoleIO()
{
	ConsoleHasBeenClosed.store(true);
	
	InputTh.join();

	endwin();
}

void ConsoleIO::RefreshConsole()
{
	erase();

	for (unsigned Line = OutputLinesStart; Line < OutputLines.size() && Line - OutputLinesStart < ConsoleY - 1; ++Line)
		mvaddnstr(Line - OutputLinesStart, 0, OutputLines[Line].c_str(), ConsoleX);

	WriteInputBuffer();

	refresh();
}

void ConsoleIO::WriteInputBuffer()
{
	mvaddstr(ConsoleY - 1, 0, ">> ");
	mvaddnstr(ConsoleY -1, ConsoleInputOffset, InputBuffer.c_str(), ConsoleX);
}

void ConsoleIO::StartInputThread()
{
	InputTh = std::move(std::thread{
		[&] {
		while (!ConsoleHasBeenClosed.load())
		{
			Lock.lock();

			int Input = getch();

			if (Input == ERR)
			{
				Lock.unlock();
				sf::sleep(sf::milliseconds(1));
				continue;
			}

			switch (Input)
			{
			case '\n':
				AwaitningInputLines.push_back(InputBuffer);
				for (int i = InputBuffer.size() + ConsoleInputOffset; i--;) mvaddch(ConsoleY - 1, i, ' ');
				InputBuffer.clear();
				break;
			case '\b':
				if (!InputBuffer.empty())
				{
					InputBuffer.pop_back();
					mvaddch(ConsoleY - 1, InputBuffer.size() + ConsoleInputOffset, ' ');
				}
				break;
			case KEY_UP:
				if (OutputLinesStart)
					--OutputLinesStart;
				break;
			case KEY_DOWN:
				if (OutputLines.size() - OutputLinesStart > ConsoleY)
					++OutputLinesStart;
				break;
			default:
				InputBuffer += static_cast<char>(Input); break;
			}

			RefreshConsole();

			Lock.unlock();
		}
	} });
}

ConsoleIO & operator<<(ConsoleIO & Console, std::string & str)
{
	Console.Write(str);

	return Console;
}

ConsoleIO & operator<<(ConsoleIO & Console, const char * str)
{
	Console.Write(std::string(str));

	return Console;
}
