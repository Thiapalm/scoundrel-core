#pragma once
#include "playerclass.h"
#include "player.h"
#include "gamecontext.h"

class PeasantClass : public PlayerClass {
public:
    std::string name() const override { return "Peasant"; }


};