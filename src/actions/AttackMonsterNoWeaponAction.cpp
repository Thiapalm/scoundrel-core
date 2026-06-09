#include "AttackMonsterNoWeaponAction.h"

bool AttackMonsterNoWeaponAction::can_execute(const GameContext &ctx, const Card *card) const
{
    if (card != nullptr && card->getType() == CardType::Monster)
    {
        return true;
    }
    return false;
}

void AttackMonsterNoWeaponAction::execute(GameContext &ctx, std::unique_ptr<Card> selected_card)
{
    ctx.fight_monster_barehanded(std::move(selected_card));
}