
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
// All the elements to be used
// Declared here
#define WIDTH 50
#define HEIGHT 30
#define PACMAN 'P'
#define WALL '+'
#define FOOD '*'
#define EMPTY ' '
#define DEMON '@'
#define ENEMY 'E'
#define ENEMIES 3
#define DOUBLE '$'
// Global Variables are
// Declared here
int res = 0;
int score = 0;
int pacman_x, pacman_y;
char board[HEIGHT][WIDTH];
int food = 0;
int curr = 0;
int enemy_x[ENEMIES] , enemy_y[ENEMIES];
int doubleCount=0;
int double_x,double_y;
struct structBoard {
    char typeCell;
};
struct Game{
    int score;
    int food;
    int curr;
    struct structBoard board[HEIGHT][WIDTH];
	int x;
	int y;
};
void save()
{
    struct Game Save;
    FILE *save = fopen("F:/PACMAN/save.bin", "wb");
    if (save == NULL)
    {
        printf("error!");
        return;
    }
    Save.score = score;
    Save.food = food;
    Save.curr = curr;
    Save.x = pacman_x;
    Save.y = pacman_y;
    for (int i=0;i<HEIGHT;i++)
    {
        for (int j=0; j<WIDTH;j++)
	    {
			Save.board[i][j].typeCell = board[i][j];
        }
    }
    fwrite(&Save,sizeof(struct Game),1,save);
    fclose(save);
    printf("save successfully");
}
void load()
{
	struct Game Save;
	FILE *save= fopen("F:/PACMAN/save.bin", "rb");
    if (save == NULL) 
	{
        printf("error!");
        return;
	}
	fread(&Save,sizeof(struct Game),1,save);
	score=Save.score;
	food=Save.food;
	curr=Save.curr;
	pacman_x=Save.x;
	pacman_y=Save.y;
	for(int i=0;i<HEIGHT;i++)
    {
        for(int j=0; j<WIDTH;j++)
	    {
          board[i][j]=Save.board[i][j].typeCell;
        }
    }
	 fclose(save);
}
void initialize()
{
	// Putting Walls as boundary in the Game
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (i == 0 || j == WIDTH - 1 || j == 0 || i == HEIGHT - 1)
		    {
				board[i][j] = WALL;
			}
			else
				board[i][j] = EMPTY;
		}
	}
	// Putting Walls inside the Game
	int count = 50;
	while (count != 0) {
		int i = (rand() % (HEIGHT + 1));
		int j = (rand() % (WIDTH + 1));
		if (board[i][j] != WALL && board[i][j] != PACMAN) 
		{
			board[i][j] = WALL;
			count--;
		}
	}
	int val = 5;
	while (val--) {
		int row = (rand() % (HEIGHT + 1));
		for (int j = 3; j < WIDTH - 3; j++) {
			if (board[row][j] != WALL && board[row][j] != PACMAN)
		    {
				board[row][j] = WALL;
			}
		}
	}
	// Putting Demons in the Game
	count = 25;
	while (count != 0) {
		int i = (rand() % (HEIGHT + 1));
		int j = (rand() % (WIDTH + 1));
		if (board[i][j] != WALL && board[i][j] != PACMAN) {
			board[i][j] = DEMON;
			count--;
		}
	}
	// Cursor at Center
	pacman_x = WIDTH / 2;
	pacman_y = HEIGHT / 2;
	board[pacman_y][pacman_x] = PACMAN;
	// Points Placed
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (i % 2 == 0 && j % 2 == 0 && board[i][j] != WALL && board[i][j] != DEMON && board[i][j] != PACMAN) 
			{
				board[i][j] = FOOD;
				food++;
			}
		}
	}
	//Enemy position
	srand(time(0));
	for(int i=0;i<ENEMIES;i++)
	{
      enemy_x[i]=rand()%WIDTH+1;
	  enemy_y[i]=rand()%HEIGHT+1;
	  board[enemy_y[i]][enemy_x[i]]=ENEMY;
	}
    //Double position
	double_x =rand()%WIDTH+1;
    double_y =rand()%HEIGHT+1;
    board[double_y][double_x]=DOUBLE;
}
void draw()
{
	// Clear screen
	system("cls");
	// Drawing All the elements in the screen
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
	printf("Score: %d\n", score);
}
// Function enables to move the Cursor
void move(int move_x, int move_y)
{
	if(doubleCount>0)
	{
       move_x+=2;
	   move_y+=2;
	   doubleCount--;
	}
	int x = pacman_x + move_x;
	int y = pacman_y + move_y;
	if (board[y][x] != WALL) {
		if (board[y][x] == FOOD) {
			score++;
			food--;
			curr++;
			if (food == 0) {
				res = 2;
				return;
			}
		}
		else if (board[y][x] == DEMON) {
			res = 1;
		}
		else if(board[y][x]==DOUBLE)
		{
			doubleCount=10;
			board[double_y][double_x] = EMPTY;
		}
		board[pacman_y][pacman_x] = EMPTY;
		pacman_x = x;
		pacman_y = y;
		board[pacman_y][pacman_x] = PACMAN;
	}
}
void enemyMove()
{
	for(int i=0;i<ENEMIES;i++)
  {
	int x = enemy_x[i] +(rand()%11-5);
	int y = enemy_y[i] +(rand()%11-5);
	if(x>0 && x<WIDTH && y>0 && y<HEIGHT)
	{
		if(board[y][x]==PACMAN)
	   {
		 res=1;
	   }
	    else if (board[y][x]!=WALL)
	   {
	     board[enemy_y[i]][enemy_x[i]] = EMPTY;
         enemy_x[i] = x;
         enemy_y [i]= y;
         board[enemy_y[i]][enemy_x[i]] = ENEMY;
	   }
	}
  }
}
void randMove()
{
	srand(time(0));
	int randMove=rand()%9+1;
    switch (randMove)
    {
        case 8:
		{
           move(0, -1);
		   break; 
		} 
        case 2: 
		{
           move(0, 1); 
		   break;   
		}
        case 4: 
		{
           move(-1, 0);
		   break; 
		}
        case 6: 
		{
			move(1, 0);
		    break;  
		}
	}
}
// Main Function
int main()
{
	printf("Do you want to continue?(y/n)\n");
	char choice;
	choice=getch();
	if(choice=='y'||choice=='Y')
	{
		load();
	}
	initialize();
	char ch;
	food -= 35;
	int totalFood = food;
	// Instructions to Play
	printf(" Use buttons for 8(up), 4(left) , 6(right) and ""2(down)\nAlso, Press q for quit\n");
	printf("Enter Y to continue: \n");
	ch = getch();
	if (ch != 'Y' && ch != 'y') {
		printf("Exit Game! ");
		return 1;
	}
	while (1) {

		draw();
		printf("Total Food count: %d\n", totalFood);
		printf("Total Food eaten: %d\n", curr);
		if (doubleCount > 0) 
	   {
          printf("Double Moves:%d",doubleCount);
	   }
		if (res == 1) {
			// Clear screen
			system("cls");
			printf("Game Over! Dead by Demon\n Your Score: ""%d\n",score);
			return 1;
		}
		if (res == 2) {
			// Clear screen
			system("cls");
			printf("You Win! \n Your Score: %d\n", score);
			return 1;
		}
		enemyMove(enemy_x,enemy_y);
		
		if(kbhit())
		{
             ch = getch();
			 
		     switch (ch) {
		        case '8':
		        	move(0, -1);
			        break;
		        case '2':
			        move(0, 1);
			        break;
		        case '4':
			       move(-1, 0);
			       break;
		        case '6':
			       move(1, 0);
			       break;
		        case 'q':
		           printf("Do you want save?(y/n)\n");
			       char select;
			       select=getch();
			       if(select=='y'||select=='Y')
                   {
				      save();
				      return 0;
			       }
			       else if(select=='n'||select=='N')
			       {
                      printf("Game Over! Your Score: %d\n", score);
			          return 0;
			       }
			       else
			       {
				      printf("error,try again\n");
			       }
				default:
				   exit(1);
		        }
		}else{
			Sleep(1000);
			randMove();
		}
	
	}
	return 0;
}
