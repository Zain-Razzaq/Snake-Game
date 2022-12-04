#include <iostream>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <ctime>
using namespace std;



#define BLACK 0
#define BROWN 6
#define WHITE 15
#define GREEN 2
#define RED 4
#define LBLUE 9

#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4


int color = 3;

void SetClr(int tcl, int bcl)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}


struct position
{
	int x;
	int y;
};

struct snake
{
	char head;
	char sym;
	position* ps;
	int size;
	int direction;
	int score;
	int color;
	int UK, DK, LK, RK;
};


void initSnake(snake &a,int row,int col)
{
	a.head = 2;
	a.sym = char(-37);
	a.size = 3;
	color+=2;
	a.color = color;
	a.ps = new position[a.size];
	for (int i = 0; i < a.size; i++)
	{
		a.ps[i].x = row / 2 + color;
		a.ps[i].y = col -70 - i;
	}
	a.direction = RIGHT;
	a.score = 0;
	if (color == 5)
	{
		a.UK = 'w';
		a.DK = 's';
		a.LK = 'a';
		a.RK = 'd';
	}
	if (color == 7)
	{
		a.UK = 't';
		a.DK = 'g';
		a.LK = 'f';
		a.RK = 'h';
	}
	if (color == 9)
	{
		a.UK = 'i';
		a.DK = 'k';
		a.LK = 'j';
		a.RK = 'l';
	}
}

void init(snake* a,int noOfSnakes, int row, int col,position &fpos,char &fsym)
{
	for (int i = 0; i < noOfSnakes; i++)
	{
		initSnake(a[i], row, col);
	}
	
	fpos.x = 40;
	fpos.y = 60;
	fsym = -37;
}


void removeSnake(snake& s)
{
	for (int i = 0; i < s.size; i++)
	{
		gotoRowCol(s.ps[i].x, s.ps[i].y);
		cout << ' ';
	}

}

void displaySnake(snake &s,int noOfSnakes)
{
	if (noOfSnakes == 1)
	{
		SetClr(1 + rand() % 10, BLACK);
	}
	else
	{
		SetClr(s.color, BLACK);
	}
	gotoRowCol(s.ps[0].x, s.ps[0].y);
	cout << s.head;
	for (int i = 1; i < s.size; i++)
	{
		gotoRowCol(s.ps[i].x, s.ps[i].y);
		cout << s.sym;
	}

}

void changeDir(snake &s, char ch)
{
	
	if(ch==s.UK)
	{
		if(s.direction!=DOWN)
			s.direction = UP;
		
	}
	else if(ch == s.DK)
	{
		if(s.direction!=UP)
			s.direction = DOWN;
		
	}
	else if(ch == s.RK)
	{
		if(s.direction!=LEFT)
			s.direction = RIGHT;
	}
	else if(ch == s.LK)
	{
		if(s.direction!=RIGHT)
			s.direction = LEFT;
	}

}

void moveSnake(snake &s,int row,int col)
{

	for (int i = s.size-1; i > 0; i--)
	{
		s.ps[i] = s.ps[i - 1];
	}

	switch (s.direction)
	{
		case UP:
		{
			s.ps[0].x--;
			if (s.ps[0].x == 1)
			{
				s.ps[0].x = row - 1;
			}
			break;
		}
		case DOWN:
		{
			s.ps[0].x++;
			if (s.ps[0].x == row)
			{
				s.ps[0].x = 2;
			}
			break;
		}
		case LEFT:
		{
			s.ps[0].y--;
			if (s.ps[0].y == 1)
			{
				s.ps[0].y = col - 1;
			}
			break;
		}
		case RIGHT:
		{
			s.ps[0].y++;
			if (s.ps[0].y == col)
			{
				s.ps[0].y = 2;
			}
			break;
		}
	}
}


void displayFood(position fpos, char fsym)
{
	gotoRowCol(fpos.x, fpos.y);
	cout << fsym;
}

bool fcapture(snake &s,position fpos)
{
	if (s.ps[0].x == fpos.x && s.ps[0].y == fpos.y)
	{
		return true;
	}
	return false;
}

void incSnake(snake &s)
{
	position* HA = new position[s.size+1]{};
	for (int i = 0; i < s.size ; i++)
	{
		HA[i+1] = s.ps[i];
	}
	HA[0] = HA[1];
	delete[] s.ps;
	s.ps = HA;
	s.size++;

}

bool inBoundries(position fpos,int row, int col)
{
	if (fpos.x > 1 && fpos.x < row)
	{
		if (fpos.y > 1 && fpos.y < col)
		{
			return true;
		}
	}
	return false;
}

bool onSnake(position fpos, snake s)
{
	for (int i = 0; i < s.size-1; i++)
	{
		if (s.ps[i].x == fpos.x)
		{
			if (s.ps[i].y == fpos.y)
			{
				return true;
			}
		}
	}
	return false;
}

bool onRisterictions(position fpos)
{
	for (int i = 2; i < 17; i++)
	{
		if (fpos.x==2 && fpos.y==i )
		{
			return 1;
		}
	}
	for (int i = 65; i < 80; i++)
	{
		if (fpos.x == 2 && fpos.y == i)
		{
			return 1;
		}
	}
	for (int i = 2; i < 17; i++)
	{
		if (fpos.x == i && (fpos.y == 2 || fpos.y==79))
		{
			return 1;
		}
	}
	for (int i = 2; i < 17; i++)
	{
		if (fpos.x == 79 && fpos.y == i)
		{
			return 1;
		}
	}
	for (int i = 65; i < 80; i++)
	{
		if (fpos.x == 79 && fpos.y == i)
		{
			return 1;
		}
	}
	for (int i = 63; i < 79; i++)
	{
		if (fpos.x == i && (fpos.y == 2 || fpos.y == 79))
		{
			return 1;
		}
	}
	for (int i = 27; i < 54; i++)
	{
		if (fpos.x == i && fpos.y == i)
		{
			return 1;
		}
	}
	for (int i = 53; i > 26; i--)
	{
		if (fpos.x == i && fpos.y ==80- i)
		{
			return 1;
		}
	}
	return 0;
}

void genFood(snake *s,int noOfSnakes,position &fpos,int row,int col)
{
	bool x;
	do
	{
		fpos.x =1+ rand()%79;
		fpos.y =1+ rand()%79;
		x = false;
		for (int i = 0; i < noOfSnakes; i++)
		{
			if (onSnake(fpos, s[i]))
			{
				x = true;
				break;
			}
		}
	} while ((!inBoundries(fpos,row,col)) || x || onRisterictions(fpos));

}

void displayRisterictions(int row, int col)
{
	SetClr(12, BLACK);
	for (int i = 2; i < 17; i++)
	{
		gotoRowCol(2, i);
		cout << char(-37);
	}
	for (int i = 65; i < 80; i++)
	{
		gotoRowCol(2, i);
		cout << char(-37);
	}
	for (int i = 2; i < 17; i++)
	{
		gotoRowCol(i, 2);
		cout << char(-37);
		gotoRowCol(i, 79);
		cout << char(-37);
	}
	for (int i = 2; i < 17; i++)
	{
		gotoRowCol(79, i);
		cout << char(-37);
	}
	for (int i = 65; i < 80; i++)
	{
		gotoRowCol(79, i);
		cout << char(-37);
	}
	for (int i = 63; i < 79; i++)
	{
		gotoRowCol(i, 2);
		cout << char(-37);
		gotoRowCol(i, 79);
		cout << char(-37);
	}
	for (int i = 27; i < 54; i++)
	{
		gotoRowCol(i, i);
		cout << char(-37);
	}
	for (int i = 53; i > 26; i--)
	{
		gotoRowCol(i, 80-i);
		cout << char(-37);
	}
}

bool snakeHit(snake* & s,int noOfSnakes,int sNo,bool* &kill)
{
	for (int i = 0; i < noOfSnakes; i++)
	{
		if (kill[i]==false)
		{
			for (int j = 0; j < s[i].size; j++)
			{
				if ((s[sNo].ps[0].x == s[i].ps[j].x) && (s[sNo].ps[0].y == s[i].ps[j].y))
				{
					if ((i != sNo) || (i == sNo && j != 0))
					{
						return true;
					}
				}
			}
		}	
	}
	return false;
}

bool killed(snake* &s,int noOfSnakes,int sNo,bool* & kill)
{
	if (onRisterictions(s[sNo].ps[0])  || snakeHit(s, noOfSnakes, sNo,kill))
	{
		return true;
	}
	return false;
}

void displayNames(int i,int color)
{
	SetClr(color, BLACK);
	gotoRowCol(5 + (i * 2), 83);
	cout << "Player " << i + 1 << " : ";
}

void displayScore(int score,int s,int color)
{
	SetClr(color, BLACK);
	gotoRowCol(5+(s*2), 94);
	cout << score;
}

int InputSpeed()
{
	gotoRowCol(0, 45);
	cout << "Enter the speed of snake (1-5) : ";
	int x;
	cin >> x;
	x = 100 / x;
	return x;
}

int main()
{
	int noOfSnakes = 0;
	cout << "Enter the number of players (max 3) : ";
	cin >> noOfSnakes;
	if (noOfSnakes>3)
	{
		noOfSnakes = 3;
	}
	int speed = InputSpeed();
	snake* s=new snake [noOfSnakes]{};
	int alive = noOfSnakes;
	position fpos;
	char fsym;
	char ch;
	bool* kill = new bool[noOfSnakes+1]{false};
	int row = 80, col = 80;
	int greaterScore,foodCap=0;
	bool largeFood = false;
	init(s,noOfSnakes,row,col,fpos,fsym);
	displayFood(fpos, fsym);
	displayRisterictions(row, col);
	for (int i = 0; i < noOfSnakes; i++)
	{
		displayNames(i,s[i].color);
		displayScore(s[i].score, i, s[i].color);
	}
	SetClr(RED,BLACK);
	gotoRowCol( 35 , 83);
	cout << "TIME : ";
	time_t  sTime = time(0);
	time_t  dTime=0, eTime=0;
	while (1)
	{
		if (_kbhit())
		{
			ch = _getch();
			for (int i = 0; i < noOfSnakes; i++)
			{
				if (ch == s[i].LK || ch == s[i].RK || ch == s[i].UK || ch == s[i].DK)
				{
					changeDir(s[i], ch);
				}
			}
			
		}
		for (int i = 0; i < noOfSnakes; i++)
		{
			
			if (kill[i] == false)
			{
				removeSnake(s[i]);
				moveSnake(s[i], row, col);
				kill[i] = killed(s,noOfSnakes,i,kill);
				if (kill[i]==true)
				{
					alive--;
				}
			}
			if (!(kill[i]))
			{
				displaySnake(s[i],noOfSnakes);
			}
			if (fcapture(s[i], fpos))
			{
				
				if (foodCap!=5)
				{
					if (largeFood)
					{
						s[i].score++;
						s[i].score++;
						largeFood = false;
					}
					else
					{
						s[i].score++;
					}
					foodCap++;
					incSnake(s[i]);
					displayScore(s[i].score, i, s[i].color);
					genFood(s, noOfSnakes, fpos, row, col);
					SetClr(GREEN, BLACK);
					displayFood(fpos, fsym);
				}
				else
				{
					s[i].score++;
					foodCap = 0;
					incSnake(s[i]);
					displayScore(s[i].score, i, s[i].color);
					genFood(s, noOfSnakes, fpos, row, col);
					SetClr(rand()%20, BLACK);
					displayFood(fpos, char(1));
					largeFood = true;
				}
			}
		}
		eTime = time(0);
		if (dTime != eTime - sTime)
		{
			dTime = eTime - sTime;
			gotoRowCol(35, 90);
			SetClr(RED, BLACK);
			cout << dTime;
		}
		if (alive==0)
		{
			greaterScore = s[0].score;
			for (int i = 0; i < noOfSnakes; i++)
			{
				if (s[i].score > greaterScore)
				{
					greaterScore = s[i].score;
				}
			}
			for (int i = 0; i < noOfSnakes; i++)
			{
				if (s[i].score == greaterScore)
				{
					SetClr(s[i].color, BLACK);
					gotoRowCol(20 + (i * 2), 83);
					cout << "Player " << i + 1 << " wins ";
				}
			}
			break;
		}
		Sleep(speed);
	}
	gotoRowCol(89, 0);
	system("pause");
	system("pause");
	return 0;
}