#include "action.h"

class AttackMonsterAction : public Action
{
public:
    std::string name() const override { return "Attack Monster"; }

    bool can_execute(const GameContext &ctx, const Card *card) const override;

    void execute(GameContext &ctx, std::unique_ptr<Card> selected_card) override;
};