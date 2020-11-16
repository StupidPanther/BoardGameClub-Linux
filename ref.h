#ifndef __REF_H__
#define __REF_H__

#define ROW_COUNT 8
#define COLUMN_COUNT 8
#define CMD_HEIGHT 25
#define CMD_WIDTH 80
#include <iostream>
#include <termio.h>
using namespace std;
/*
constexpr unsigned int ROW_COUNT = 8;
constexpr unsigned int COLUMN_COUNT = 8;
constexpr unsigned int CMD_HEIGHT = 25;
constexpr unsigned int CMD_WIDTH = 80;
*/

enum gametype_t
{
    GOBANG,
    FLIPCHESS,
    IDLE
};

enum user_t
{
    BLACK,
    WHITE,
    EMPTY
};

void operator++(user_t& usr, int);
ostream& operator<<(ostream& out, user_t usr);
istream& operator>>(istream& in, user_t usr);

enum movestatus_t
{
    SUCCESS,
    OCCUPIED,
    INVALID
};

enum cursor_t
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef user_t plainboard_t[ROW_COUNT][COLUMN_COUNT];

class position_t
{
public:
    unsigned int row;
    unsigned int column;

    position_t();
    position_t(unsigned int r, unsigned int c);

    void operator++();
    void operator++(int);
    void operator--();
    void operator--(int);
};

int _getch();

#endif