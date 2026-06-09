#pragma once
#include "action.h"

class AttackMonsterNoWeaponAction : public Action
{
public:
    std::string name() const override { return "Hand Combat"; }

    bool can_execute(const GameContext &ctx, const Card *card) const override;

    void execute(GameContext &ctx, std::unique_ptr<Card> selected_card) override;
};