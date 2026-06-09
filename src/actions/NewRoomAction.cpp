#include "action.h"
#include <vector>
#include "NewRoomAction.h"

bool NewRoomAction::can_execute(const GameContext &ctx, const Card *card) const
{
    bool can_skip = false;
    if (ctx.get_actions_taken() == 3 || ctx.get_room()->room_empty())
        can_skip = true;
    return can_skip;
}

void NewRoomAction::execute(GameContext &ctx, std::unique_ptr<Card> selected_card)
{
    ctx.prepare_new_room();
}