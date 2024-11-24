#include<iostream>
#include<windows.h> // for Sleep()
#include<conio.h> // for _getch() and _kbhit()


using namespace std;
//Map Related
const int ROWS = 50;
const int COLS = 30;
const int LAST_X = COLS - 1, LAST_Y = ROWS - 1;
char map[ROWS][COLS] = {};
//General Game Mechanics
bool gameOver = 0;
// Ship Coords
int s_y = LAST_Y-1, s_x = LAST_X/2;

void processShip(int x, int y);
void renderMap();
void buttonPressed(char btn);
void gameOverFn();



int main()
{
	char key = 0;
	for (int i = 0; i < ROWS; i++) // ALL THE INITIALS HERE
	{
		for (int j = 0; j < COLS; j++)
		{
			map[i][j] = ' ';
		}
	}

	do { // GAME LOOP
		system("CLS");
		processShip(s_x, s_y);
		renderMap();





		
		if (_kbhit()) // if keyboard key pressed
		{
			key = _getch();
			buttonPressed(key);
		}
		
		Sleep(100); // Game Overall Speed
	} while (gameOver==0);

	if (gameOver == 1)
	{
		gameOverFn();
	}

	system("pause");
	return 0;
}

void processShip(int x, int y) { // To store the location of ship
	map[y][x-1] = '<';
	map[y][x] = '^';
	map[y][x+1] = '>';
}

void renderMap() { // To print everything
	for (int i = 0; i < COLS + 2; i++) // Top Boundary
	{
		cout << "=";
	}
	cout << endl;

	for (int i = 0; i < ROWS; i++) // Inner Game
	{
		cout << "|";
		for (int j = 0; j < COLS; j++)
		{
			cout << map[i][j];
			
		}
		cout << "|" << endl;
	}
	for (int i = 0; i < COLS + 2; i++) // Bottom Boundary
	{
		cout << "=";
	}
	
}


void buttonPressed(char btn) { // To process the pressed key
	switch (btn)
	{
	case 'a': // Left
	case 'A':
		if ((s_x-1) != 0)  // if Left Wing doesn't touch Boundary
		{
			map[s_y][s_x+1] = ' ';
			s_x--;
		}
		break;

	case 'd': // Right 
	case 'D':
		if ((s_x+1) != LAST_X) // if Right Wing doesn't touch Boundary
		{
			map[s_y][s_x-1] = ' ';
			s_x++;
		}
		break;

	case 'w': // Up 
	case 'W':
		if ((s_y + 1) != 40) // if Ship doesn't touch Row 30
		{
			map[s_y][s_x-1] = ' ';
			map[s_y][s_x] = ' ';
			map[s_y][s_x+1] = ' ';
			s_y--;
		}
		break;

	case 's': // Down
	case 'S':
		if ((s_y - 1) != LAST_Y-1) // if Shit doesn't touch Bottom Boundary
		{
			map[s_y][s_x - 1] = ' ';
			map[s_y][s_x] = ' ';
			map[s_y][s_x + 1] = ' ';
			s_y++;
		}
		break;
	}

}

void gameOverFn() { // Prints Game Over on screen
	cout << endl;
	cout << "Game Over" << endl;
}