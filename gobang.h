#ifndef __GOBANG_H__
#define __GOBANG_H__

#include "ref.h"

class Gobang
{
public:
    movestatus_t IfMoveValid(const plainboard_t pb, const user_t user, position_t pos);
    user_t WhoWin(const plainboard_t pb);
};

#endif