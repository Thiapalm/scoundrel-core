#include "action.h"
#include <vector>
#include "AttachWeaponAction.h"

bool AttachWeaponAction::can_execute(const GameContext &ctx, const Card *card) const
{
    if (card != nullptr && card->getType() == CardType::Weapon)
    {
        return true;
    }
    return false;
}

void AttachWeaponAction::execute(GameContext &ctx, std::unique_ptr<Card> selected_card)
{
    ctx.equip_weapon(std::move(selected_card));
}