#pragma once
#include <string>
#include <vector>

class GameContext;
class Player;

class PlayerClass {
public:
    virtual ~PlayerClass() = default;
    virtual std::string name() const = 0;
    std::vector<std::string> buffs;
    std::vector<std::string> debuffs;

    // Chamado quando o jogo começa
    virtual void on_game_start(Player &player, GameContext &ctx) {}

    virtual int on_gain_life(Player &player, int base) { return base; }
    
    virtual int on_lose_life(Player &player, int base) { return base; }

    // Chamado quando uma poção é usada
    virtual void on_potion_use(Player &player, GameContext &ctx) {}

    // Chamado antes de entrar em uma sala
    virtual void on_pre_room(Player &player, GameContext &ctx) {}

    // Chamado quando o jogador sofre dano
    virtual int modify_incoming_damage(Player &player, int base_damage) { return base_damage; }

    // Chamado quando o jogador causa dano
    virtual int modify_outgoing_damage(int base_damage) { return base_damage; }
};
