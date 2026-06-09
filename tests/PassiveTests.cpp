#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "assets/player.h"
#include "assets/passive_ability.h"
#include "assets/Passives.h"
#include "gamecontext.h"
#include "DefaultGameContextFactory.h"
#include "WarriorClass.h"
#include "HealerClass.h"
#include "DefaultGameLogic.h"
#include "MockUI.h"
#include "TestGameLogic.h"

class MockPassive : public PassiveAbility {
public:
    MOCK_METHOD(std::string, name, (), (const, override));
    MOCK_METHOD(std::string, description, (), (const, override));
    MOCK_METHOD(void, execute, (GameContext& ctx, PassiveTrigger trigger), (override));
};

TEST(PassiveAbilityTests, PlayerStoresAndTriggersPassives) {
    DefaultGameContextFactory factory;
    auto ctx = factory.create_game_context();
    auto player = std::make_unique<Player>();
    
    auto mock1 = std::make_unique<MockPassive>();
    auto mock2 = std::make_unique<MockPassive>();
    
    EXPECT_CALL(*mock1, execute(testing::_, PassiveTrigger::START_TURN)).Times(1);
    EXPECT_CALL(*mock2, execute(testing::_, PassiveTrigger::START_TURN)).Times(1);
    
    player->add_passive(std::move(mock1));
    player->add_passive(std::move(mock2));
    
    player->trigger_passives(*ctx, PassiveTrigger::START_TURN);
}

TEST(PassiveAbilityTests, GameLogicTriggersAllPhases) {
    DefaultGameContextFactory factory;
    auto ctx = factory.create_game_context(12345); // Seeded to have cards in room
    
    auto mock_passive = std::make_unique<MockPassive>();
    auto* ptr = mock_passive.get();
    
    ctx->get_player()->add_passive(std::move(mock_passive));
    
    TestDefaultGameLogic logic;
    GMockUI ui;
    logic.set_interface(&ui);
    
    EXPECT_CALL(*ptr, execute(testing::_, PassiveTrigger::START_TURN)).Times(testing::AtLeast(1));
    EXPECT_CALL(*ptr, execute(testing::_, PassiveTrigger::BEFORE_COMBAT)).Times(testing::AtLeast(1));
    EXPECT_CALL(*ptr, execute(testing::_, PassiveTrigger::AFTER_COMBAT)).Times(testing::AtLeast(1));
    EXPECT_CALL(*ptr, execute(testing::_, PassiveTrigger::END_TURN)).Times(testing::AtLeast(1));
    
    EXPECT_CALL(ui, display_game_state(testing::_)).Times(testing::AtLeast(1));
    EXPECT_CALL(ui, display_valid_actions(testing::_)).Times(testing::AtLeast(1));
    
    // Setup expectations for combat
    // We need to return an index that is valid for current room.
    // Standard game has 4 cards. Let's pick 0 (first action) and card 1.
    testing::Expectation first_call = EXPECT_CALL(ui, request_action_index(testing::_))
        .WillOnce(testing::Return(0)); 
    
    EXPECT_CALL(ui, request_action_index(testing::_))
        .After(first_call)
        .WillRepeatedly(testing::Return(std::nullopt));

    EXPECT_CALL(ui, request_card_index(testing::_)).WillRepeatedly(testing::Return(1));
    // EXPECT_CALL(ui, show_defeat(testing::_, testing::_)).Times(1); // Removed as it might show victory if survived
    EXPECT_CALL(ui, show_victory(testing::_)).Times(testing::AtMost(1));
    EXPECT_CALL(ui, show_defeat(testing::_, testing::_)).Times(testing::AtMost(1));
    EXPECT_CALL(ui, show_error(testing::_)).Times(testing::AnyNumber());

    // initialize_game overwrites player, so we need to add passive AFTER or use create_player hook
    // Let's just call the phases directly on our already setup context
    // logic.initialize_game(*ctx); // Don't call this, it resets player
    
    logic.start_turn(*ctx);
    logic.pre_combat(*ctx);
    logic.combat(*ctx);
    logic.post_combat(*ctx);
    logic.terminate_turn(*ctx);
    logic.end_game(*ctx);
}

TEST(PassiveAbilityTests, HealPassiveTriggersOnStartRoom) {
    DefaultGameContextFactory factory;
    auto ctx = factory.create_game_context();
    auto player = ctx->get_player();
    
    player->set_max_health(20);
    player->set_current_life(15);
    
    HealPassive heal;
    heal.execute(*ctx, PassiveTrigger::START_ROOM);
    
    EXPECT_EQ(player->current_life(), 16);
}

TEST(PassiveAbilityTests, WarriorDamageResistWorks) {
    auto player = std::make_unique<Player>(std::make_unique<WarriorClass>());
    EXPECT_EQ(player->modify_incoming_damage(5), 4);
    EXPECT_EQ(player->modify_incoming_damage(1), 1);
}
