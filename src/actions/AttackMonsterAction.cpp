#include "AttackMonsterAction.h"

bool AttackMonsterAction::can_execute(const GameContext &ctx, const Card *card) const
{
    if (card != nullptr && card->getType() == CardType::Monster)
    {
        if (ctx.get_weapon() == nullptr)
            return false;

        if (ctx.get_weapon()->number_of_monsters() != 0)
        {
            auto monster = static_cast<const Monster *>(card);
            if (ctx.get_weapon()->last_monster_damage() <= monster->get_damage())
                return false;
        }
        return true;
    }
    return false;
}

void AttackMonsterAction::execute(GameContext &ctx, std::unique_ptr<Card> selected_card)
{
    ctx.slay_monster_with_weapon(std::move(selected_card));
}