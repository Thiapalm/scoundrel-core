#include <gtest/gtest.h>
#include "assets/player.h"
#include "assets/cards.h"
#include "actions/action.h"
#include "classes/WizardClass.h"
#include "classes/PeasantClass.h"
#include "gamecontext.h"
#include "assets/dungeon.h"
#include "assets/graveyard.h"
#include "assets/room.h"
#include "actions/BanishMonsterAction.h"
#include <vector>

TEST(WizardTests, WizardInitialHealth)
{
    auto player = std::make_unique<Player>(std::make_unique<WizardClass>());
    auto dungeon = std::make_unique<Dungeon>();
    auto graveyard = std::make_unique<Graveyard>();
    auto room = std::make_unique<Room>(std::vector<std::unique_ptr<Card>>{});
    std::vector<std::unique_ptr<Action>> actions;
    GameContext ctx(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions));

    ctx.get_player()->on_game_start(ctx);

    EXPECT_EQ(ctx.get_player()->get_max_health(), 14);
    EXPECT_EQ(ctx.get_player()->current_life(), 14);
}

TEST(WizardTests, WizardSequentialPotions)
{
    auto player = std::make_unique<Player>(std::make_unique<WizardClass>());
    auto dungeon = std::make_unique<Dungeon>();
    auto graveyard = std::make_unique<Graveyard>();
    auto room = std::make_unique<Room>(std::vector<std::unique_ptr<Card>>{});
    std::vector<std::unique_ptr<Action>> actions;
    GameContext ctx(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions));

    ctx.get_player()->on_game_start(ctx);
    ctx.get_player()->set_current_life(5);

    // First potion
    ctx.consume_potion(std::make_unique<Potion>(suit::Hearts, face::_8)); // 8 + 2 = 10. Wait, face::_8 is index 6, so 6+2=8. Correct.
    EXPECT_EQ(ctx.get_player()->current_life(), 13); // 5 + 8 = 13

    // Second potion (should work for Wizard)
    ctx.consume_potion(std::make_unique<Potion>(suit::Hearts, face::_4)); // 2 + 2 = 4. Wait, face::_4 is index 2, so 2+2=4. Correct.
    EXPECT_EQ(ctx.get_player()->current_life(), 14); // 13 + 4 = 17, capped at 14
}

TEST(WizardTests, PeasantSequentialPotionsFail)
{
    auto player = std::make_unique<Player>(std::make_unique<PeasantClass>());
    auto dungeon = std::make_unique<Dungeon>();
    auto graveyard = std::make_unique<Graveyard>();
    auto room = std::make_unique<Room>(std::vector<std::unique_ptr<Card>>{});
    std::vector<std::unique_ptr<Action>> actions;
    GameContext ctx(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions));

    ctx.get_player()->on_game_start(ctx);
    ctx.get_player()->set_current_life(5);

    // First potion
    ctx.consume_potion(std::make_unique<Potion>(suit::Hearts, face::_8)); // 6 + 2 = 8
    EXPECT_EQ(ctx.get_player()->current_life(), 13);

    // Second potion (should NOT work for Peasant)
    ctx.consume_potion(std::make_unique<Potion>(suit::Hearts, face::_4)); // 2 + 2 = 4
    EXPECT_EQ(ctx.get_player()->current_life(), 13); 
}

TEST(WizardTests, BanishMonsterActionAvailability)
{
    auto wizard_player = std::make_unique<Player>(std::make_unique<WizardClass>());
    auto peasant_player = std::make_unique<Player>(std::make_unique<PeasantClass>());
    
    auto dungeon = std::make_unique<Dungeon>();
    auto graveyard = std::make_unique<Graveyard>();
    auto room = std::make_unique<Room>(std::vector<std::unique_ptr<Card>>{});
    std::vector<std::unique_ptr<Action>> actions;
    
    GameContext ctx_wizard(std::move(dungeon), std::move(wizard_player), std::move(graveyard), std::move(room), std::vector<std::unique_ptr<Action>>{});
    
    auto dungeon2 = std::make_unique<Dungeon>();
    auto graveyard2 = std::make_unique<Graveyard>();
    auto room2 = std::make_unique<Room>(std::vector<std::unique_ptr<Card>>{});
    GameContext ctx_peasant(std::move(dungeon2), std::move(peasant_player), std::move(graveyard2), std::move(room2), std::vector<std::unique_ptr<Action>>{});

    BanishMonsterAction banish_action;
    Monster monster(suit::Clubs, face::_10);

    EXPECT_TRUE(banish_action.can_execute(ctx_wizard, &monster));
    EXPECT_FALSE(banish_action.can_execute(ctx_peasant, &monster));
}

TEST(WizardTests, BanishMonsterActionEffect)
{
    auto player = std::make_unique<Player>(std::make_unique<WizardClass>());
    auto dungeon = std::make_unique<Dungeon>();
    unsigned int initial_dungeon_size = dungeon->dungeon_size();
    
    auto graveyard = std::make_unique<Graveyard>();
    auto room = std::make_unique<Room>(std::vector<std::unique_ptr<Card>>{});
    std::vector<std::unique_ptr<Action>> actions;
    GameContext ctx(std::move(dungeon), std::move(player), std::move(graveyard), std::move(room), std::move(actions));

    auto monster = std::make_unique<Monster>(suit::Clubs, face::_A);
    Card* monster_ptr = monster.get();

    ctx.banish_monster(std::move(monster));

    EXPECT_EQ(ctx.get_dungeon()->dungeon_size(), initial_dungeon_size + 1);
    
    // Check if it's at the end
    const Card& last_card = ctx.get_dungeon()->card_at(ctx.get_dungeon()->dungeon_size() - 1);
    EXPECT_EQ(last_card.get_suit(), monster_ptr->get_suit());
    EXPECT_EQ(last_card.get_face(), monster_ptr->get_face());
}
