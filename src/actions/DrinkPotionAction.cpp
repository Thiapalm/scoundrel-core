#include "action.h"
#include "DrinkPotionAction.h"

bool DrinkPotionAction::can_execute(const GameContext &ctx, const Card *card) const
{
    if (card != nullptr && card->getType() == CardType::Potion)
        return true;
    return false;
}

void  DrinkPotionAction::execute(GameContext &ctx, std::unique_ptr<Card> selected_card)
{
    ctx.consume_potion(std::move(selected_card));
}