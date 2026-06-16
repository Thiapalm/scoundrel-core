#include "BanishMonsterAction.h"
#include "gamecontext.h"
#include "cards.h"

bool BanishMonsterAction::can_execute(const GameContext &ctx, const Card *card) const
{
    if (card != nullptr && card->getType() == CardType::Monster)
    {
        if (ctx.get_player() && ctx.get_player()->getName() == "Wizard")
        {
            return true;
        }
    }
    return false;
}

void BanishMonsterAction::execute(GameContext &ctx, std::unique_ptr<Card> selected_card)
{
    ctx.banish_monster(std::move(selected_card));
}