#pragma once
#include <string>

class GameContext;

enum class PassiveTrigger {
    START_TURN,
    BEFORE_COMBAT,
    AFTER_COMBAT,
    END_TURN,
    START_ROOM,
    END_ROOM
};

class PassiveAbility {
public:
    virtual ~PassiveAbility() = default;
    virtual std::string name() const = 0;
    virtual std::string description() const = 0;
    virtual void execute(GameContext& ctx, PassiveTrigger trigger) = 0;
};
