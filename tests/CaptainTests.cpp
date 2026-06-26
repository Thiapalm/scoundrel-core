#include <gtest/gtest.h>
#include "assets/cards.h"
#include "assets/dungeon.h"
#include "assets/player.h"
#include "assets/room.h"
#include "assets/graveyard.h"
#include "gamecontext.h"
#include "DefaultGameContextFactory.h"

TEST(CaptainTests, CaptainPropertiesAndDungeon)
{
    // 1. Captain damage is 12
    Monster warlord(suit::Spades, face::_JK);
    Monster plague_doctor(suit::Clubs, face::_JK);
    EXPECT_EQ(warlord.get_damage(), 12);
    EXPECT_EQ(plague_doctor.get_damage(), 12);

    // 2. Default mode removes Jokers, but Extended mode retains them
    DefaultGameContextFactory factory;
    auto ctx_default = factory.create_game_context(1234, GameType::DEFAULT);
    auto *dung_def = ctx_default->get_dungeon();
    bool has_joker_def = false;
    for (unsigned i = 0; i < dung_def->dungeon_size(); ++i)
    {
        const auto &card = dung_def->card_at(i);
        if (card.get_face() == face::_JK)
        {
            has_joker_def = true;
        }
    }
    EXPECT_FALSE(has_joker_def);

    auto ctx_ext = factory.create_game_context(1234, GameType::EXTENDED);
    auto *dung_ext = ctx_ext->get_dungeon();
    bool has_joker_clubs_ext = false;
    bool has_joker_spades_ext = false;
    for (unsigned i = 0; i < dung_ext->dungeon_size(); ++i)
    {
        const auto &card = dung_ext->card_at(i);
        if (card.get_face() == face::_JK)
        {
            if (card.get_suit() == suit::Clubs)
                has_joker_clubs_ext = true;
            if (card.get_suit() == suit::Spades)
                has_joker_spades_ext = true;
        }
    }
    // They are either in the dungeon or already in the starting room. Let's check both.
    auto *room_ext = ctx_ext->get_room();
    for (int i = 0; i < room_ext->cards_in_room(); ++i)
    {
        const auto &card = room_ext->look_card(i);
        if (card.get_face() == face::_JK)
        {
            if (card.get_suit() == suit::Clubs)
                has_joker_clubs_ext = true;
            if (card.get_suit() == suit::Spades)
                has_joker_spades_ext = true;
        }
    }
    EXPECT_TRUE(has_joker_clubs_ext);
    EXPECT_TRUE(has_joker_spades_ext);
}

TEST(CaptainTests, PlagueDoctorPassiveHalvesHealing)
{
    {
        auto dungeon = std::make_unique<Dungeon>();
        auto player = std::make_unique<Player>(20);
        auto graveyard = std::make_unique<Graveyard>();

        std::vector<std::unique_ptr<Card>> room_cards;
        room_cards.push_back(std::make_unique<Monster>(suit::Clubs, face::_JK)); // Plague Doctor
        room_cards.push_back(std::make_unique<Potion>(suit::Hearts, face::_8));  // heals 8
        auto room = std::make_unique<Room>(std::move(room_cards));

        std::vector<std::unique_ptr<Action>> actions;
        actions.push_back(std::make_unique<DrinkPotionAction>());

        GameContext ctx(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions), nullptr, GameType::EXTENDED);

        EXPECT_TRUE(ctx.is_plague_doctor_active());

        // Drink Potion 8 -> heals 4
        ctx.get_player()->set_current_life(10);
        ctx.consume_potion(ctx.pop_card_from_room(1));
        EXPECT_EQ(ctx.get_player()->current_life(), 14);
        EXPECT_EQ(ctx.get_last_potion_value(), 4);
    }

    {
        auto dungeon = std::make_unique<Dungeon>();
        auto player = std::make_unique<Player>(20);
        auto graveyard = std::make_unique<Graveyard>();

        std::vector<std::unique_ptr<Card>> room_cards;
        room_cards.push_back(std::make_unique<Monster>(suit::Clubs, face::_JK)); // Plague Doctor
        room_cards.push_back(std::make_unique<Potion>(suit::Hearts, face::_9));  // heals 9
        auto room = std::make_unique<Room>(std::move(room_cards));

        std::vector<std::unique_ptr<Action>> actions;
        actions.push_back(std::make_unique<DrinkPotionAction>());

        GameContext ctx(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions), nullptr, GameType::EXTENDED);

        EXPECT_TRUE(ctx.is_plague_doctor_active());

        // Drink Potion 9 -> heals 4 (rounded down)
        ctx.get_player()->set_current_life(10);
        ctx.consume_potion(ctx.pop_card_from_room(1));
        EXPECT_EQ(ctx.get_player()->current_life(), 14);
        EXPECT_EQ(ctx.get_last_potion_value(), 4);
    }
}

TEST(CaptainTests, WarlordPassiveInspiresOtherMonsters)
{
    auto dungeon = std::make_unique<Dungeon>();
    auto player = std::make_unique<Player>(20);
    auto graveyard = std::make_unique<Graveyard>();

    std::vector<std::unique_ptr<Card>> room_cards;
    room_cards.push_back(std::make_unique<Monster>(suit::Spades, face::_JK)); // Warlord
    room_cards.push_back(std::make_unique<Monster>(suit::Clubs, face::_3));   // base 3
    auto room = std::make_unique<Room>(std::move(room_cards));

    std::vector<std::unique_ptr<Action>> actions;
    actions.push_back(std::make_unique<AttackMonsterAction>());

    GameContext ctx(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions), nullptr, GameType::EXTENDED);

    EXPECT_TRUE(ctx.is_warlord_active());

    // Check effective damage
    auto *m_warlord = static_cast<const Monster *>(&ctx.get_room()->look_card(0));
    auto *m_other = static_cast<const Monster *>(&ctx.get_room()->look_card(1));

    EXPECT_EQ(ctx.get_monster_effective_damage(m_warlord), 12);
    EXPECT_EQ(ctx.get_monster_effective_damage(m_other), 5); // 3 + 2
}

TEST(CaptainTests, WarlordBonusIsStoredAsWeaponLastKillValue)
{
    auto dungeon = std::make_unique<Dungeon>();
    auto player = std::make_unique<Player>(20);
    auto graveyard = std::make_unique<Graveyard>();

    std::vector<std::unique_ptr<Card>> room_cards;
    room_cards.push_back(std::make_unique<Monster>(suit::Spades, face::_JK)); // Warlord (active aura)
    room_cards.push_back(std::make_unique<Monster>(suit::Clubs, face::_3));   // base 3, effective 5 with warlord
    auto room = std::make_unique<Room>(std::move(room_cards));

    std::vector<std::unique_ptr<Action>> actions;

    GameContext ctx(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions), nullptr, GameType::EXTENDED);

    ctx.equip_weapon(std::make_unique<Weapon>(suit::Diamonds, face::_10));
    ctx.slay_monster_with_weapon(ctx.pop_card_from_room(1));

    ASSERT_NE(ctx.get_weapon(), nullptr);
    EXPECT_EQ(ctx.get_weapon()->last_monster_damage(), 5);
}

TEST(CaptainTests, FleeingCaptainPassivePersists)
{
    auto dungeon = std::make_unique<Dungeon>();
    auto player = std::make_unique<Player>(20);
    auto graveyard = std::make_unique<Graveyard>();

    std::vector<std::unique_ptr<Card>> room_cards;
    room_cards.push_back(std::make_unique<Monster>(suit::Clubs, face::_JK)); // Plague Doctor
    auto room = std::make_unique<Room>(std::move(room_cards));

    std::vector<std::unique_ptr<Action>> actions;

    GameContext ctx(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions), nullptr, GameType::EXTENDED);

    EXPECT_TRUE(ctx.is_plague_doctor_active());

    // Skip room
    ctx.skip_current_room();

    // Plague doctor was in skipped room and not killed, so passive should persist!
    EXPECT_TRUE(ctx.is_plague_doctor_active());

    // Drink Potion 8 after skipping -> should heal only 4 because Plague Doctor is still active!
    ctx.get_player()->set_current_life(10);
    ctx.consume_potion(std::make_unique<Potion>(suit::Hearts, face::_8));
    EXPECT_EQ(ctx.get_player()->current_life(), 14);
    EXPECT_EQ(ctx.get_last_potion_value(), 4);
}

TEST(CaptainTests, KillingCaptainDeactivatesPassive)
{
    auto dungeon = std::make_unique<Dungeon>();
    auto player = std::make_unique<Player>(20);
    auto graveyard = std::make_unique<Graveyard>();

    std::vector<std::unique_ptr<Card>> room_cards;
    room_cards.push_back(std::make_unique<Monster>(suit::Clubs, face::_JK)); // Plague Doctor
    room_cards.push_back(std::make_unique<Potion>(suit::Hearts, face::_8));  // heals 8
    auto room = std::make_unique<Room>(std::move(room_cards));

    std::vector<std::unique_ptr<Action>> actions;

    GameContext ctx(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions), nullptr, GameType::EXTENDED);

    EXPECT_TRUE(ctx.is_plague_doctor_active());

    // Fight plague doctor barehanded
    ctx.fight_monster_barehanded(ctx.pop_card_from_room(0));

    // Plague doctor is killed, so passive is deactivated!
    EXPECT_FALSE(ctx.is_plague_doctor_active());
}
