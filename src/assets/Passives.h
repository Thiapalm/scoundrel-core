#pragma once
#include "passive_ability.h"
#include "gamecontext.h"
#include "actions/action.h"
#include "i18n/LocalizationManager.h"
#include <string>

class HealPassive : public PassiveAbility {
public:
    std::string name() const override { return "ui.passive.soothing_aura.name"; }
    std::string description() const override { return "ui.passive.soothing_aura.description"; }
    
    void execute(GameContext& ctx, PassiveTrigger trigger) override {
        if (trigger == PassiveTrigger::START_ROOM) {
            auto player = ctx.get_player();
            if (player->current_life() < player->get_max_health()) {
                player->set_current_life(std::min(player->get_max_health(), player->current_life() + 1));
                auto& lm = LocalizationManager::instance();
                ctx.add_message(lm.get_string(name()) + " " + lm.get_string("ui.passive.heal_message", {"1"}));
            }
        }
    }
};

class DamageResistPassive : public PassiveAbility {
public:
    std::string name() const override { return "ui.passive.thick_skin.name"; }
    std::string description() const override { return "ui.passive.thick_skin.description"; }
    
    void execute(GameContext& ctx, PassiveTrigger trigger) override {
        // This passive is passive-reactive, handled in Player::modify_incoming_damage.
        // We can keep this class as a descriptor or for other triggers.
    }
};
