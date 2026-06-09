#pragma once
#include "gamecontext.h"
#include "ui/UserInterface.h"

enum class GamePhase
{
    INITIALIZE_GAME,
    PRE_TURN,
    START_TURN,
    PRE_COMBAT,
    COMBAT,
    POST_COMBAT,
    TERMINATE_TURN,
    POST_TURN,
    END_GAME
};

class AuthManager;

namespace scoundrel {
    class PersistenceManager;
}

class GameLogic
{
protected:
    GameType game_type{GameType::DEFAULT};
    GamePhase current_phase{GamePhase::INITIALIZE_GAME};
    std::unique_ptr<GameContext> context;
    UserInterface* ui{nullptr};
    static std::shared_ptr<AuthManager> s_current_auth;

public:
    explicit GameLogic(GameType type = GameType::DEFAULT) : game_type{type} {};
    virtual ~GameLogic() = default;
    virtual GamePhase initialize_game(GameContext &ctx) = 0;
    virtual GamePhase pre_turn(GameContext &ctx) = 0;
    virtual GamePhase start_turn(GameContext &ctx) = 0;
    virtual GamePhase pre_combat(GameContext &ctx) = 0;
    virtual GamePhase combat(GameContext &ctx) = 0;
    virtual GamePhase post_combat(GameContext &ctx) = 0;
    virtual GamePhase terminate_turn(GameContext &ctx) = 0;
    virtual GamePhase post_turn(GameContext &ctx) = 0;
    virtual GamePhase end_game(GameContext &ctx) = 0;

    // Phase Hooks
    virtual void on_before_phase(GamePhase phase) {}
    virtual void on_after_phase(GamePhase phase) {}

    void set_interface(UserInterface* interface) {
        ui = interface;
    }

    void set_context(std::unique_ptr<GameContext> ctx) {
        context = std::move(ctx);
    }
    
    static AuthManager* get_current_auth_manager();
    static std::shared_ptr<AuthManager> get_current_auth_manager_ptr();
    static void set_auth_manager(std::shared_ptr<AuthManager> auth);

    static scoundrel::PersistenceManager* get_persistence_manager();
    static void set_persistence_manager(std::shared_ptr<scoundrel::PersistenceManager> persistence);

    static std::unique_ptr<GameLogic> create(GameType type);
    
    void run_game_loop();

private:
    GamePhase execute_phase(GamePhase phase);
};
