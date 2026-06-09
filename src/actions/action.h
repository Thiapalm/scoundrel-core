#pragma once

#include <string>
#include "gamecontext.h"
#include <memory>


class Action
{
public:
    virtual ~Action() = default;

    virtual std::string name() const = 0;

    virtual bool can_execute(const GameContext &ctx, const Card *card = nullptr) const { return true; }

    virtual void execute(GameContext &ctx, std::unique_ptr<Card> selected_card = nullptr) = 0;
};