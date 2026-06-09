#include "gamecontext.h"
#include "cards.h"
#include "action.h"
#include <algorithm>

std::vector<Action*> GameContext::get_turn_valid_actions() const {
    std::vector<Action*> result;
    for (const auto& action : available_actions) {
        // Check if action can execute without a card (e.g., New Room, Skip Room)
        if (action->can_execute(*this, nullptr)) {
            result.emplace_back(action.get());
        }

        // Check if action can execute with any card in the room
        for (int i = 0; i < room->cards_in_room(); ++i) {
            auto& c = room->look_card(i);
            if(action->can_execute(*this, &c)) {
                result.emplace_back(action.get());
                // We found at least one card it works on, so this action is valid for the turn
                break; 
            }
        }
    }
    
    std::sort(result.begin(), result.end(), [](Action* a, Action* b) {
        return a->name() < b->name();
    });
    auto last = std::unique(result.begin(), result.end());
    result.erase(last, result.end());
    return result;
}

void GameContext::change_turn() {
    for (auto& action : available_actions) {
        if (action->name() == "New Room")
            if (action->can_execute(*this, nullptr))
                action->execute(*this, nullptr);
    }
}

void GameContext::equip_weapon(std::unique_ptr<Card> new_weapon_card) {
    if (weapon != nullptr) {
        while (weapon->number_of_monsters() != 0) {
            graveyard->put_into(weapon->remove_monster());
        }
        graveyard->put_into(std::move(weapon));
    }
    
    if (auto* weapon_ptr = dynamic_cast<Weapon*>(new_weapon_card.get())) {
        weapon = std::unique_ptr<Weapon>(weapon_ptr);
        new_weapon_card.release();
    }
    actions_taken++;
}

void GameContext::slay_monster_with_weapon(std::unique_ptr<Card> monster_card) {
    auto* monster = dynamic_cast<Monster*>(monster_card.get());
    if (!monster) return;

    int weapon_attack = player->modify_outgoing_damage(weapon->weapon_damage());
    int damage = std::max(0, monster->get_damage() - weapon_attack);
    
    weapon->kill_monster(std::unique_ptr<Monster>(static_cast<Monster*>(monster_card.release())));
    player->lose_life(damage);
    actions_taken++;
}

void GameContext::fight_monster_barehanded(std::unique_ptr<Card> monster_card) {
    auto* monster = dynamic_cast<Monster*>(monster_card.get());
    if (!monster) return;

    int damage = monster->get_damage();
    graveyard->put_into(std::move(monster_card));
    player->lose_life(damage);
    actions_taken++;
}

void GameContext::consume_potion(std::unique_ptr<Card> potion_card) {
    auto* potion = dynamic_cast<Potion*>(potion_card.get());
    if (!potion) return;

    int potion_value = potion->drink_potion();
    if (!potion_used) {
        player->gain_life(potion_value);
    }
    
    graveyard->put_into(std::move(potion_card));
    potion_used = true;
    actions_taken++;
    last_action_name = "Drink Potion";
    last_potion_value = potion_value;
}

void GameContext::skip_current_room() {
    for (auto card = room->pop_last(); card; card = room->pop_last()) {
        dungeon->return_card(std::move(card));
    }

    if (dungeon->dungeon_size() <= 3) {
        endgame = true;
        return;
    }

    std::vector<std::unique_ptr<Card>> cards;
    for (int i = 0; i < 4; ++i)
        cards.push_back(dungeon->get_card());
    
    room = std::make_unique<Room>(std::move(cards));
    room_skipped = true;
    potion_used = false;
    actions_taken = 0;
    player->on_pre_room(*this);
}

void GameContext::prepare_new_room() {
    while (!room->room_empty()) {
        auto card = room->pop_last();
        dungeon->return_top(std::move(card));
    }

    if (dungeon->dungeon_size() <= 3) {
        endgame = true;
        return;
    }

    std::vector<std::unique_ptr<Card>> cards;
    for (int i = 0; i < 4; ++i)
        cards.push_back(dungeon->get_card());
    
    room = std::make_unique<Room>(std::move(cards));
    room_skipped = false;
    potion_used = false;
    actions_taken = 0;
    player->on_pre_room(*this);
}
