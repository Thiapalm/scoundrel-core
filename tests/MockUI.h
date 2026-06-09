#pragma once
#include "ui/UserInterface.h"
#include <queue>
#include <random>
#include <optional>

class MockUI : public UserInterface
{
public:
    std::queue<int> mode_choices;
    std::queue<int> class_choices;
    std::queue<int> action_choices;
    std::queue<int> card_choices;

    bool victory_shown = false;
    bool defeat_shown = false;
    int final_score = 0;

    std::optional<int> select_game_mode() override
    {
        if (mode_choices.empty())
            return 1;
        int choice = mode_choices.front();
        mode_choices.pop();
        if (choice == -1)
            return std::nullopt;
        return choice;
    }

    std::optional<int> select_player_class() override
    {
        if (class_choices.empty())
            return 1;
        int choice = class_choices.front();
        class_choices.pop();
        if (choice == -1)
            return std::nullopt;
        return choice;
    }

    void display_game_state(const GameContext &ctx) override {}
    void display_valid_actions(std::span<Action *const> actions) override {}

    std::optional<int> request_action_index(int max_index) override
    {
        if (!action_choices.empty())
        {
            int choice = action_choices.front();
            action_choices.pop();
            if (choice == -1)
                return std::nullopt;
            if (max_index <= 0)
                return 0;
            return static_cast<unsigned int>(choice) % max_index;
        }
        return 0;
    }

    std::optional<int> request_card_index(int max_cards) override
    {
        if (max_cards <= 0)
            return 1;
        if (!card_choices.empty())
        {
            int choice = card_choices.front();
            card_choices.pop();
            if (choice == -1)
                return std::nullopt;
            return (static_cast<unsigned int>(choice) % max_cards) + 1;
        }
        return 1;
    }

    void show_message(const std::string &message) override {}
    void show_error(const std::string &error) override {}

    void show_victory(int score) override
    {
        victory_shown = true;
        final_score = score;
    }

    void show_defeat(int score, const std::string &reason) override
    {
        defeat_shown = true;
        final_score = score;
    }

    bool ask_restart() override
    {
        return false;
    }
};

#include <gmock/gmock.h>
class GMockUI : public UserInterface
{
public:
    MOCK_METHOD(std::optional<int>, select_game_mode, (), (override));
    MOCK_METHOD(std::optional<int>, select_player_class, (), (override));
    MOCK_METHOD(void, display_game_state, (const GameContext &ctx), (override));
    MOCK_METHOD(void, display_valid_actions, (std::span<Action *const> actions), (override));
    MOCK_METHOD(std::optional<int>, request_action_index, (int max_index), (override));
    MOCK_METHOD(std::optional<int>, request_card_index, (int max_cards), (override));
    MOCK_METHOD(void, show_message, (const std::string &message), (override));
    MOCK_METHOD(void, show_error, (const std::string &error), (override));
    MOCK_METHOD(void, show_victory, (int score), (override));
    MOCK_METHOD(void, show_defeat, (int score, const std::string &reason), (override));
    MOCK_METHOD(bool, ask_restart, (), (override));
};
