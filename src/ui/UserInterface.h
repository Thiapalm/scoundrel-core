#pragma once
#include <vector>
#include <string>
#include <span>
#include <optional>
#include "action.h"

class GameContext;

class UserInterface
{
public:
    virtual ~UserInterface() = default;

    virtual std::optional<int> select_game_mode() = 0;
    virtual std::optional<int> select_player_class() = 0;

    virtual void display_game_state(const GameContext &ctx) = 0;
    virtual void display_valid_actions(std::span<Action *const> actions) = 0;

    virtual std::optional<int> request_action_index(int max_index) = 0;
    virtual std::optional<int> request_card_index(int max_cards) = 0;

    virtual void show_message(const std::string &message) = 0;
    virtual void show_error(const std::string &error) = 0;

    virtual void show_victory(int score) = 0;
    virtual void show_defeat(int score, const std::string &reason) = 0;
    virtual bool ask_restart() = 0;
    virtual void reset() {}
};
