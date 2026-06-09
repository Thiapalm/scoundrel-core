#include "action.h"

class DrinkPotionAction : public Action
{
public:
    std::string name() const override { return "Drink Potion"; }

    bool can_execute(const GameContext &ctx, const Card *card) const override;

    void execute(GameContext &ctx, std::unique_ptr<Card> selected_card = nullptr) override;
};