#include<iostream>
#include<windows.h> // for Sleep() and gotoxy
#include<conio.h> // for _getch() and _kbhit()


using namespace std;

// ------------All Global Variables

// for keeping track of game-frames
unsigned long int frame = 0; 
// Map Related
const int ROWS = 50;
const int COLS = 30;
const int LAST_X = COLS - 1, LAST_Y = ROWS - 1;
char map[ROWS][COLS] = {};
// General Game Mechanics Flags
bool gameOver = 0;
// Ship Coords
int ship_y = LAST_Y-1, ship_x = LAST_X/2;
// Laser Count and Coords
int laser_count = 0; 
int laser_x[100] = {};
int laser_y[100] = {};
// Asteroids
int ast_count = 0;
int ast_x[100] = {};
int ast_y[100] = {};
// Explosion 
int exp_count = 0;
int exp_x[100] = {};
int exp_y[100] = {};
int exp_state[100] = {};
// Storing keyboard hit key
char key = 0;

// -------------Declaring all functions
// Coding related
void gotoxy(int x, int y);
// Game Related
void cleanBoundary();
void processShip(int x, int y);
void initializeMap();
void renderMap();
void keyListen();
void buttonPressed(char btn);
void moveShipLeft();
void moveShipRight();
void moveShipUp();
void moveShipDown();
// LASER
void shootLaser();
void progressLaser();
void neutralizeLaser(int i);
void sortLaser();
// ASTEROID
void generateAsteroid();
// PROGRESSION
void progressObjects();
void progressAsteroid();
void gameOverFn();




int main()
{	// ALL THE INITIALS HERE aka One Timers
	
	initializeMap();



	do { // -------------------------- GAME LOOP
		frame++;
		//system("CLS");
		
		processShip(ship_x, ship_y);
		renderMap();
		if (frame % 2 == 0) // to slow down the other things
		{
			progressObjects();
			cleanBoundary();
		}




		
		
		keyListen();
		Sleep(10); // Game Overall Speed
	} while (gameOver==0);

	if (gameOver)
	{
		gameOverFn();
	}

	system("pause");
	return 0;
}

void cleanBoundary()
{
	for (int i = 0; i < 30; i++)
	{
		map[49][i] = ' ';
	}
}

void processShip(int x, int y) { // To store the location of ship
	map[y][x-1] = '<';
	map[y][x] = '^';
	map[y][x+1] = '>';
}

void initializeMap() {
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			map[i][j] = ' ';
		}
	}
}

void renderMap() { // To print everything
	for (int i = 0; i < COLS + 2; i++) // Top Boundary
	{
		cout << "=";
	}
	cout << endl;

	for (int i = 0; i < ROWS; i++) // Inner Game
	{
		gotoxy(0,i+1);
		cout << "|";
		for (int j = 0; j < COLS; j++)
		{
			cout << map[i][j];
			
		}
		cout << "|";
	}
	gotoxy(0, 51);
}

void keyListen() {
	if (_kbhit()) // if keyboard key pressed
	{
		key = _getch();
		buttonPressed(key);
	}
}

void buttonPressed(char btn) { // To process the pressed key
	switch (btn)
	{
	case 'a': // Ship Left
	case 'A':
		moveShipLeft();
		break;

	case 'd': // Ship Right 
	case 'D':
		moveShipRight();
		break;

	case 'w': // Ship Up 
	case 'W':
		moveShipUp();
		break;

	case 's': // Ship Down
	case 'S':
		moveShipDown();
		break;

	case 'b': // Shoot Laser
	case 'B':
		shootLaser();
		break;
	case 'p':
	case 'P':
		generateAsteroid();
		break;
	}
	


}

// Button Related Actions
void moveShipLeft()
{
	if ((ship_x - 1) != 0)  // if Left Wing doesn't touch Boundary
	{
		map[ship_y][ship_x + 1] = ' ';
		ship_x--;
	}
}
void moveShipRight()
{
	if ((ship_x + 1) != LAST_X) // if Right Wing doesn't touch Boundary
	{
		map[ship_y][ship_x - 1] = ' ';
		ship_x++;
	}
}
void moveShipUp()
{
	if ((ship_y + 1) != 40) // if Ship doesn't touch Row 30
	{
		map[ship_y][ship_x - 1] = ' ';
		map[ship_y][ship_x] = ' ';
		map[ship_y][ship_x + 1] = ' ';
		ship_y--;
	}
}
void moveShipDown()
{
	if ((ship_y - 1) != LAST_Y - 1) // if Shit doesn't touch Bottom Boundary
	{
		map[ship_y][ship_x - 1] = ' ';
		map[ship_y][ship_x] = ' ';
		map[ship_y][ship_x + 1] = ' ';
		ship_y++;
	}
}
void shootLaser() { // Generates a laser
	laser_x[laser_count] = ship_x;
	laser_y[laser_count] = ship_y - 1;
	map[ship_y - 1][ship_x] = '|';
	laser_count++;
}
void generateAsteroid()
{
	int random = rand() % 30;
	ast_x[ast_count] = random;
	ast_y[ast_count] = 0;
	map[ast_y[ast_count]][ast_x[ast_count]] = 'E';
	ast_count++;
}
void generateExplosion(int x, int y) // <--------- Checkpoint
{
	exp_x[exp_count] = x;
	exp_y[exp_count] = y;
	exp_state[exp_count] = 2;
	exp_count++;
}

// Progression of Objects
void progressObjects() { // Progression for multiple objects
	progressLaser();
	progressAsteroid();
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
			//map[laser_y[i]][laser_x[i]] = ' '; // remove that laser
			//for (int j = 0; j < laser_count; j++) // Re-arrange the laser arrays
			//{
			//	for (int k = 0; k < laser_count; k++)
			//	{
			//		if (laser_y[k] == 0) // rearranges if this condiion follows
			//		{
			//			int temp = 0;
			//			temp = laser_y[k];
			//			laser_y[k] = laser_y[k+1];
			//			laser_y[k+1] = temp;

			//			temp = 0; laser_x[k] = 0;
			//			temp = laser_x[k];
			//			laser_x[k] = laser_x[k + 1];
			//			laser_x[k + 1] = temp;
			//		}
			//	}
			//}
			//laser_count--;
			neutralizeLaser(i);
		}
	}
}
void neutralizeLaser(int i)
{
	laser_y[i] = 0;
	map[laser_y[i]][laser_x[i]] = ' '; // remove that laser
	sortLaser();
	laser_count--;
}
void sortLaser()
{
	for (int j = 0; j < laser_count; j++) // Re-arrange the laser arrays
	{
		for (int k = 0; k < laser_count; k++)
		{
			if (laser_y[k] == 0) // rearranges if this condiion follows
			{
				int temp = 0;
				temp = laser_y[k];
				laser_y[k] = laser_y[k + 1];
				laser_y[k + 1] = temp;

				temp = 0; laser_x[k] = 0;
				temp = laser_x[k];
				laser_x[k] = laser_x[k + 1];
				laser_x[k + 1] = temp;
			}
		}
	}
}
void progressAsteroid()
{
	for (int i = 0; i < ast_count; i++)
	{
		if (ast_y[i] != 49) // if Asteroid not hitting bottom boundary
		{
			map[ast_y[i]][ast_x[i]] = ' ';
			ast_y[i]++;
			map[ast_y[i]][ast_x[i]] = 'E';
		}
		else if (ast_y[i] == 49) // if it is hitting bottom boundary
		{
			map[ast_y[i]][ast_x[i]] = ' '; // remove that asteroid
			for (int j = 0; j < ast_count; j++) // Re-arrange the asteroid arrays
			{
				for (int k = 0; k < ast_count; k++)
				{
					if (ast_y[k] == 49)
					{
						int temp = 0;
						temp = ast_y[k];
						ast_y[k] = ast_y[k + 1];
						ast_y[k + 1] = temp;

						temp = 0; ast_x[k] = 0;
						temp = ast_x[k];
						ast_x[k] = ast_x[k + 1];
						ast_x[k + 1] = temp;
					}
				}
			}
			ast_count--;
		}
	}
}
// General Game
void gameOverFn() { // Prints Game Over on screen
	cout << endl;
	cout << "Game Over" << endl;
}



// Coding Related
void gotoxy(int x, int y) // to get rid of System("CLS") and, get better rendering
{
	COORD coord;
	coord.X = x; // Column
	coord.Y = y; // Row
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}