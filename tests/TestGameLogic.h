#pragma once
#include "DefaultGameLogic.h"

class TestDefaultGameLogic : public DefaultGameLogic {
public:
    using DefaultGameLogic::initialize_game;
    using DefaultGameLogic::start_turn;
    using DefaultGameLogic::pre_combat;
    using DefaultGameLogic::combat;
    using DefaultGameLogic::post_combat;
    using DefaultGameLogic::terminate_turn;
    using DefaultGameLogic::end_game;
};
