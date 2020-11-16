#include "gobang.h"

movestatus_t Gobang::IfMoveValid(const plainboard_t pb, const user_t user, position_t pos)
{
	if (pb[pos.row][pos.column] == EMPTY)
		return SUCCESS;
	else
		return OCCUPIED;
}

user_t Gobang::WhoWin(const plainboard_t pb)
{
	// row oriented
	for (position_t begin(0, 0); begin.row < ROW_COUNT; ++begin)
	{
		for (position_t trigger = begin; trigger.column < COLUMN_COUNT;)
		{
			position_t vernier;
			for (vernier = trigger, vernier++; pb[vernier.row][vernier.column] == pb[trigger.row][trigger.column]&& pb[trigger.row][trigger.column]!=EMPTY; vernier++)
			{
				if (vernier.column - trigger.column == 4 )
					return pb[trigger.row][trigger.column];
				if (vernier.column == COLUMN_COUNT - 1)
					break;
			}
			if (trigger.column == COLUMN_COUNT - 1)
				break;
			trigger = vernier;
		}
		if (begin.row == ROW_COUNT - 1)
			break;
	}
	// column oriented
	for (position_t begin(0, 0); begin.column < COLUMN_COUNT; begin++)
	{
		for (position_t trigger = begin; trigger.row < ROW_COUNT;)
		{
			position_t vernier;
			for (vernier = trigger, ++vernier; pb[vernier.row][vernier.column] == pb[trigger.row][trigger.column] && pb[trigger.row][trigger.column] != EMPTY; ++vernier)
			{
				if (vernier.row - trigger.row == 4)
					return pb[trigger.row][trigger.column];
				if (vernier.row == ROW_COUNT - 1)
					break;
			}
			if (trigger.row == ROW_COUNT - 1)
				break;
			trigger = vernier;
		}
		if (begin.column == COLUMN_COUNT - 1)
			break;
	}
	// diagonally up oriented
	for (position_t begin(0, 0); begin.row<ROW_COUNT && begin.column < COLUMN_COUNT; (begin.row==ROW_COUNT-1)?begin++:++begin)
	{
		for (position_t trigger = begin; trigger.row < ROW_COUNT && trigger.column<COLUMN_COUNT;)
		{
			position_t vernier;
			for (vernier = trigger, --vernier, vernier++; pb[vernier.row][vernier.column] == pb[trigger.row][trigger.column] && pb[trigger.row][trigger.column] != EMPTY; --vernier, vernier++)
			{
				if (vernier.column - trigger.column == 4)
					return pb[trigger.row][trigger.column];
				if (vernier.row == 0 || vernier.column == COLUMN_COUNT - 1)
					break;
			}
			if (trigger.row == 0 || trigger.column == COLUMN_COUNT - 1)
				break;
			trigger = vernier;
		}
		if (begin.column == COLUMN_COUNT - 1)
			break;
	}
	// diagonally down oriented
	for (position_t begin(0, COLUMN_COUNT-1); begin.row < ROW_COUNT && begin.column < COLUMN_COUNT; (begin.column == 0) ? ++begin : begin--)
	{
		for (position_t trigger = begin; trigger.row < ROW_COUNT && trigger.column < COLUMN_COUNT;)
		{
			position_t vernier;
			for (vernier = trigger, ++vernier, vernier++; pb[vernier.row][vernier.column] == pb[trigger.row][trigger.column] && pb[trigger.row][trigger.column] != EMPTY; ++vernier, vernier++)
			{
				if (vernier.column - trigger.column == 4)
					return pb[trigger.row][trigger.column];
				if (vernier.row == ROW_COUNT - 1 || vernier.column == COLUMN_COUNT - 1)
					break;
			}
			if (trigger.row == ROW_COUNT-1 || trigger.column == COLUMN_COUNT - 1)
				break;
			trigger = vernier;
		}
		if (begin.row == ROW_COUNT - 1)
			break;
	}
	return EMPTY;
}