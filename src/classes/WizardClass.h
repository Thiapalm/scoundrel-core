#pragma once
#include "playerclass.h"
#include "player.h"
#include "gamecontext.h"

class WizardClass : public PlayerClass
{
public:
    std::string name() const override { return "Wizard"; }

    void on_game_start(Player &player, GameContext &ctx) override
    {
        player.set_max_health(14);
        player.set_current_life(player.get_max_health());
        buffs.push_back("ui.class.wizard_buff1"); // Sequential Potions
        buffs.push_back("ui.class.wizard_buff2"); // Banish Monster
        debuffs.push_back("ui.class.wizard_debuff"); // Lower Max HP
    }

    bool can_use_multiple_potions() const override { return true; }
};