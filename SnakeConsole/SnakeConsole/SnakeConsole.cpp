// SnakeConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<Windows.h>
#include <iostream>
#include <list>
#include <thread>

using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 30;

struct sSnakeSegment
{
	int x;
	int y;
};
int main()
{
	//Screen Buffer

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;


	while (1) {
		//Timing and Input
		list<sSnakeSegment> snake = { {60,15},{61,15},{62,15},{63,15},{64,15},{65,15},{66,15},{67,15},{68,15},{69,15} };
		int nFoodX = 30;
		int nFoodY = 15;
		int nScore = 0;
		int nSnakeDirection = 3;
		bool bDead = false;
		bool bKeyLeft = false, bKeyRight = false, bKeyLeftOld = false, bKeyRightOld = false;

		while (!bDead) {
			auto t1 = chrono::system_clock::now();
			while ((chrono::system_clock::now() - t1) < ((nSnakeDirection % 2 == 1) ? 60ms : 100ms))
			{
				bKeyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
				bKeyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;

				if (bKeyRight && !bKeyRightOld) {
					nSnakeDirection++;
					if (nSnakeDirection == 4) nSnakeDirection = 0;
				}
				if (bKeyLeft && !bKeyLeftOld) {
					nSnakeDirection--;
					if (nSnakeDirection == -1) nSnakeDirection = 3;
				}
				bKeyRightOld = bKeyRight;
				bKeyLeftOld = bKeyLeft;
			}
		
		// Game Logic
		switch (nSnakeDirection) // mover a snake
		{
		case 0: // UP
			snake.push_front({ snake.front().x, snake.front().y - 1 });
			break;
		case 1: //right
			snake.push_front({ snake.front().x + 1, snake.front().y });
			break;
		case 2: //down
			snake.push_front({ snake.front().x, snake.front().y + 1 });
			break;
		case 3://left
			snake.push_front({ snake.front().x - 1, snake.front().y });
			break;
		}

		// Collision Detect @ Food
		if (snake.front().x == nFoodX && snake.front().y == nFoodY) 
		{
			nScore++;
			while (screen[nFoodY * nScreenWidth + nFoodX] != L' ')
			{
				nFoodX = rand() % nScreenWidth;
				nFoodY = (rand() % (nScreenHeight - 3)) + 3;
			}
			//increase snake size
			for (int i = 0; i < 10; i++)
				snake.push_back({ snake.back().x, snake.back().y });
		}

		// Collision Detect @ World
		if (snake.front().x < 0 || snake.front().x >= nScreenWidth)
			bDead = true;
		if (snake.front().y < 3 || snake.front().y >= nScreenHeight)
			bDead = true;

		//Colision Detect Suicide
		for (list<sSnakeSegment>::iterator i = snake.begin(); i != snake.end(); i++)
			if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y)
				bDead = true;

		snake.pop_back();

		// -Display to player 
		//limpar ecra
		for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';

		for (int i = 0; i < nScreenWidth; i++)
		{
			screen[i] = L'=';
			screen[2 * nScreenWidth + i] = L'=' ;
		}
		wsprintf(&screen[nScreenWidth + 5], L"MG 2750 ---- SNAKE ----   Score: %d", nScore);

		//Snake Body
		for (auto s : snake)
			screen[s.y * nScreenWidth + s.x] = bDead ? L'+' : L'*';

		//Snake Head
		screen[snake.front().y * nScreenWidth + snake.front().x] = bDead ? L'X' : L'O';

		//Food
		screen[nFoodY * nScreenWidth + nFoodX] = L'%';

		if (bDead)
			wsprintf(&screen[15 * nScreenWidth + 40], L"*    PRESS SPACE TO PLAY AGAIN    *");

		//Display buffer to the Screen ( Frame )
		WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight * nScreenWidth, { 0,0 }, &dwBytesWritten);
	}
	while ((0X8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);
	}
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
