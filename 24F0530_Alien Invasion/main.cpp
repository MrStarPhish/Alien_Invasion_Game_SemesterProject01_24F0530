#include<iostream>
#include<windows.h> // for Sleep() and gotoxy()
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
bool DevNotes = 0;
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
int enemy_life[100] = {}; // Keeps track of the duration of life of enemy ship (in frames)
// Enemy Lasers
int e_laser_x[100] = {};
int e_laser_y[100] = {};
int e_laser_count = 0;
// Collectibles
		//void generateCollectible(int x);
// ---Stars
int star_count = 0;
int star_x[100] = {};
int star_y[100] = {};
// ---MedKit
int med_count = 0;
int med_x[100] = {};
int med_y[100] = {};


// Storing keyboard hit key
char key = 0;

// -------------Declaring all functions
// Coding related
void gotoxy(int x, int y);
void hideCursor();
void setConsoleSize(int width, int height);
void debugMsg(int code, int frame);
void printArr(int arr[], int size, int col, int row);
void printStr(string line, int i_frame, int row);
void clearRow(int row);
// Dev Notes
void checkForDevNotesDisplay();
void showDevNotes(bool &a);
void printDevNotes();
void clearDevNotes();
void clearDevRow(int row);
void printLaserStats();
void printAsteroidStats();
void printEnemyStats();
void printEnemyLaserStats();
void printFrameCount();
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
bool laserHitEnemy(int laser_n);
int hitEnemyNum(int laser_n);
bool AstHitShip(int ast_n);
// EXPLOSION RELATED
void generateExplosion(int x, int y);
void progressExplosion();
void sortExplosion();
// ENEMY SHIP RELATED
void generateEnemy(int y);
void progressEnemy();
void clearEnemyCurrentLocation(int num);
void updateEnemyLocation(int num);
void neutralizeEnemy(int i);
void sortEnemy();
// ENEMY LASER RELATED
void generateEnemyLaser();
void shootEnemyLaser(int enemy_num);
void progressEnemyLaser();
void neutralizeEnemyLaser(int e_laser_num);
void sortEnemyLaser();
// COLLECTIBLES
void generateCollectible(int x);
void progressCollectible();
// ---STARS
void generateStar(int x);
void progressStar();
void neutralizaStar(int star_n);
void sortStar();
// ---MED KIT
void generateMed(int x);
void progressMed();
void neutralizeMed(int med_n);
void sortMed();

// PROGRESSION
void progressObjects();
void gameOverFn();







int main()
{	// ALL THE INITIALS HERE aka One Timers
	hideCursor();
	setConsoleSize(1280, 720);
	initializeMap();



	do { // -------------------------- GAME LOOP
		// keeping track of game's progression
		FRAME++;
		// Ship and Render
		
		processShip(ship_x, ship_y);
		renderMap();
		// Print Below-Game stuff here
		checkForDevNotesDisplay();
		
		
		// General
		progressAsteroid();
		progressLaser();
		progressExplosion();
		progressEnemy();
		progressEnemyLaser();
		progressCollectible();
		cleanBoundary(); // to remove any buggy display at top/bottom

		keyListen();
		Sleep(17); // Game Overall Speed
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
	gotoxy(0, 0);
}
void cleanBoundary()
{	// To clean Top and Bottom boundary to refrain from any buggy still display
	if (FRAME % 25 == 0)
	{
		for (int i = 0; i < 30; i++)
		{
			map[49][i] = map[0][i] = ' ';
		}
	}
}
// DevNotes Related
void checkForDevNotesDisplay()
{
	if (DevNotes)
	{
		if (FRAME % 10 == 0)
			clearDevNotes();
		printDevNotes();
	}
	else
	{
		clearDevNotes();
	}
}
void showDevNotes(bool &a)
{
	static int button = 0;
	if (button == 0)
	{
		button = 1;
		a = 1;
	}
	else if (button == 1)
	{
		button = 0;
		a = 0;
	}
}
void printDevNotes()
{
	gotoxy(125, 0);
	cout << "-- DEVELOPER NOTES --";
	gotoxy(125, 1);
	printLaserStats();
	printAsteroidStats();
	printEnemyStats();
	printEnemyLaserStats();
	printFrameCount();
}
void printLaserStats()
{
	gotoxy(125, 1);
	cout << "LASER RELATED";
	gotoxy(125, 2);
	cout << "LASER Y:"; printArr(laser_y, laser_count, 135, 2);
	gotoxy(125, 3);
	cout << "LASER X:"; printArr(laser_x, laser_count, 135, 3);
}
void printAsteroidStats()
{
	gotoxy(125, 4);
	cout << "ASTEROID RELATED";
	gotoxy(125, 5);
	cout << "AST Y:"; printArr(ast_y, ast_count, 135, 5);
	gotoxy(125, 6);
	cout << "AST X:"; printArr(ast_x, ast_count, 135, 6);
}
void printEnemyStats()
{
	gotoxy(125, 7);
	cout << "ENEMY RELATED";
	gotoxy(125, 8);
	cout << "ENEMY Y:"; printArr(enemy_y, enemy_count, 135, 8);
	gotoxy(125, 9);
	cout << "ENEMY X:"; printArr(enemy_x, enemy_count, 135, 9);
}
void printEnemyLaserStats()
{
	gotoxy(125, 10);
	cout << "ENEMY RELATED";
	gotoxy(125, 11);
	cout << "E_LASER Y:"; printArr(e_laser_y, e_laser_count, 135, 11);
	gotoxy(125, 12);
	cout << "E_LASER X:"; printArr(e_laser_x, e_laser_count, 135, 12);
}
void printFrameCount()
{
	gotoxy(125, 15);
	cout << "FRAME: " << FRAME;
}
void clearDevNotes()
{
	for (int i = 0; i <= 15; i++)
	{
		clearDevRow(i);
	}
}
void clearDevRow(int row)
{
	gotoxy(125, row);
	for (int i = 0; i < 80; i++)
	{
		cout << " ";
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
	case 'i':
	case 'I':
		generateCollectible(2);
		break;
	case '0':
		showDevNotes(DevNotes);
		break;
	case '9':
		generateEnemyLaser();
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
		enemy_x[enemy_count] = x = LAST_X - 1;
		enemy_direction[enemy_count] = 2;
		// Right Side, Towards Left
	}
	enemy_y[enemy_count] = y;

	map[y][x - 1] = '{';
	map[y][x] = 'V';
	map[y][x + 1] = '}';

	enemy_count++;
	enemy_life[enemy_count] = 1;
}
void generateCollectible(int x)
{
	if (x == 1)
	{
		generateStar(rand() % 30);
	}
	if (x == 2)
	{
		generateMed(rand() % 30);
	}
}
void generateStar(int x) // checkpoint
{
	star_y[star_count] = 0;
	star_x[star_count] = x;
	map[0][x] = '$';
	star_count++;
}
void generateMed(int x)
{
	med_y[med_count] = 0;
	med_x[med_count] = x;
	map[0][x] = '+';
	med_count++;
}




// Flags Functions
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
bool laserHitEnemy(int laser_n)
{
	int laserVar = laser_x[laser_n];
	for (int j = 0; j < enemy_count; j++)
	{
		if ( (laserVar == enemy_x[j]) || (laserVar == enemy_x[j]-1) || (laserVar == enemy_x[j]+1) ) // checking collision for left and right wing as well
		{
			if ((laser_y[laser_n] == enemy_y[j]) || (laser_y[laser_n] - 1 == enemy_y[j]))
			{
				return 1;
			}
		}
	}
	return 0;
}
bool AstHitShip(int ast_n)
{
	int ax = ast_x[ast_n];
	int ay = ast_y[ast_n];
	if (ay == ship_y)
	{
		if ((ax==ship_x) || (ax==ship_x+1) || (ax==ship_x-1)) // if asteroid hits Ship or its wing
		{
			return 1;
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
	return 0;
}
int hitEnemyNum(int laser_n)   
{ // To know, which Nth Asteroid is getting hit with laser
	int laserVar = laser_x[laser_n];
	for (int j = 0; j < enemy_count; j++)
	{
		if ((laserVar == enemy_x[j]) || (laserVar == enemy_x[j] - 1) || (laserVar == enemy_x[j] + 1)) // checking collision for left and right wing as well
		{
			if ((laser_y[laser_n] == enemy_y[j]) || (laser_y[laser_n] - 1 == enemy_y[j]))
			{
				gotoxy(0, 60); cout << j;
				return j;
			}
		}
	}
	return 0;
}

// Main Functions
void progressLaser()
{ // Movement of Laser(s)
	for (int i = 0; i < laser_count; i++) // i'th laser
	{
		if (laserHitAst(i))
		{
			int astNum = hitAstNum(i);
			map[laser_y[i]][laser_x[i]] = ' ';
			map[ast_y[i]][ast_x[i]] = ' ';
			map[ast_y[i]+1][ast_x[i]] = ' ';
			neutralizeAsteroid(astNum);
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
		if (laserHitEnemy(i))
		{
			int enemyNum = hitEnemyNum(i);
			neutralizeEnemy(enemyNum);
			generateExplosion(laser_x[i], laser_y[i]);
			neutralizeLaser(i);
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
		if (AstHitShip(i))
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
void progressEnemy()
{
	for (int i = 0; i < enemy_count; i++)
	{
		if (enemy_direction[i] == 2)
			if ((enemy_x[i] - 1) != 0) // if Left Wing of Enemy is NOT at column = 0 / very left , then move ship left
			{
				clearEnemyCurrentLocation(i);
				enemy_x[i]--;
				updateEnemyLocation(i);
				if (enemy_x[i] == 1) // switching direction
					enemy_direction[i] = 1;
			}
		if (enemy_direction[i] == 1)
		{
			if ((enemy_x[i] + 1) != LAST_X) // if Right Wing of Enemy is NOT at column = 29 / very right , then move ship right
			{
				clearEnemyCurrentLocation(i);
				enemy_x[i]++;
				updateEnemyLocation(i);
				if (enemy_x[i] == LAST_X - 1) // switching direction
					enemy_direction[i] = 2;
			}

		}
		if (enemy_life[i] != 0) // updating enemy ship's lifetime
		{
			enemy_life[i]++;
		}
	}
}
void clearEnemyCurrentLocation(int num)
{
	// To Clear the current position of the enemy ship
	map[enemy_y[num]][enemy_x[num] - 1] = ' ';
	map[enemy_y[num]][enemy_x[num]] = ' ';
	map[enemy_y[num]][enemy_x[num] + 1] = ' ';
}
void updateEnemyLocation(int num)
{
	// To reprint the Enemy Ship (Being used after updating the coordinates of the ship)
	map[enemy_y[num]][enemy_x[num] - 1] = '{';
	map[enemy_y[num]][enemy_x[num]] = 'V';
	map[enemy_y[num]][enemy_x[num] + 1] = '}';
}
void neutralizeEnemy(int i)
{
	clearEnemyCurrentLocation(i);
	enemy_y[i] = enemy_x[i] = 49;
	enemy_life[i] = 0;
	sortEnemy();
	enemy_count--;
}
void sortEnemy()
{// To sort the Array of Enemy Ships Coordinates
	int count = 0;
	for (int i = 0; i < enemy_count; i++) // Removing all indexes with value 49
	{
		if (enemy_y[i] != 49) {
			enemy_y[count] = enemy_y[i];
			enemy_x[count] = enemy_x[i];
			if (enemy_life[i] != 0)
			{
				enemy_life[count] = enemy_life[i]; // sorting enemy_life array
			}
			count++;
		}
		
	}
	for (int i = count; i < enemy_count; i++) {
		enemy_y[i] = 0;
		enemy_x[i] = 0;
		enemy_life[i] = 0;
	}
}
void progressStar()
{
	for (int i = 0; i < star_count; i++)
	{
		if (star_y[i] != 49)
		{
			map[star_y[i]][star_x[i]] = ' ';
			star_y[i]++;
			map[star_y[i]][star_x[i]] = '$';
		}
		else if (star_y[i] == 49)
		{
			neutralizaStar(i);
		}
	}
}
void neutralizaStar(int star_n)
{
	map[star_y[star_n]][star_x[star_n]] = ' ';
	star_y[star_n] = 49;
	sortStar();
	star_count--;
}
void sortStar()
{ // To sort the Array of Stars Coordinates
	int count = 0;
	for (int i = 0; i < star_count; i++) // Removing all indexes with value 49
	{
		if (star_y[i] != 49) {
			star_y[count] = star_y[i];
			star_x[count] = star_x[i];
			count++;
		}
	}
	for (int i = count; i < star_count; i++) {
		star_y[i] = 0;
		star_x[i] = 0;
	}
}
void progressMed()
{
	for (int i = 0; i < med_count; i++)
	{
		if (med_y[i] != 49)
		{
			map[med_y[i]][med_x[i]] = ' ';
			med_y[i]++;
			map[med_y[i]][med_x[i]] = '+';
		}
		else if (med_y[i] == 49)
		{
			neutralizeMed(i);
		}
	}
}
void neutralizeMed(int med_n)
{
	map[med_y[med_n]][med_x[med_n]] = ' ';
	med_y[med_n] = 49;
	sortMed();
	med_count--;
}
void sortMed()
{
	int count = 0;
	for (int i = 0; i < med_count; i++) // Removing all indexes with value 49
	{
		if (med_y[i] != 49) {
			med_y[count] = med_y[i];
			med_x[count] = med_x[i];
			count++;
		}
	}
	for (int i = count; i < med_count; i++) {
		med_y[i] = 0;
		med_x[i] = 0;
	}
}

// Enemey Laser Related
void generateEnemyLaser()
{
	for (int i = 0; i < enemy_count; i++)
	{
		shootEnemyLaser(i);
	}
}
void shootEnemyLaser(int enemy_num)
{
	// Generates an enemy laser
	int z = enemy_num;
	e_laser_x[e_laser_count] = enemy_x[z]; // same X-axis
	e_laser_y[e_laser_count] = enemy_y[z] + 1; // +1 Y-Axis
	map[enemy_y[z] + 1][enemy_x[z]] = '!';
	e_laser_count++;
}
void progressEnemyLaser()
{
	for (int i = 0; i < e_laser_count; i++)
	{
		if (e_laser_y[i] != 49) // if Asteroid not hitting bottom boundary
		{
			map[e_laser_y[i]][e_laser_x[i]] = ' ';
			e_laser_y[i]++;
			map[e_laser_y[i]][e_laser_x[i]] = '!';
		}
		else if (e_laser_y[i] == 49) // if it is hitting bottom boundary
		{
			neutralizeEnemyLaser(i);
		}
		/*if (AstHitShip(i))
		{
			neutralizeAsteroid(i);
		}*/
	}
}
void neutralizeEnemyLaser(int e_laser_num)
{ // Removal of Enemy Laser and other related functions
	int z = e_laser_num;
	map[e_laser_y[z]][e_laser_x[z]] = ' ';
	e_laser_y[z] = 49;
	sortEnemyLaser();
	e_laser_count--;
}
void sortEnemyLaser()
{
	// To sort the Array of Enemy Lasers Coordinates
	int count = 0;
	for (int i = 0; i < e_laser_count; i++) // Removing all indexes with value 49
	{
		if (e_laser_y[i] != 49) {
			e_laser_y[count] = e_laser_y[i];
			e_laser_x[count] = e_laser_x[i];
			count++;
		}
	}
	for (int i = count; i < e_laser_count; i++) {
		e_laser_y[i] = 0;
		e_laser_x[i] = 0;
	}
}


// Progression of Objects
void progressObjects() { // Progression for multiple objects
	progressLaser();
	progressAsteroid();
}
void progressCollectible()
{
	progressStar();
	progressMed();
}

// General Game
void gameOverFn() { // Prints Game Over on screen
	cout << endl;
	cout << "Game Over" << endl;
}


// Coding and Debugging Related
void gotoxy(int x, int y) // to get rid of System("CLS") and, get better rendering
{
	COORD coord;
	coord.X = x; // Column
	coord.Y = y; // Row
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void hideCursor()
{   // To hide the blinking cursor on the console, for better visibility
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = false; 
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}
void setConsoleSize(int width, int height)
{   // To make sure that the console opens up in 720p dimensions, to refrain from any buggy display
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize;
	bufferSize.X = width;
	bufferSize.Y = height;
	SetConsoleScreenBufferSize(consoleHandle, bufferSize);
	SMALL_RECT windowSize;
	windowSize.Left = 0;
	windowSize.Top = 0;
	windowSize.Right = width - 1;
	windowSize.Bottom = height - 1;
	SetConsoleWindowInfo(consoleHandle, TRUE, &windowSize);
}
void debugMsg(int code, int i_frame)
{
	if (code == 1)
	{
		gotoxy(0,55);
		cout << "Asteroid Hit : " << i_frame;
	}
}
void printArr(int arr[],int size, int col, int row)
{
	gotoxy(col, row);
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

// Progress Note:  
// Work on Collectibles.