#pragma once
#include "GameLogic.h"
#include <vector>


class ExtendedGameLogic : public GameLogic
{
private:
    int last_monster{0};
    size_t action{0};
    int card{0};
    int damage{0};
    std::vector<Action *> valid_actions;
    int calculate_loosing_score(GameContext &ctx);
public:
    explicit ExtendedGameLogic(GameType type = GameType::EXTENDED) : GameLogic(type){}
protected:
    GamePhase initialize_game(GameContext &ctx) override;
    GamePhase pre_turn(GameContext &ctx) override;
    GamePhase start_turn(GameContext &ctx) override;
    GamePhase pre_combat(GameContext &ctx) override;
    GamePhase combat(GameContext &ctx) override;
    GamePhase post_combat(GameContext &ctx) override;
    GamePhase terminate_turn(GameContext &ctx) override;
    GamePhase post_turn(GameContext &ctx) override;
    GamePhase end_game(GameContext &ctx) override;

};