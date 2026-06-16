#pragma once

#include <memory>
#include "dungeon.h"
#include "player.h"
#include "graveyard.h"
#include "room.h"
#include "playerclass.h"
#include "ui/AuthManager.h"
#include <vector>
class Action;

enum class GameType
{
    DEFAULT,
    EXTENDED
};

class GameContext
{
private:
    std::unique_ptr<Dungeon> dungeon;
    std::unique_ptr<Player> player;
    std::unique_ptr<Graveyard> graveyard;
    std::unique_ptr<Room> room;
    std::unique_ptr<Weapon> weapon;
    std::shared_ptr<AuthManager> auth_manager;
    GameType type{GameType::DEFAULT};
    bool room_skipped{false};
    bool potion_used{false};
    int actions_taken{0};
    bool endgame{false};
    bool victory{false};
    std::vector<std::unique_ptr<Action>> available_actions;
    std::vector<std::string> messages;
    std::string last_action_name;
    int last_potion_value{0};

public:
    GameContext(std::unique_ptr<Dungeon> dg,
                std::unique_ptr<Player> pl,
                std::unique_ptr<Graveyard> gr,
                std::unique_ptr<Room> rm,
                std::vector<std::unique_ptr<Action>> ac,
                std::shared_ptr<AuthManager> auth = std::make_shared<NullAuthManager>(),
                GameType t = GameType::DEFAULT)
        : dungeon(std::move(dg)), player(std::move(pl)),
          graveyard(std::move(gr)), room(std::move(rm)),
          weapon(nullptr), auth_manager(auth), type(t),
          available_actions(std::move(ac)) {}

    // Getters
    Dungeon* get_dungeon() const { return dungeon.get(); }
    Player* get_player() const { return player.get(); }
    Graveyard* get_graveyard() const { return graveyard.get(); }
    Room* get_room() const { return room.get(); }
    Weapon* get_weapon() const { return weapon.get(); }
    AuthManager* get_auth_manager() const { return auth_manager.get(); }
    GameType get_type() const { return type; }
    bool is_room_skipped() const { return room_skipped; }
    bool is_potion_used() const { return potion_used; }
    int get_actions_taken() const { return actions_taken; }
    bool is_endgame() const { return endgame; }
    bool is_victory() const { return victory; }
    const std::vector<std::unique_ptr<Action>> &get_available_actions() const { return available_actions; }
    const std::string& get_last_action_name() const { return last_action_name; }
    int get_last_potion_value() const { return last_potion_value; }
    
    // Message Queue
    void add_message(const std::string& msg) { messages.push_back(msg); }
    std::vector<std::string> get_and_clear_messages() {
        auto result = std::move(messages);
        messages.clear();
        return result;
    }

    // Domain Mutations
    void equip_weapon(std::unique_ptr<Card> new_weapon_card);
    void slay_monster_with_weapon(std::unique_ptr<Card> monster_card);
    void fight_monster_barehanded(std::unique_ptr<Card> monster_card);
    void banish_monster(std::unique_ptr<Card> monster_card);
    void consume_potion(std::unique_ptr<Card> potion_card);
    void skip_current_room();
    void prepare_new_room();
    void increment_actions_taken() { actions_taken++; }
    void set_game_over(bool is_victory) { endgame = true; victory = is_victory; }
    std::unique_ptr<Card> pop_card_from_room(int pos) { return room->pop_card(pos); }

    void set_player(std::unique_ptr<Player> new_player) { player = std::move(new_player); }
    std::vector<Action *> get_turn_valid_actions() const;
    void change_turn();
    void set_last_action_name(const std::string& name) { last_action_name = name; }
};