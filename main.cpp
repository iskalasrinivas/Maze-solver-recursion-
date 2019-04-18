// Copyright (c) 2019 Ari Kupferberg, Bharat Mathur, Brenda Scheufele, Jagadesh Nagireddi, Raja Iskala
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/**
* @file maze_solver.cpp
* @author Ari Kupferberg, Bharat Mathur, Brenda Scheufele, Jagadesh Nagireddi, Raja Iskala
* @date 24 Mar 2019
* @copyright 2019 Ari Kupferberg, Bharat Mathur, Brenda Scheufele, Jagadesh Nagireddi, Raja Iskala
* @brief This file describes a maze-solving algorithm using a recursive function
*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Symbols:
// '.' = open
// '#' = blocked
// 'S' = start
// 'G' = goal
// '+' = path
// 'x' = bad path

/**
* @brief This function advances the path North.
* @param x Current x position
* @param y Current y position
* @return Vector of updated coordinates in North direction
*/
vector<int> goNorth(int,int);

/**
* @brief This function advances the path East.
* @param x Current x position
* @param y Current y position
* @return Vector of updated coordinates in East direction
*/
vector<int> goEast(int,int);

/**
* @brief This function advances the path South.
* @param x Current x position
* @param y Current y position
* @return Vector of updated coordinates in South direction
*/
vector<int> goSouth(int,int);

/**
* @brief This function advances the path West.
* @param x Current x position
* @param y Current y position
* @return Vector of updated coordinates in West direction
*/
vector<int> goWest(int,int);

/**
* @brief This function checks if the path has an obstacle.
* @param x Current x position
* @param y Current y position
* @param maze The maze array
* @return True if the current cell is an obstacle.
*/
bool obstacleCheck(int,int,vector<vector<char>>);

/**
* @brief This function checks if the path is outside the maze.
* @param x Current x position
* @param y Current y position
* @return True if the current cell is outside the maze.
*/
bool outsideMaze(int,int);

/**
* @brief This function checks if the position is the goal.
* @param x Current x position
* @param y Current y position
* @param goalX goal x position
* @param goalY goal y position
* @return True if the current cell is the goal
*/
bool goalCheck(int,int,int,int);

/**
* @brief This function recursively finds a valid path from the start to the goal.
* 
* This function first checks that the position attempted is not an obstacle or outside the maze.
* Then the function checks if it's the goal position, and if it isn't, attemps to move again.
* Next, it runs through the movement function, in order of North, East, South, West.
* If any of these function return a possible move, it saves the new move in a vector array, 
* and then returns True and goes into the recursion.
* If no move can be made, the function notes the wrong move with an 'X' and backtracks.
* Once the final goal is reached, the function returns a final True and completes.
* 
* @param x Current x position
* @param y Current y position
* @return True if a path from start to goal is found
*/
bool findPath(int,int);

/**
* @brief The function displays the maze itself, with boundaries and numbering.
* @return None
*/
void displayMaze();

/**
* @brief This function checks for and removes any X's from the maze display.
* This function runs a loop to check if there are any 'x's in the maze.
* If one is found, the function then runs a second loop to rewrite every 'x' with '.'
* Once this is done, the clean maze is then displayed using displayMaze().
* @return None
*/
void mazePathDisplay();

int goalX{};    ///< Initialize variable to store x position of goal
int goalY{};	///< Initialize variable to store y position of goal

vector<vector<int>> pathTaken; ///< Initialize variable to store solution path
int sizePathTaken;  ///< Initialize variable to store Size of pathTaken
vector<string> movements; ///< Initialize variable to store string vector of actions

const int nRows = 6, ///< Constant value representing the number of maze rows 
nCols = 10; ///< Constant value representing the number of maze columns
vector<vector<char>> maze {
	{'.','.','.','#','.','.','.','#','#','.'},
	{'.','#','.','.','.','#','.','.','#','.'},
	{'.','#','.','#','#','.','#','.','#','.'},
	{'#','.','.','.','#','.','.','.','#','#'},
	{'#','#','.','#','.','#','.','#','.','#'},
	{'.','.','.','.','.','#','.','.','.','G'},
	};							///< A 2-D character vector that stores the maze itself.

/**
* @brief This is the main function
* @return 0 Exit status
*/
int main()
{
	int startx; ///< Initialize variable to store user input of x start position
	int starty;  ///< Initialize variable to store user input of y start position

	cout << "The maze looks like this:";
	displayMaze();
	char choice{};
	cout << "Would you like to use a different goal position than the predefined one? [Y/n]: ";
	cin >> choice;
	switch(choice) { // Switch that lets user choose how to select the goal position
		case 'N':
		case 'n':
		{
			goalX = nRows-1; goalY = nCols-1; // Save goal position as bottom right corner
			break;
		}
		case 'Y':
		case 'y':
		{
			maze[nRows-1][nCols-1]='.'; // 
			cout << "Please enter the goal position (x y): "; // Prompts user for the goal position.
			cin >> goalX >> goalY;
			// Checking for valid goal positions, if not valid, prompts user for different input.
			while(maze[goalX][goalY]=='#' || goalX<0 || goalX>nRows || goalY<0 || goalY>nCols) {
				cout << "Invalid placement of the goal position, please choose again." << endl;
				displayMaze();
				cout << "Please enter different coordinates for the goal position (x y): ";
				cin >> goalX >> goalY;
			}
			maze[goalX][goalY]='G';
			break;
		}
		default:
		{
			cout << "That is not a valid input, predefined goal defaulted.\n";
			goalX = nRows-1; goalY = nCols-1;
		}
	}

	// User inputs for the start position coordinates.
	cout << "Please enter the start position (x y): ";
	cin >> startx >> starty;
	// Checking for valid start positions, if not valid, prompts user for different input.
	while(maze[startx][starty]=='#' || startx<0 || startx>=nRows || starty<0 || starty>=nCols) {
		cout << "Invalid placement of the start position, please choose again." << endl;
		displayMaze();
		cout << "Please enter different coordinates for the start position (x y): ";
		cin >> startx >> starty;
	}

	maze[startx][starty]='S'; // Prints 'S' on maze for display purposes
	cout << "\nThe maze to be solved:";
	displayMaze();
	maze[startx][starty]='.'; // Removes 'S' from maze after displayed

	findPath(startx,starty); // Runs recursive function to solve maze
	if(maze[startx][starty]=='+') {
		cout << "\nThere exists a path from the start to the goal position!!" << endl;
		maze[startx][starty]='S';
		cout << "\nThe maze solution path looks like this: " << endl;
		cout << "Starting Position: " << "[" << startx << "," << starty << "]\n";
		// This loop outputs the movements made on the solution path:
			// the loop runs through the vector backwards,
			// since the recursive function saves the data in reverse order
		for (int i=sizePathTaken-1; i > -1; --i) {
			cout << movements[i] << ": "; // Action string
			cout << "[" << pathTaken[i][0] << "," << pathTaken[i][1] << "]" << endl;
		}
		cout << "The Goal is reached!" << endl;
		displayMaze();
		mazePathDisplay(); // This displays the maze path without wrong turns
	}
	else
		cout << " No path can be found!! :(\n" << endl;
	return 0;
}


bool obstacleCheck(int x,int y,vector<vector<char>> maze)
{
	if(maze[x][y]=='#' || maze[x][y]=='+')
		return true;
	return false;
}

bool outsideMaze(int x,int y)
{
	if(x<0 || x>=nRows || y<0 || y>=nCols) {
		return true;
	}
	return false;
}


bool goalCheck(int x,int y,int goalX,int goalY)
{
	if(x==goalX && y==goalY)
		return true;
	return false;
}


vector<int> goNorth(int x,int y)
{
	x=x-1;
	y=y;
	vector<int> north{x,y};
	return north;
}

vector<int> goSouth(int x,int y)
{
	x=x+1;
	y=y;
	vector<int> south{x,y};
	return south;
}

vector<int> goEast(int x,int y)
{
	x=x;
	y=y+1;
	vector<int> east{x,y};
	return east;
}

vector<int> goWest(int x,int y)
{
	x=x;
	y=y-1;
	vector<int> west{x,y};
	return west;
}


bool findPath(int x,int y)
{
	if(outsideMaze(x,y))
		return false;
	if(obstacleCheck(x,y,maze))
		return false;
	if(goalCheck(x,y,goalX,goalY))
		return true;

	vector<int> movePos;
	maze[x][y]='+';
	if (findPath(goNorth(x,y)[0],goNorth(x,y)[1])) {
		movePos = goNorth(x,y); // Obtain [north] position index
		pathTaken.push_back(movePos); // Add [north] position to movement vector
		sizePathTaken = pathTaken.size(); // Update size of pathTaken
		movements.push_back("Moved North"); // Add action string to vector
		return true;
	}
	if (findPath(goEast(x,y)[0],goEast(x,y)[1])) {
		movePos = goEast(x,y);
		pathTaken.push_back(movePos);
		sizePathTaken = pathTaken.size();
		movements.push_back("Moved East");
		return true;
	}
	if (findPath(goSouth(x,y)[0],goSouth(x,y)[1])) {
		movePos = goSouth(x,y);
		pathTaken.push_back(movePos);
		sizePathTaken = pathTaken.size();
		movements.push_back("Moved South");
		return true;
	}
	if (findPath(goWest(x,y)[0],goWest(x,y)[1])) {
		movePos = goWest(x,y);
		pathTaken.push_back(movePos);
		sizePathTaken = pathTaken.size();
		movements.push_back("Moved West");
		return true;
	}
	// If no movement can be made, we must backtrack.
	maze[x][y]='x';
	return false;
}


void displayMaze() {
	cout << "\n     "; // Prepares first line
	for (int j=0; j<nCols; ++j)
		cout << j << " "; // This loop displayes the column maze numbering
	cout << "\n   ---";
	for (int j=0; j<nCols; ++j)
		cout << "--"; // This loop displays the upper maze boundary based on its size
	for (int i=0; i<nRows; ++i) {
		cout << "\n " << i << " | "; // This loop displayes the row maze numbering
		for (int j=0; j<nCols; ++j)
			cout << maze[i][j] << " "; // This loop displayes the actual maze characters
		cout << "|";
	}
	cout << "\n   ---";
	for (int j=0; j<nCols; ++j)
		cout << "--"; // This loop displays the lower maze boundary based on its size
	cout << endl;
}


void mazePathDisplay() {
	for (int i=0; i<nRows; i++) {
		for (int j=0; j<nCols; j++) {
			// If the maze contains a wrong turn 'x'
			if(maze[i][j]=='x') {
				// Then replace each 'x' with a '.'
				for (int i=0; i<nRows; i++) {
					for (int j=0; j<nCols; j++) {
						if(maze[i][j]=='x')
							maze[i][j]='.';
					}
				}
				cout << "\nThe maze path displayed without wrong paths taken:";
				displayMaze();
				return;
			}
		}
	}
}
