#include <iostream>
#include <map>
#include <cstdlib>
#include <conio.h>
#include <cmath>
#include <vector>
#include <ctime>

using namespace std;

const int boardX = 40, boardY = 20;
int center = ceil ((boardX/2*boardY)-(boardX/2));

map<int, char> gameMap;
map<int, char>::const_iterator it;

int snakeXY = center;
enum snakeDir {UP, DOWN, LEFT, RIGHT};
snakeDir direction;
vector<int> tailPieces;

bool Spawned = false;
bool gameOver;

void createMap()
{
    for(int i = 1; boardX*boardY>=i; i++)
    { 
        if((i<=boardX) || (i>(boardX*boardY-boardX))) gameMap[i] = '#';
        else if((i % boardX == 0) || ((i-1) % boardX == 0)) gameMap[i] = '#';
        else gameMap[i] = ' ';     
    }
}

void keyUpdate()
{
    if(kbhit() == true)
    {
        char key = getch();
        if(key == 'w') direction = UP; 
        else if(key == 'a') direction = LEFT;
        else if(key == 's') direction = DOWN;
        else if(key == 'd') direction = RIGHT;
    }
}

void updateSnake()
{
    int prevXY = snakeXY;
    if(direction == UP) snakeXY = snakeXY - boardX;
    else if(direction == DOWN) snakeXY = snakeXY + boardX;
    else if(direction == LEFT) --snakeXY;
    else if(direction == RIGHT) ++snakeXY;

    if(tailPieces.empty()) gameMap[prevXY] = ' ';
     
    else 
    {
        int tailPieceXY;
        for(int i = 0; i <= tailPieces.size(); i++)
        {
            if (i == tailPieces.size()) gameMap[prevXY] = ' ';      
            else 
            {
                tailPieceXY = tailPieces[i];
                tailPieces[i] = prevXY;
                gameMap[prevXY] = 'o';
                prevXY = tailPieceXY;
            }
        }
    }
    if(gameMap[snakeXY] == '#' || gameMap[snakeXY] == 'o') gameOver = true;
    else if(gameMap[snakeXY] == '$') 
    {
        tailPieces.push_back(prevXY);
        gameMap[snakeXY] = 'O';
        Spawned = false;
    } 
    else gameMap[snakeXY] = 'O';  
}

void updateMap()
{
    system("cls");
    for (it = gameMap.begin(); it!= gameMap.end(); it++)
    {
        cout << it -> second;
        if(it -> first % boardX == 0) cout << endl;    
    }
}

void spawn()
{
    vector<int> safeSpace;
    for (it = gameMap.begin(); it!= gameMap.end(); it++)
    if(it->second == ' ') safeSpace.push_back(it->first);      
    srand(time(NULL)); 
    int random = rand() % safeSpace.size();
    gameMap[safeSpace[random]] = '$';
    Spawned = true;
}

int main()
{
    createMap();
    gameOver = false;

    while(!gameOver)
    {
        updateMap();
        updateSnake();
        if(!Spawned) spawn();
        keyUpdate();
        _sleep(125);
    }

    return 0;
}
