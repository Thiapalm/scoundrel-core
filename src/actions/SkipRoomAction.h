#include "action.h"

class SkipRoomAction : public Action
{
public:
    std::string name() const override { return "Skip Room"; }


    bool can_execute(const GameContext &ctx, const Card *card = nullptr) const override;
    void execute(GameContext &ctx, std::unique_ptr<Card> selected_card) override;
};