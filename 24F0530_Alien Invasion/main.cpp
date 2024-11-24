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
int ship_y = LAST_Y-1, ship_x = LAST_X/2;
// Laser
int laser_count = 0; 
int laser_x[50] = {};
int laser_y[50] = {};

// -------------Declaring all functions
// Game Related
void processShip(int x, int y);
void renderMap();
void buttonPressed(char btn);
void shootLaser();
void progressObjects();
void progressLaser();
void gameOverFn();



int main()
{	// ALL THE INITIALS HERE , One Timers
	char key = 0; // for storing keyboard key
	for (int i = 0; i < ROWS; i++) 
	{
		for (int j = 0; j < COLS; j++)
		{
			map[i][j] = ' ';
		}
	}

	do { // -------------------------- GAME LOOP
		system("CLS");
		processShip(ship_x, ship_y);
		renderMap();
		progressObjects();




		
		if (_kbhit()) // if keyboard key pressed
		{
			key = _getch();
			buttonPressed(key);
		}
		
		Sleep(100); // Game Overall Speed
	} while (gameOver==0);

	if (gameOver)
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
	case 'a': // Ship Left
	case 'A':
		if ((ship_x-1) != 0)  // if Left Wing doesn't touch Boundary
		{
			map[ship_y][ship_x+1] = ' ';
			ship_x--;
		}
		break;

	case 'd': // Ship Right 
	case 'D':
		if ((ship_x+1) != LAST_X) // if Right Wing doesn't touch Boundary
		{
			map[ship_y][ship_x-1] = ' ';
			ship_x++;
		}
		break;

	case 'w': // Ship Up 
	case 'W':
		if ((ship_y + 1) != 40) // if Ship doesn't touch Row 30
		{
			map[ship_y][ship_x-1] = ' ';
			map[ship_y][ship_x] = ' ';
			map[ship_y][ship_x+1] = ' ';
			ship_y--;
		}
		break;

	case 's': // Ship Down
	case 'S':
		if ((ship_y - 1) != LAST_Y-1) // if Shit doesn't touch Bottom Boundary
		{
			map[ship_y][ship_x - 1] = ' ';
			map[ship_y][ship_x] = ' ';
			map[ship_y][ship_x + 1] = ' ';
			ship_y++;
		}
		break;

	case 'b': // Shoot Laser
	case 'B':
		shootLaser();
		break;
	}

}

void shootLaser() { // Generates a laser
	laser_x[laser_count] = ship_x;
	laser_y[laser_count] = ship_y - 1;
	map[ship_y - 1][ship_x] = '|';
	laser_count++;
}

void progressObjects() { // Progression for multiple objects
	progressLaser();
}


void progressLaser() { // Movement of Laser(s)
	for (int i = 0; i < laser_count; i++)
	{
		if (laser_y[i] != 0) // if laser not hitting top boundary, move them upward
		{
			map[laser_y[i]][laser_x[i]] = ' ';
			laser_y[i]--;
			map[laser_y[i]][laser_x[i]] = '|';
		}
		else if (laser_y[i]==0) // If laser hit boundary, 
		{
			map[laser_y[i]][laser_x[i]] = ' '; // remove that laser
			for (int j = 0; j < laser_count; j++) // Re-arrange the laser arrays
			{
				for (int k = 0; k < laser_count; k++)
				{
					if (laser_y[k] == 0)
					{
						int temp = 0;
						temp = laser_y[k];
						laser_y[k] = laser_y[k+1];
						laser_y[k+1] = temp;

						temp = 0; laser_x[k] = 0;
						temp = laser_x[k];
						laser_x[k] = laser_x[k + 1];
						laser_x[k + 1] = temp;
					}
				}
			}
			laser_count--;
		}
	}
}

void gameOverFn() { // Prints Game Over on screen
	cout << endl;
	cout << "Game Over" << endl;
}