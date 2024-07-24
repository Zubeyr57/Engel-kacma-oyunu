#include <iostream>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

class Character {
    public:
    int x = 15;
    int y = 12;
    unsigned char tus;
    int puan = 0;
    int durum = 1;
    int yon = 1;
};

class Game {
    public:
    int hiz = 0;
    int katsayi = 0;
    int cerceve_boyut[80][23] = {0};
};

Game game;
Character character;                    
                                     

void hidecursor() {
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void gotoxy(short x, short y) {
    HANDLE hConsoleOutput;
    COORD Cursor_Pos;
    Cursor_Pos.X = static_cast<SHORT>(x - 1);
    Cursor_Pos.Y = static_cast<SHORT>(y - 1);
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_Pos);
}


void cerceve(int x1, int y1, int x2, int y2, int certip) {
    int i;
    for (i = x1; i <= x2; i++) {
        game.cerceve_boyut[i][y1] = certip;
        game.cerceve_boyut[i][y2] = certip;
    }
    for (i = y1; i <= y2; i++) {
        game.cerceve_boyut[x1][i] = certip;
        game.cerceve_boyut[x2][i] = certip;
    }
}

void ekrani_bas() {
    int x, y, i;
    for (x = 0; x < 80; x++) {
        for (y = 0; y < 23; y++) {
            gotoxy(x + 1, y + 1);
            if (game.cerceve_boyut[x][y] == 1) {
                cout << (char)219;
            }
        }
    }
}

struct Obstacle {
    int col;
    int gap_row;
};

Obstacle obstacles[50];
int obstacle_count = 0;

void generateObstacle() {
    obstacles[obstacle_count].col = 30 + rand() % 35;
    obstacles[obstacle_count].gap_row = 4 + rand() % 10;
    obstacle_count++;
}

void printObstacles() {
    for (int n = 0; n < obstacle_count; n++) {
        for (int i = 2; i < 16; i++) {
            if (i != obstacles[n].gap_row) {
                gotoxy(obstacles[n].col, i);
                cout << (char)176;
            }
        }
    }
}

void deleteObstacles() {
    for (int n = 0; n < obstacle_count; n++) {
        for (int i = 2; i < 16; i++) {
            gotoxy(obstacles[n].col, i);
            cout << " ";
        }
    }
}

int Menu() {
    int x = 0;
    cout << "1-4 arasi karakter hizi girin: ";
    cin >> x;
    switch (x) {
        case 1:
            game.hiz = 100;
            game.katsayi = 1;
            break;
        case 2:
            game.hiz = 75;
            game.katsayi = 3;
            break;
        case 3:
            game.hiz = 50;
            game.katsayi = 6;
            break;
        case 4:
            game.hiz = 25;
            game.katsayi = 10;
    }
    return 0;
}

bool checkCollision() {
    for (int n = 0; n < obstacle_count; n++) {
        if (character.x == obstacles[n].col && character.y != obstacles[n].gap_row) {
            return true;
        }
    }
    return false;
}

int main() {
    Menu();
    cerceve(0, 0, 79, 15, 1);
    ekrani_bas();
    hidecursor();
    srand(time(NULL));
    generateObstacle();
    printObstacles();

    do {
        gotoxy(character.x, character.y);
        cout << (char)1;

        Sleep(game.hiz - character.puan);
        gotoxy(character.x, character.y);
        cout << " ";

        gotoxy(2, 2);
        cout << "Puaniniz: " << character.puan;

        if (kbhit()) {
            character.tus = getch();
            if (character.tus == 224) {
                character.tus = getch();
                switch (character.tus) {
                    case 72:
                        character.y--;
                        break;
                    case 77:
                        character.x++;
                        break;
                    case 75:
                        character.x--;
                        break;
                    case 80:
                        character.y++;
                }
            }
        }

        character.x = character.x + character.yon;

        if (checkCollision()) {
            gotoxy(40, 12);
            cout << "------------------";
            gotoxy(40, 13);
            cout << "-----YANDINIZ-----";
            gotoxy(40, 14);
            cout << "------------------";
            Sleep(1000);
            character.durum = 0;
        }

        if (character.x == obstacles[obstacle_count-1].col && character.y == obstacles[obstacle_count-1].gap_row) {
            character.puan += game.katsayi;
        }

        if (character.x > 77) {
            deleteObstacles();
            generateObstacle();
            printObstacles();
            character.yon = -1;
        }
        if (character.x < 4) {
            deleteObstacles();
            generateObstacle();
            printObstacles();
            character.yon = 1;
        }
        if (character.y > 15)
            character.y = 2;
        if (character.y < 2)
            character.y = 15;

    } while (character.durum);

    return 0;
}
