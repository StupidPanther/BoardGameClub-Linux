#include "board.h"

Board::Board()
{
	Init();
}

Board::~Board()
{

}

void Board::Init()
{
	for (unsigned int i = 0; i < ROW_COUNT; i++)
	{
		for (unsigned int j = 0; j < COLUMN_COUNT; j++)
		{
			plainboard[i][j] = EMPTY;
			plainboard_regret[i][j] = EMPTY;
		}
	}
	position.row = (ROW_COUNT - 1) / 2;
	position.column = (COLUMN_COUNT - 1) / 2;
}

void Board::MoveCursor(cursor_t cursor)
{
	if (cursor == UP)
		--position;
	else if (cursor == DOWN)
		++position;
	else if (cursor == LEFT)
		position--;
	else if (cursor == RIGHT)
		position++;
}

movestatus_t Board::Move(user_t moveuser)
{
	for (unsigned int i = 0; i < ROW_COUNT; i++)
	{
		for (unsigned int j = 0; j < COLUMN_COUNT; j++)
		{
			plainboard_regret[i][j] = plainboard[i][j];
		}
	}
	plainboard[position.row][position.column] = moveuser;
	return SUCCESS;
}

void Board::Regret()
{
	for (unsigned int i = 0; i < ROW_COUNT; i++)
	{
		for (unsigned int j = 0; j < COLUMN_COUNT; j++)
		{
			plainboard[i][j] = plainboard_regret[i][j];
		}
	}
}

const plainboard_t& Board::GetPlainBoard() const
{
	return plainboard;
}

position_t Board::GetCursorPosition() const
{
	return position;
}

plainboard_t& Board::SetPlainBoard()
{
	return plainboard;
}

position_t& Board::SetCursorPosition()
{
	return position;
}