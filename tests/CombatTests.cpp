#include <gtest/gtest.h>
#include "assets/player.h"
#include "assets/cards.h"
#include "actions/action.h"
#include "classes/HealerClass.h"
#include "classes/WarriorClass.h"
#include "gamecontext.h"
#include "assets/dungeon.h"
#include "assets/graveyard.h"
#include "assets/room.h"

TEST(CombatTests, PlayerLifeGainLoss)
{
    Player player(20);

    EXPECT_EQ(player.current_life(), 20);

    player.lose_life(5);
    EXPECT_EQ(player.current_life(), 15);

    player.gain_life(3);
    EXPECT_EQ(player.current_life(), 18);

    // Test max life cap
    player.gain_life(10);
    EXPECT_EQ(player.current_life(), 20);
}

TEST(CombatTests, MonsterDamageCalculation)
{
    // Monster face value + 2 = damage
    // _10 is index 8, so 8 + 2 = 10 damage
    Monster monster(suit::Clubs, face::_10);
    EXPECT_EQ(monster.get_damage(), 10);

    Monster ace(suit::Spades, face::_A);
    EXPECT_EQ(ace.get_damage(), 14);
}

TEST(CombatTests, HealerClassLogic)
{
    auto player = std::make_unique<Player>(std::make_unique<HealerClass>());
    auto dungeon = std::make_unique<Dungeon>();
    auto graveyard = std::make_unique<Graveyard>();
    auto room = std::make_unique<Room>(std::vector<std::unique_ptr<Card>>{});
    std::vector<std::unique_ptr<Action>> actions;
    GameContext ctx(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions));

    ctx.get_player()->on_game_start(ctx);

    // Healer starts with -5 max life (15)
    EXPECT_EQ(ctx.get_player()->get_max_health(), 15);
    EXPECT_EQ(ctx.get_player()->current_life(), 15);

    // Healer has +1 Health gain
    ctx.get_player()->set_current_life(10);
    ctx.get_player()->gain_life(5);                  // Should gain 6
    EXPECT_EQ(ctx.get_player()->current_life(), 15); // Capped at max health (15)
}

TEST(CombatTests, WarriorClassLogic)
{
    auto player = std::make_unique<Player>(std::make_unique<WarriorClass>());
    auto dungeon = std::make_unique<Dungeon>();
    auto graveyard = std::make_unique<Graveyard>();
    auto room = std::make_unique<Room>(std::vector<std::unique_ptr<Card>>{});
    std::vector<std::unique_ptr<Action>> actions;
    GameContext ctx(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions));

    ctx.get_player()->on_game_start(ctx);

    // Warrior has +1 Weapon Damage
    ctx.equip_weapon(std::make_unique<Weapon>(suit::Diamonds, face::_5)); // Base 5

    // Attack a monster (Value 10)
    // Warrior attack = (face 10 + 2) = 12 damage monster.
    // Weapon base damage = (face 5 + 2) = 7.
    // Warrior +1 Weapon Damage = 8.
    // Base damage taken = 12 - 8 = 4.
    // Warrior Thick Skin (-1 incoming) = 4 - 1 = 3.
    // Life lost = 3
    ctx.slay_monster_with_weapon(std::make_unique<Monster>(suit::Clubs, face::_10));

    EXPECT_EQ(ctx.get_player()->current_life(), 14); // 17 - 3
}
