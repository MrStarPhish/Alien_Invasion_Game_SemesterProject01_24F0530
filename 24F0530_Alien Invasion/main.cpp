#include<iostream>
#include<windows.h> // for Sleep() and gotoxy
#include<conio.h> // for _getch() and _kbhit()


using namespace std;

// ------------All Global Variables

// for keeping track of game-frames
unsigned long int FRAME = 0; 
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
// Enemy Ship
int enemy_count = 0;
int enemy_x[100] = {};
int enemy_y[100] = {};
int enemy_direction[100] = {}; // Direction of motion of enemy ship. 1-Right, 2-left
// Storing keyboard hit key
char key = 0;

// -------------Declaring all functions
// Coding related
void gotoxy(int x, int y);
void debugMsg(int code, int frame);
void printArr(int arr[], int size, int row);
void printStr(string line, int i_frame, int row);
void clearRow(int row);
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
void progressAsteroid();
void neutralizeAsteroid(int i);
void sortAsteroid();
// LASER INTERACTION
bool laserHitAst(int laser_n);
int hitAstNum(int laser_n);
// EXPLOSION RELATED
void generateExplosion(int x, int y);
void progressExplosion();
void sortExplosion();
// ENEMY SHIP RELATED
void generateEnemy(int y);
// PROGRESSION
void progressObjects();
void gameOverFn();







int main()
{	// ALL THE INITIALS HERE aka One Timers
	
	initializeMap();



	do { // -------------------------- GAME LOOP
		// keeping track of game's progression
		FRAME++;
		// Ship and Render
		processShip(ship_x, ship_y);
		renderMap();
		// Print Below-Game stuff here
		clearRow(56);
		clearRow(57);
		printArr(laser_y, laser_count, 56);
		printArr(ast_y, ast_count, 57);
		if (FRAME % 5 == 0)  // to slow down the other things
		{
			progressAsteroid();
			 // to remove any buggy display at bottom
		}
		if (FRAME % 50 == 0)
		{
			//sortExplosion();

		}


		progressLaser();
		progressExplosion();
		/*exp_count--;
		cleanBoundary();*/
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

// Render Related

void processShip(int x, int y)
{ // To store the location of ship
	map[y][x-1] = '<';
	map[y][x] = '^';
	map[y][x+1] = '>';
}

void initializeMap() {
	// Initializes the whole map array
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			map[i][j] = ' ';
		}
	}
}

void renderMap() 
{ // To print everything
	gotoxy(0, 0);
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
	for (int i = 0; i < COLS + 2; i++) // Top Boundary
	{
		cout << "=";
	}
}
void cleanBoundary()
{
	for (int i = 0; i < 30; i++)
	{
		map[49][i] = ' ';
	}
}

// Button Listener
void keyListen()
{
	if (_kbhit()) // if keyboard key pressed
	{
		key = _getch();
		buttonPressed(key);
	}
}

void buttonPressed(char btn) 
{ // To process the pressed key
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
	case 'o':
	case 'O':
		generateEnemy(10);
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
void shootLaser() 
{ // Generates a laser
	laser_x[laser_count] = ship_x;
	laser_y[laser_count] = ship_y - 1;
	map[ship_y - 1][ship_x] = '|';
	laser_count++;
}
void generateAsteroid()
{ // To create an asteroid
	int random = rand() % 30;
	ast_x[ast_count] = random;
	ast_y[ast_count] = 0;
	map[ast_y[ast_count]][ast_x[ast_count]] = '#';
	ast_count++;
}
void generateExplosion(int x, int y)
{
	exp_x[exp_count] = x;
	exp_y[exp_count] = y;
	exp_state[exp_count] = 3;
	map[y][x] = 'O';
	exp_count++;
}

// Progression of Objects
void progressObjects() { // Progression for multiple objects
	progressLaser();
	progressAsteroid();
}


void progressLaser()
{ // Movement of Laser(s)
	for (int i = 0; i < laser_count; i++) // i'th laser
	{
		if (laserHitAst(i))
		{
			int astNum = hitAstNum(i);
			debugMsg(1,FRAME);
			map[laser_y[i]][laser_x[i]] = ' ';
			map[ast_y[i]][ast_x[i]] = ' ';
			map[ast_y[i]+1][ast_x[i]] = ' ';
			neutralizeAsteroid(astNum); // <- CHeckpoint
			generateExplosion(laser_x[i],laser_y[i]);
			neutralizeLaser(i);
		}
		else if (laser_y[i] != 0) // if laser not hitting top boundary, move them upward
		{
			map[laser_y[i]][laser_x[i]] = ' ';
			laser_y[i]--;
			map[laser_y[i]][laser_x[i]] = '|';
		}
		else if (laser_y[i]==0) // If laser hit boundary, 
		{
			neutralizeLaser(i);
		}
	}
}

bool laserHitAst(int laser_n)
{ // To check whether Laser is Hitting any Asteroid or not
	for (int j = 0; j < ast_count; j++)
	{
		if (laser_x[laser_n] == ast_x[j])
		{
			if ( (laser_y[laser_n] == ast_y[j]) || (laser_y[laser_n]-1 == ast_y[j]))
			{
				return 1;
			}
		}
	}
	return 0;
}
int hitAstNum(int laser_n)   
{ // To know, which Nth Asteroid is getting hit with laser
	for (int j = 0; j < ast_count; j++)
	{
		if (laser_x[laser_n] == ast_x[j])
		{
			if ((laser_y[laser_n] == ast_y[j]) || (laser_y[laser_n] - 1 == ast_y[j]))
			{
				gotoxy(0, 58); cout << j;
				return j;
			}
		}
	}
}


void neutralizeLaser(int i)
{ // Remove a specific laser and perform related operations
	laser_y[i] = 0;
	map[laser_y[i]][laser_x[i]] = ' '; // remove that laser
	sortLaser();
	laser_count--;
}
void sortLaser()
{  // Sort the Array of Laser Coordinates, so the Neutralized Lasers get aside
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
{ // Movement of Asteroid
	for (int i = 0; i < ast_count; i++)
	{
		if (ast_y[i] != 49) // if Asteroid not hitting bottom boundary
		{
			map[ast_y[i]][ast_x[i]] = ' ';
			ast_y[i]++;
			map[ast_y[i]][ast_x[i]] = '#';
		}
		else if (ast_y[i] == 49) // if it is hitting bottom boundary
		{
			neutralizeAsteroid(i);
		}
	}
}
void neutralizeAsteroid(int i)
{ // Removal of Asteroid and other related functions
	map[ast_y[i]][ast_x[i]] = ' ';
	ast_y[i] = 49;
	sortAsteroid();
	ast_count--;
}
void progressExplosion()
{ // For creation of Explosion Animation :>
	for (int i = 0; i < exp_count; i++)
	{
		if (exp_state[i] == 3) // first stage of explosion
		{
			map[exp_y[i]][exp_x[i]] = 'O';
		}
		else if (exp_state[i] == 2) // second stage of explosion
		{
			map[exp_y[i]][exp_x[i]] = 'o';
		}
		else if (exp_state[i] == 1) // second stage of explosion
		{
			map[exp_y[i]][exp_x[i]] = '.';
		}
		else if (exp_state[i] == 0) // removal/final stage of explosion
		{
			map[exp_y[i]][exp_x[i]] = ' ';
		}
	}
	for (int i = 0; i < exp_count; i++)
	{
		if (exp_state[i] > 0)
			exp_state[i]--;
	}
}
void sortExplosion()
{	// Sort the Array of Explosion Coordinates, so the Neutralized Lasers get aside
		for (int j = 0; j < exp_count; j++) // Re-arrange the laser arrays
		{
			for (int k = 0; k < exp_count; k++)
			{
				if (exp_state[k] == 0) // rearranges if this condiion follows
				{
					int temp = 0;
					temp = exp_y[k];
					exp_y[k] = exp_y[k + 1];
					exp_y[k + 1] = temp;

					temp = 0; exp_x[k] = 0;
					temp = exp_x[k];
					exp_x[k] = exp_x[k + 1];
					exp_x[k + 1] = temp;

					temp = 0; exp_state[k] = 0;
					temp = exp_state[k];
					exp_state[k] = exp_state[k + 1];
					exp_state[k + 1] = temp;
				}
			}
		}
		exp_count--;
}
void sortAsteroid()
{ // To sort the Array of Asteroids Coordinates
	int count = 0; 
	for (int i = 0; i < ast_count; i++) // Removing all indexes with value 49
	{
		if (ast_y[i] != 49) { 
			ast_y[count] = ast_y[i];
			ast_x[count] = ast_x[i];
			count++;
		}
	}
	for (int i = count; i < ast_count; i++) {
		ast_y[i] = 0;
		ast_x[i] = 0; 
	}
}
// General Game
void gameOverFn() { // Prints Game Over on screen
	cout << endl;
	cout << "Game Over" << endl;
}

void generateEnemy(int y)
{
	int x = 0;
	if (FRAME % 2 == 0) // Randomizing the spawn and flow direction
	{
		enemy_x[enemy_count] = x = 1;
		enemy_direction[enemy_count] = 1; 
		// Left Side, Towards Right
	}
	else
	{
		enemy_x[enemy_count] = x = LAST_X-1;
		enemy_direction[enemy_count] = 2;
		// Right Side, Towards Left
	}
	enemy_y[enemy_count] = y;

	map[y][x - 1] = '{';
	map[y][x] = 'V';
	map[y][x + 1] = '}';

	enemy_count++;
}

// Coding and Debugging Related
void gotoxy(int x, int y) // to get rid of System("CLS") and, get better rendering
{
	COORD coord;
	coord.X = x; // Column
	coord.Y = y; // Row
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void debugMsg(int code, int i_frame)
{
	if (code == 1)
	{
		gotoxy(0,55);
		cout << "Asteroid Hit : " << i_frame;
	}
}

void printArr(int arr[],int size, int row)
{
	gotoxy(0, row);
	for (int i = 0; i < size; i++)
	{
		cout << arr[i] << " ";
	}
}
void clearRow(int row)
{
	gotoxy(0, row);
	for (int i = 0; i < 30; i++)
	{
		cout << " ";
	}
}
void printStr(string line, int i_frame, int row)
{
	static int ity = i_frame+10;
	
	gotoxy(0, row);
	cout << line;
	//if (ity == FRAME)
		//clearRow(58);
	//static int activationFrame = -1; // Track when the text was printed
	//static int asteroidHitCount = 0;

	//if (activationFrame == -1) {
	//	activationFrame = i_frame; // Initialize activation frame only on first call
	//}

	//// Print the string at the specified position
	//gotoxy(0, row);
	//cout << line << " : " << ++asteroidHitCount;

	//// Check if the time to clear has arrived
	//if (FRAME >= activationFrame + 20) 
	//{
	//	clearRow(row); // Clear the specified row
	//	activationFrame = -1; // Reset activation frame for the next activation
	//}
}

// Progress Note:  To make: Enemy Progress