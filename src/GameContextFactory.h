#pragma once

#include "gamecontext.h"
#include "ui/AuthManager.h"
#include <vector>
#include <memory>

class GameContextFactory
{
protected:
    int life;
    std::unique_ptr<Player> m_player;
    std::shared_ptr<AuthManager> m_auth_manager;
    virtual std::vector<std::unique_ptr<Card>> remove_cards() = 0;
    virtual std::vector<std::unique_ptr<Action>> valid_actions() = 0;
    GameContextFactory() : life{0}, m_auth_manager{std::make_shared<NullAuthManager>()} {};

public:
    virtual void set_max_player_life(int life) = 0;
    virtual void set_player(std::unique_ptr<Player> player) { m_player = std::move(player); }
    virtual void set_auth_manager(std::shared_ptr<AuthManager> auth) { m_auth_manager = auth; }
    virtual std::unique_ptr<GameContext> create_game_context(unsigned int seed = 0, GameType type = GameType::DEFAULT) = 0;
    virtual ~GameContextFactory() {}
};