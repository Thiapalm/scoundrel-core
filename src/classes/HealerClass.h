#pragma once
#include "playerclass.h"
#include "player.h"
#include "gamecontext.h"
#include "assets/Passives.h"

class HealerClass : public PlayerClass {
public:
    std::string name() const override { return "Healer"; }

    void on_game_start(Player &player, GameContext &ctx) override {
        player.set_max_health(player.get_max_health() - 5); // começa com -5 de vida (20 -> 15)
        player.set_current_life(player.get_max_health());
        debuffs.push_back("ui.class.healer_debuff");
        buffs.push_back("ui.class.healer_buff");
        player.add_passive(std::make_unique<HealPassive>());
    }
    
    int on_gain_life(Player& player, int val) override {
        return val+=1;
    }

};