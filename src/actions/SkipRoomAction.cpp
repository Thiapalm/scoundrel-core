#include "action.h"
#include <vector>
#include "SkipRoomAction.h"

bool SkipRoomAction::can_execute(const GameContext &ctx, const Card *card) const
{
    bool can_skip = false;
    if ((ctx.get_actions_taken() == 3) || (ctx.is_room_skipped() == false && ctx.get_actions_taken() == 0))
        can_skip = true;
    return can_skip;
}

void  SkipRoomAction::execute(GameContext &ctx, std::unique_ptr<Card> selected_card)
{
    ctx.skip_current_room();
}