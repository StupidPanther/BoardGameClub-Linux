#include "ref.h"

void operator++(user_t& usr, int)
{
	if (usr == BLACK)
		usr = WHITE;
	else if (usr == WHITE)
		usr = BLACK;
}

ostream& operator<<(ostream& out, user_t usr)
{
	if (usr == BLACK)
		out << "BLACK ●";
	else if (usr == WHITE)
		out << "WHITE ○";
	else
		out << 2;
	return out;
}

istream& operator>>(istream& in, user_t usr)
{
	int num;
	in >> num;
	if (num == 0)
		usr = BLACK;
	else if (num == 1)
		usr = WHITE;
	else
		usr = EMPTY;
	return in;
}

position_t::position_t()
{
	row = 0;
	column = 0;
}

position_t::position_t(unsigned int r, unsigned int c)
{
	row = r;
	column = c;
}

void position_t::operator++()
{
	if (row < ROW_COUNT - 1)
		row++;
}

void position_t::operator++(int)
{
	if (column < COLUMN_COUNT - 1)
		column++;
}

void position_t::operator--()
{
	if (row > 0)
		row--;
}

void position_t::operator--(int)
{
	if (column > 0)
		column--;
}

int _getch()
{
     struct termios tm, tm_old;
     int fd = 0, ch;

     if (tcgetattr(fd, &tm) < 0) {//保存现在的终端设置
          return -1;
     }

     tm_old = tm;
     cfmakeraw(&tm);//更改终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理
     if (tcsetattr(fd, TCSANOW, &tm) < 0) {//设置上更改之后的设置
          return -1;
     }

     ch = getchar();
     if (tcsetattr(fd, TCSANOW, &tm_old) < 0) {//更改设置为最初的样子
          return -1;
     }

     return ch;
}
