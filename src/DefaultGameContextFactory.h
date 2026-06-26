#pragma once
#include "GameContextFactory.h"
#include "AttackMonsterAction.h"
#include "DrinkPotionAction.h"
#include "SkipRoomAction.h"
#include "AttachWeaponAction.h"
#include "NewRoomAction.h"
#include "AttackMonsterNoWeaponAction.h"
#include "BanishMonsterAction.h"
#include <vector>

class DefaultGameContextFactory : public GameContextFactory
{
private:
    std::vector<std::unique_ptr<Card>> remove_cards() override
    {
        std::vector<std::unique_ptr<Card>> cards;

        cards.push_back(std::make_unique<Weapon>(suit::Diamonds, face::_JK));
        cards.push_back(std::make_unique<Potion>(suit::Hearts, face::_JK));
        cards.push_back(std::make_unique<Monster>(suit::Clubs, face::_JK));
        cards.push_back(std::make_unique<Monster>(suit::Spades, face::_JK));

        cards.push_back(std::make_unique<Potion>(suit::Hearts, face::_A));
        cards.push_back(std::make_unique<Weapon>(suit::Diamonds, face::_A));

        cards.push_back(std::make_unique<Potion>(suit::Hearts, face::_K));
        cards.push_back(std::make_unique<Potion>(suit::Hearts, face::_Q));
        cards.push_back(std::make_unique<Potion>(suit::Hearts, face::_J));

        cards.push_back(std::make_unique<Weapon>(suit::Diamonds, face::_K));
        cards.push_back(std::make_unique<Weapon>(suit::Diamonds, face::_Q));
        cards.push_back(std::make_unique<Weapon>(suit::Diamonds, face::_J));

        return cards;
    }

    std::vector<std::unique_ptr<Action>> valid_actions() override
    {
        std::vector<std::unique_ptr<Action>> actions;
        actions.push_back(std::make_unique<AttackMonsterAction>());
        actions.push_back(std::make_unique<AttackMonsterNoWeaponAction>());
        actions.push_back(std::make_unique<AttachWeaponAction>());
        actions.push_back(std::make_unique<DrinkPotionAction>());
        actions.push_back(std::make_unique<SkipRoomAction>());
        actions.push_back(std::make_unique<NewRoomAction>());
        actions.push_back(std::make_unique<BanishMonsterAction>());
        return actions;
    }

public:
    void set_max_player_life(int life = 20) override { this->life = life; };

    std::unique_ptr<GameContext> create_game_context(unsigned int seed = 0, GameType type = GameType::DEFAULT) override
    {
        auto dungeon = std::make_unique<Dungeon>();
        auto remove = remove_cards();
        if (type == GameType::EXTENDED) {
            std::erase_if(remove, [](const std::unique_ptr<Card>& c) {
                return c->get_face() == face::_JK && (c->get_suit() == suit::Spades || c->get_suit() == suit::Clubs);
            });
        }
        dungeon->remove(remove);
        dungeon->shuffle(seed);
        
        std::unique_ptr<Player> player;
        if (m_player) {
            player = std::move(m_player);
        } else {
            player = std::make_unique<Player>(life > 0 ? life : 20);
        }
        
        auto graveyard = std::make_unique<Graveyard>();

        std::vector<std::unique_ptr<Card>> cards;
        for (int i = 0; i < 4; ++i)
            cards.push_back(dungeon->get_card());

        auto room = std::make_unique<Room>(std::move(cards));

        auto available_actions = valid_actions();

        return std::make_unique<GameContext>(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(available_actions), m_auth_manager, type);
    }
};
