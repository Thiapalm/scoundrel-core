#pragma once
#include "playerclass.h"
#include "player.h"
#include "gamecontext.h"
#include "assets/Passives.h"

class WarriorClass : public PlayerClass
{
public:
    std::string name() const override { return "Warrior"; }

    void on_game_start(Player &player, GameContext &ctx) override
    {
        player.set_max_health(17);
        player.set_current_life(player.get_max_health());
        debuffs.push_back("ui.class.warrior_debuff");
        buffs.push_back("ui.class.warrior_buff");
        player.add_passive(std::make_unique<DamageResistPassive>());
    }

    int on_gain_life(Player &player, int val) override
    {
        return val -= 1;
    }

    int modify_outgoing_damage(int base_damage) override { return base_damage + 1; }

    int modify_incoming_damage(Player &player, int base_damage) override
    {
        if (base_damage > 0)
        {
            return std::max(1, base_damage - 1); // Reduced by 1, min 1
        }
        return base_damage;
    }
};