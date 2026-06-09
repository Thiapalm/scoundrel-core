#include "GameLogic.h"
#include "DefaultGameLogic.h"
#include "DefaultGameContextFactory.h"
#include "ExtendedGameLogic.h"
#include "GameContextFactory.h"
#include "PersistenceManager.h"
#include "ui/AuthManager.h"

std::shared_ptr<AuthManager> GameLogic::s_current_auth = nullptr;
static std::shared_ptr<scoundrel::PersistenceManager> s_persistence_inst = nullptr;

scoundrel::PersistenceManager* GameLogic::get_persistence_manager() {
    return s_persistence_inst.get();
}

void GameLogic::set_persistence_manager(std::shared_ptr<scoundrel::PersistenceManager> persistence) {
    s_persistence_inst = persistence;
}

AuthManager* GameLogic::get_current_auth_manager() {
    return s_current_auth.get();
}

std::shared_ptr<AuthManager> GameLogic::get_current_auth_manager_ptr() {
    if (!s_current_auth) {
        s_current_auth = std::make_shared<NullAuthManager>();
    }
    return s_current_auth;
}

void GameLogic::set_auth_manager(std::shared_ptr<AuthManager> auth) {
    s_current_auth = auth;
}

void GameLogic::run_game_loop()
{
    while (true)
    {
        GamePhase phase_to_execute = current_phase;
        current_phase = execute_phase(phase_to_execute);
        
        if (phase_to_execute == GamePhase::END_GAME) break;
    }
}

GamePhase GameLogic::execute_phase(GamePhase phase) {
    on_before_phase(phase);
    
    GamePhase next = phase;
    switch (phase) {
        case GamePhase::INITIALIZE_GAME:
            next = initialize_game(*context);
            break;
        case GamePhase::PRE_TURN:
            next = pre_turn(*context);
            break;
        case GamePhase::START_TURN:
            next = start_turn(*context);
            break;
        case GamePhase::PRE_COMBAT:
            next = pre_combat(*context);
            break;
        case GamePhase::COMBAT:
            next = combat(*context);
            break;
        case GamePhase::POST_COMBAT:
            next = post_combat(*context);
            break;
        case GamePhase::TERMINATE_TURN:
            next = terminate_turn(*context);
            break;
        case GamePhase::POST_TURN:
            next = post_turn(*context);
            break;
        case GamePhase::END_GAME:
            next = end_game(*context);
            break;
    }
    
    on_after_phase(phase);
    return next;
}

std::unique_ptr<GameLogic> GameLogic::create(GameType type)
{
    std::unique_ptr<GameLogic> logic;
    std::unique_ptr<GameContextFactory> factory;

    switch (type)
    {
    case GameType::DEFAULT:
        logic = std::make_unique<DefaultGameLogic>();
        factory = std::make_unique<DefaultGameContextFactory>();
        break;
    case GameType::EXTENDED:
        logic = std::make_unique<ExtendedGameLogic>();
        factory = std::make_unique<DefaultGameContextFactory>();
        break;
    default:
        throw std::invalid_argument("Unknown GameType");
    }

    factory->set_auth_manager(get_current_auth_manager_ptr());
    logic->set_context(factory->create_game_context(0, type));
    return logic;
}
