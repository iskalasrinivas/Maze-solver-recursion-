#include <iostream>
#include <string>
#include <vector>
using namespace std;

// function prototypes
vector<int> goSouth(int,int);
vector<int> goNorth(int,int);
bool findPath(int,int);
bool obstacleCheck(int,int,vector<vector<char>>,size_t size);
bool outsideMaze(int ,int);
bool goalCheck(int,int,int,int);
void markPath(int,int,vector<vector<char>> &maze,size_t size);
void unmarkPath(int,int,vector<vector<char>> &maze,size_t size);
	int goalx{};    // initialised variables to store x and y positions of goal
	int goaly{};
	vector<vector<char>> maze {
		{'#','#','#','#','#','#'},
		{'.','.','.','.','.','#'},
		{'#','.', '#','#','#','#'},
		{'#','.', '#','#','.','#'},
		{'.','.', '.','#','.','.'},
		{'#','#', '.','.','.','#'},
		};
		
int main()
{
	int startx;    // initialised variables to store x and y positions of start
	int starty;
	// user inputs
	cout<<"please enter the start position: "<<endl;   // the start coordinates
	cin>>starty>>startx;
	
	// Checking for valid start and goal positions
	while(maze[startx][starty]=='#'||startx<0||startx>5||starty<0||starty>5){
		cout<<" Invalid placement of the start position, here is the maze:"<<endl;
		for(int i=0;i<6;i++){
			for(int j=0;j<6;j++)
			{
				cout<<maze[i][j]<<" " ;
			}
		cout<<endl;
		  }
		cout<< "Please enter different coordinates for the start position:"<<endl;
		cin>>starty>>startx;
	}
	cout<<"please enter the goal position: "<<endl;     // the end coordinates
	cin>>goaly>>goalx;
	while(maze[goalx][goaly]=='#'||goalx<0||goalx>5||goaly<0||goaly>5){
		cout<<" Invalid placement of the goal position, here is the maze:"<<endl;
		for(int i=0;i<6;i++){
			for(int j=0;j<6;j++)
			{
				cout<<maze[i][j]<<" " ;
			}
		cout<<endl;
		  }
		cout<< "Please enter different coordinates for the goal position:"<<endl;
		cin>>goaly>>goalx;
	}
	
	maze[startx][starty]='S';
	maze[goalx][goaly]='G';
	cout<<"The maze looks like this: "<<endl;
	for(int i=0;i<6;i++){
		for(int j=0;j<6;j++)
			{
				cout<<maze[i][j]<<" " ;
			}
		cout<<endl;
		  }
	findPath(startx,starty);
	if(maze[startx][starty]=='+')
		cout<<"There exists a path from the start to the goal position!!"<<endl;
	else 
		cout<<" No path has been found!!"<<endl;
}
// obstacle check function

bool obstacleCheck(int x,int y,vector<vector<char>> maze,size_t size)
{
	if(maze[x][y]=='#'||maze[x][y]=='+')
		return true;
	
	return false;
}
// outside maze

bool outsideMaze(int x,int y)
{
	if(x<0||x>5||y<0||y>5){
		return true;
	}
	return false;
}

// goalchecking function

bool goalCheck(int x,int y,int goalx,int goaly)
{
	if(x==goalx && y==goaly)
	{
		cout<<"the solution path."<<endl;
		for(int i=0;i<6;i++){
			for(int j=0;j<6;j++)
			{
				cout<<maze[i][j]<<" " ;
			}
		cout<<endl;
		  }
		cout << " goal is reached"<<endl;
		return true;
	}
	return false;
}

// marking the path

void markPath(int x,int y,vector<vector<char>> &maze,size_t size)
{
	maze[x][y]='+';
}
// unmarking the path

void unmarkPath(int x,int y,vector<vector<char>> &maze,size_t size)
{
	maze[x][y]='X';
}




// go north function
vector<int> goNorth(int x,int y)
{

		x=x;
		y=y-1;
		vector<int> north{x,y};
		return north;
}

// go south

vector<int> goSouth(int x,int y)
{

		x=x;
		y=y+1;
		vector<int> south{x,y};
		return south;
	}

// going east

vector<int> goEast(int x,int y)
{

		x=x+1;
		y=y;
		vector<int> east{x,y};
		return east;
}

// going west

vector<int> goWest(int x,int y)
{

		x=x-1;
		y=y;
		vector<int> west{x,y};
		return west;

}





// findpath recursive function

bool findPath(int x,int y)
{
	if(outsideMaze(x,y))
	{ return false;
	}
	if(goalCheck(x,y,goalx,goaly)){
		return true;
	}
	if(obstacleCheck(x,y,maze,maze.size())) {
		return false;
	}
	
	markPath(x,y,maze,maze.size());
	if (findPath(goNorth(x,y)[0],goNorth(x,y)[1])){
		return true;
	}
	if (findPath(goEast(x,y)[0],goEast(x,y)[1])){
		return true;
	}
	if (findPath(goSouth(x,y)[0],goSouth(x,y)[1])){
		return true;
	}
	if (findPath(goWest(x,y)[0],goWest(x,y)[1])){
		return true;
	}
	unmarkPath(x,y,maze,maze.size());
	
	return false;
}
