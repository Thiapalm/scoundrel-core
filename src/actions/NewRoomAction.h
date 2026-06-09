#include "action.h"

class NewRoomAction : public Action
{
public:
    std::string name() const override { return "New Room"; }


    bool can_execute(const GameContext &ctx, const Card *card = nullptr) const override;
    void execute(GameContext &ctx, std::unique_ptr<Card> selected_card) override;
};