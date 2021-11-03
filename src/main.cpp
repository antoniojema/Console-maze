#include <iostream>
#include <ctime>
#include <windows.h>
#include "maze.h"

int main()
{
	// Time stuff
	clock_t t0 = clock();
	clock_t t1 = clock();
	double spf = 1.f/30.f;

	// Parameters
	int screen_width = 120;
	int screen_height = 40;
	float px_init = 0.5f;
	float py_init = 0.5f;
	float mv_per_secs = 0.1f/spf;
	float turn_per_secs = 0.1f/spf;
	float real_spf = 0.f;

	std::cout << "Pene" << std::endl;

	// Create Screen Buffer
	//wchar_t* screen = new wchar_t[screen_width * screen_height];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Maze
	std::wstring maze_str = L"";
	maze_str += L"..#############";
	maze_str += L"..#...........#";
	maze_str += L"#.#.####......#";
	maze_str += L"#.###.#.......#";
	maze_str += L"#.............#";
	maze_str += L"#.............#";
	maze_str += L"#.............#";
	maze_str += L"#.............#";
	maze_str += L"#.............#";
	maze_str += L"#.............#";
	maze_str += L"#.............#";
	maze_str += L"#.............#";
	maze_str += L"#.............#";
	maze_str += L"#..............";
	maze_str += L"#############..";
	int maze_width = 15;
	int maze_height = 15;


	Maze M;
	M.set(maze_str, maze_width, maze_height, px_init, py_init, screen_width, screen_height);

	int frame = 0;
	while(true)
	{

		// Clean scene and show maze
		frame++;

		if (GetKeyState('W') & 0x8000)
		{
			M.player_forward(mv_per_secs * real_spf);
		}
		else if (GetKeyState('S') & 0x8000)
		{
			M.player_forward(-mv_per_secs * real_spf);
		}
		if (GetKeyState('D') & 0x8000)
		{
			M.player_turn(turn_per_secs * real_spf);
		}
		else if (GetKeyState('A') & 0x8000)
		{
			M.player_turn(-turn_per_secs * real_spf);
		}

		// Draw
		wchar_t* view = M.get_player_view();

		// Display
		view[screen_width * screen_height - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, view, screen_width * screen_height, { 0,0 }, &dwBytesWritten);
		delete [] view;

		// Wait for frame to end
		//do
		//{
		//	t1 = clock();
		//} while ((float(t1) - float(t0)) / CLOCKS_PER_SEC < spf);
		//t0 = clock();
		real_spf = ((float)clock() - t0) / CLOCKS_PER_SEC;
		t0 = clock();
	}

	return 0;
}
