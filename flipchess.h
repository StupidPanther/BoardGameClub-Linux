#ifndef __FLIPCHESS_H__
#define __FLIPCHESS_H__

#include "ref.h"

class Flipchess
{
public:
    void Init(plainboard_t& pb);
    movestatus_t IfMoveValid(const plainboard_t pb, const user_t user, const position_t pos);
    void Flip(plainboard_t& pb, const position_t pos);
    string CurStatus(const plainboard_t pb);
    bool IfStop(const plainboard_t pb);
    user_t WhoWin(const plainboard_t pb);
    bool CouldMove(const plainboard_t pb, user_t usr);
};

#endif