#include <gtest/gtest.h>
#include "gamecontext.h"
#include "assets/dungeon.h"
#include "assets/graveyard.h"
#include "assets/room.h"
#include "assets/player.h"
#include "actions/AttackMonsterAction.h"
#include "actions/DrinkPotionAction.h"
#include "actions/SkipRoomAction.h"
#include "actions/action.h"

class ActionTests : public ::testing::Test {
protected:
    std::unique_ptr<GameContext> ctx;
    
    void SetUp() override {
        auto dungeon = std::make_unique<Dungeon>();
        auto player = std::make_unique<Player>(20);
        auto graveyard = std::make_unique<Graveyard>();
        auto room = std::make_unique<Room>(std::vector<std::unique_ptr<Card>>{});
        std::vector<std::unique_ptr<Action>> actions;
        
        ctx = std::make_unique<GameContext>(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions));
    }
};

TEST_F(ActionTests, AttackMonsterActionValidation) {
    AttackMonsterAction attack;
    Monster weak_monster(suit::Clubs, face::_2); // 2 damage
    Monster strong_monster(suit::Clubs, face::_10); // 10 damage
    
    // Cannot attack with weapon if no weapon equipped
    EXPECT_FALSE(attack.can_execute(*ctx, &weak_monster));
    
    // Equip weapon (Value 5)
    ctx->equip_weapon(std::make_unique<Weapon>(suit::Diamonds, face::_5));
    
    // Can attack any monster with first weapon strike
    EXPECT_TRUE(attack.can_execute(*ctx, &strong_monster));
    
    // Slay monster (10 damage)
    ctx->slay_monster_with_weapon(std::make_unique<Monster>(suit::Clubs, face::_10));
    
    // Now can only attack monsters with damage < 10
    EXPECT_TRUE(attack.can_execute(*ctx, &weak_monster));
    EXPECT_FALSE(attack.can_execute(*ctx, &strong_monster));
}

TEST_F(ActionTests, DrinkPotionActionValidation) {
    DrinkPotionAction drink;
    Potion potion(suit::Hearts, face::_5);
    
    ctx->prepare_new_room(); // Dungeon has cards by default
    
    EXPECT_TRUE(drink.can_execute(*ctx, &potion));
    
    // Use a potion
    ctx->consume_potion(std::make_unique<Potion>(suit::Hearts, face::_5));
}

TEST_F(ActionTests, SkipRoomActionValidation) {
    SkipRoomAction skip;
    
    // Can skip first room
    EXPECT_TRUE(skip.can_execute(*ctx, nullptr));
    
    // Skip it
    ctx->skip_current_room();
    
    // Cannot skip second room in a row
    EXPECT_FALSE(skip.can_execute(*ctx, nullptr));
    
    // Enter a new room normally (resets skipped flag)
    ctx->prepare_new_room();
    EXPECT_TRUE(skip.can_execute(*ctx, nullptr));
}
