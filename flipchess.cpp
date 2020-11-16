#include "flipchess.h"
#include <sstream>
using namespace std;

void Flipchess::Init(plainboard_t& pb)
{
	pb[(ROW_COUNT - 1) / 2][(COLUMN_COUNT - 1) / 2] = BLACK;
	pb[(ROW_COUNT - 1) / 2 + 1][(COLUMN_COUNT - 1) / 2] = WHITE;
	pb[(ROW_COUNT - 1) / 2][(COLUMN_COUNT - 1) / 2 + 1] = WHITE;
	pb[(ROW_COUNT - 1) / 2 + 1][(COLUMN_COUNT - 1) / 2 + 1] = BLACK;
}

movestatus_t Flipchess::IfMoveValid(const plainboard_t pb, const user_t user, const position_t pos)
{
	if (pb[pos.row][pos.column] != EMPTY)
		return OCCUPIED;
	for (int i = 0; i < 8; i++)
	{
		position_t pos_side = pos;
		user_t oppo = user;
		oppo++;
		bool meet_edge = false;
		bool meet_oppo = false;
		do{
			switch (i)
			{
			case 0:if (!(pos_side.row > 0 && pos_side.column < COLUMN_COUNT - 1))meet_edge = true; break;
			case 1:if (!(pos_side.row > 0))meet_edge = true; break;
			case 2:if (!(pos_side.row > 0 && pos_side.column > 0))meet_edge = true; break;
			case 3:if (!(pos_side.column > 0))meet_edge = true; break;
			case 4:if (!(pos_side.row < ROW_COUNT - 1 && pos_side.column>0))meet_edge = true; break;
			case 5:if (!(pos_side.row < ROW_COUNT - 1))meet_edge = true; break;
			case 6:if (!(pos_side.row < ROW_COUNT - 1 && pos_side.column < COLUMN_COUNT - 1))meet_edge = true; break;
			case 7:if (!(pos_side.column < COLUMN_COUNT - 1))meet_edge = true; break;
			}
			switch (i)
			{
			case 0:--pos_side, pos_side++; break;
			case 1:--pos_side; break;
			case 2:--pos_side, pos_side--; break;
			case 3:pos_side--; break;
			case 4:++pos_side, pos_side--; break;
			case 5:++pos_side; break;
			case 6:++pos_side, pos_side++; break;
			case 7:pos_side++; break;
			}
			if (pb[pos_side.row][pos_side.column] == oppo)
				meet_oppo = true;
		} while (pb[pos_side.row][pos_side.column] == oppo && meet_edge==false);
		if (pb[pos_side.row][pos_side.column] == user && meet_oppo==true && meet_edge==false)
			return SUCCESS;
	}
	return INVALID;
}

void Flipchess::Flip(plainboard_t& pb, const position_t pos)
{
	for (int i = 0; i < 8; i++)
	{
		position_t pos_side = pos;
		user_t oppo = pb[pos.row][pos.column];
		oppo++;
		bool meet_edge = false;
		bool could_flip = false;
		do {
			switch (i)
			{
			case 0:if (!(pos_side.row > 0 && pos_side.column < COLUMN_COUNT - 1))meet_edge = true; break;
			case 1:if (!(pos_side.row > 0))meet_edge = true; break;
			case 2:if (!(pos_side.row > 0 && pos_side.column > 0))meet_edge = true; break;
			case 3:if (!(pos_side.column > 0))meet_edge = true; break;
			case 4:if (!(pos_side.row < ROW_COUNT - 1 && pos_side.column>0))meet_edge = true; break;
			case 5:if (!(pos_side.row < ROW_COUNT - 1))meet_edge = true; break;
			case 6:if (!(pos_side.row < ROW_COUNT - 1 && pos_side.column < COLUMN_COUNT - 1))meet_edge = true; break;
			case 7:if (!(pos_side.column < COLUMN_COUNT - 1))meet_edge = true; break;
			}
			switch (i)
			{
			case 0:--pos_side, pos_side++; break;
			case 1:--pos_side; break;
			case 2:--pos_side, pos_side--; break;
			case 3:pos_side--; break;
			case 4:++pos_side, pos_side--; break;
			case 5:++pos_side; break;
			case 6:++pos_side, pos_side++; break;
			case 7:pos_side++; break;
			}
			if (meet_edge == false && pb[pos_side.row][pos_side.column] == pb[pos.row][pos.column])
			{
				could_flip = true;
				break;
			}
			if (meet_edge == true || pb[pos_side.row][pos_side.column]==EMPTY)
			{
				could_flip = false;
				break;
			}
		} while (true);
		if (could_flip == false)
			continue;
		pos_side = pos;
		meet_edge = false;
		do {
			switch (i)
			{
			case 0:if (!(pos_side.row > 0 && pos_side.column < COLUMN_COUNT - 1))meet_edge = true; break;
			case 1:if (!(pos_side.row > 0))meet_edge = true; break;
			case 2:if (!(pos_side.row > 0 && pos_side.column > 0))meet_edge = true; break;
			case 3:if (!(pos_side.column > 0))meet_edge = true; break;
			case 4:if (!(pos_side.row < ROW_COUNT - 1 && pos_side.column>0))meet_edge = true; break;
			case 5:if (!(pos_side.row < ROW_COUNT - 1))meet_edge = true; break;
			case 6:if (!(pos_side.row < ROW_COUNT - 1 && pos_side.column < COLUMN_COUNT - 1))meet_edge = true; break;
			case 7:if (!(pos_side.column < COLUMN_COUNT - 1))meet_edge = true; break;
			}
			switch (i)
			{
			case 0:--pos_side, pos_side++; break;
			case 1:--pos_side; break;
			case 2:--pos_side, pos_side--; break;
			case 3:pos_side--; break;
			case 4:++pos_side, pos_side--; break;
			case 5:++pos_side; break;
			case 6:++pos_side, pos_side++; break;
			case 7:pos_side++; break;
			}
			if (pb[pos_side.row][pos_side.column] != oppo || meet_edge == true)
				break;
			pb[pos_side.row][pos_side.column]++;
		} while (true);
	}
}

string Flipchess::CurStatus(const plainboard_t pb)
{
	unsigned int black_count = 0;
	unsigned int white_count = 0;
	for (unsigned int i = 0; i < ROW_COUNT; i++)
	{
		for (unsigned int j = 0; j < COLUMN_COUNT; j++)
		{
			if (pb[i][j] == BLACK)
				black_count++;
			else if (pb[i][j] == WHITE)
				white_count++;
		}
	}
	stringstream ss;
	string ret;
	string temp;
	ret += "BLACK●: ";
	ss << black_count;
	ss >> temp;
	ret += temp;
	ss.clear();
	ret += "  WHITE○: ";
	ss << white_count;
	ss >> temp;
	ret += temp;
	return ret;
}

bool Flipchess::IfStop(const plainboard_t pb)
{
	if (CouldMove(pb, BLACK) == false && CouldMove(pb, WHITE) == false)
		return true;
	else
		return false;
}

user_t Flipchess::WhoWin(const plainboard_t pb)
{
	if (CouldMove(pb, BLACK) == false && CouldMove(pb, WHITE) == false)
	{
		unsigned int black_count = 0;
		unsigned int white_count = 0;
		for (unsigned int i = 0; i < ROW_COUNT; i++)
		{
			for (unsigned int j = 0; j < COLUMN_COUNT; j++)
			{
				if (pb[i][j] == BLACK)
					black_count++;
				else if (pb[i][j] == WHITE)
					white_count++;
			}
		}
		if (black_count > white_count)
			return BLACK;
		else if (black_count < white_count)
			return WHITE;
		else
			return EMPTY;
	}
	else
		return EMPTY;
}

bool Flipchess::CouldMove(const plainboard_t pb, user_t usr)
{
	for (unsigned int i = 0; i < ROW_COUNT; i++)
	{
		for (unsigned int j = 0; j < COLUMN_COUNT; j++)
		{
			if (IfMoveValid(pb, usr, position_t(i, j)) == SUCCESS)
				return true;
		}
	}
	return false;
}
