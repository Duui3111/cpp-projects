#include <iostream>
#include <Windows.h>
#include <time.h>
#include <conio.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];

char car[4][4] = {
    ' ', '+', '+', ' ',
    '+', '+', '+', '+',
    ' ', '+', '+', ' ',
    '+', '+', '+', '+'
};

int carPos = WIN_WIDTH/2;
int score = 0;

void gotoxy(int x, int y)
{
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size)
{
    if(size == 0) size = 20;

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder()
{
    for(int i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            gotoxy(0+j, i); cout << "+";
            gotoxy(WIN_WIDTH - j, i);  cout << "+";
        }     
    }
    for(int i = 0; i < SCREEN_HEIGHT; i++)
    {
        gotoxy(SCREEN_WIDTH, i); cout << "+";
    }
}

void genEnemy(int ind)
{
    enemyX[ind] = 17 + rand()%(33);
}

void drawEnemy(int ind)
{
    if(enemyFlag[ind] == true)
    {
        gotoxy(enemyX[ind], enemyY[ind]); cout << "**";
        gotoxy(enemyX[ind], enemyY[ind]+1); cout << "**";
        gotoxy(enemyX[ind], enemyY[ind]+2); cout << "**";
        gotoxy(enemyX[ind], enemyY[ind]+3); cout << "**";
    }
}

void eraseEnemy(int ind)
{
    if(enemyFlag[ind] == true)
    {
        gotoxy(enemyX[ind], enemyY[ind]); cout << "  ";
        gotoxy(enemyX[ind], enemyY[ind]+1); cout << "  ";
        gotoxy(enemyX[ind], enemyY[ind]+2); cout << "  ";
        gotoxy(enemyX[ind], enemyY[ind]+3); cout << "  ";
    }
}

void resetEnemy(int ind)
{
    eraseEnemy(ind);
    enemyY[ind] = 1;
    genEnemy(ind);
}

void drawCar()
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            gotoxy(j+carPos, i+22); cout << car[i][j];
        }
    }
}

void eraseCar()
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            gotoxy(j+carPos, i +22); cout << " ";
        }
    }
}

int collision()
{
    if(enemyY[0]+4 >= 23)
    {
        if(enemyX[0]+4 - carPos >= 0 && enemyX[0] +4 - carPos < 9)
        {
            return 1;
        }
    }
    return 0;
}

void gameover()
{
    system("cls");
    cout << endl;
    cout << "press any key to play agin" << endl;
    getch();
}

void updateScore()
{
    gotoxy(WIN_WIDTH + 7, 5); cout << "Score: " << score << endl;
}

void play()
{
    carPos = -1 + WIN_WIDTH/2;
    score = 0;
    enemyFlag[0] = 1;
    enemyFlag[1] = 0;
    enemyY[0] = enemyY[1] = 1;
    
    system("cls");
    drawBorder();
    updateScore();
    genEnemy(0);
    genEnemy(1);

    gotoxy(WIN_WIDTH + 7, 2); cout << "Car Game" << endl;
    gotoxy(18, 5); cout << "press any key to start" << endl;
    getch();
    gotoxy(18, 5); cout << "                      " << endl;

    while(1)
    {
        if(kbhit() == true)
        {
            char ch = getch();
            if(ch == 'a' || ch == 'A')
            {
                if(carPos > 18) carPos -=4;
            }

            else if(ch == 'd' || ch == 'D')
            {
                if(carPos < 50) carPos += 4;
            }
            else if(ch == 27)
            {
                break;
            }
        }

        drawCar();
        drawEnemy(0);
        drawEnemy(1);
        if(collision() == 1)
        {
            gameover();
            return;
        }

        Sleep(50);
        eraseCar();
        eraseEnemy(0);
        eraseEnemy(1);

        if(enemyY[0] == 10)
            if(enemyFlag[1] == 0) 
                enemyY[0] +=1;
        
        if(enemyFlag[0] == 1) 
            enemyY[0] +=1;
        
        if(enemyFlag[1] == 1) 
            enemyY[1] +=1;

        if(enemyY[0] > SCREEN_HEIGHT-4)
        {
            resetEnemy(0);
            score++;
            updateScore();
        }

        if(enemyY[1] > SCREEN_HEIGHT-4)
        {
            resetEnemy(1);
            score++;
            updateScore();
        }     
    }
}

int main()
{
    setcursor(0, 0);
    srand((unsigned)time(NULL));

    do
    {
       system("cls");
       play();

    }
    while(1);
    
    return 0;
}
