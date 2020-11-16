#ifndef __BOARD_H__
#define __BOARD_H__

#include "ref.h"

class Board
{
private:
    plainboard_t plainboard;
    plainboard_t plainboard_regret;
    position_t position;

public:
    Board();
    ~Board();
    void Init();

    void MoveCursor(cursor_t cursor);
    movestatus_t Move(user_t moveuser);
    void Regret();
    const plainboard_t& GetPlainBoard() const;
    position_t GetCursorPosition() const;
    plainboard_t& SetPlainBoard();
    position_t& SetCursorPosition();
};

#endif