#include <gtest/gtest.h>
#include "GameLogic.h"
#include "DefaultGameContextFactory.h"
#include "MockUI.h"
#include "assets/cards.h"

TEST(GameLoopTests, SeededQuickGame) {
    // We want a very short game or a specific sequence.
    // Seed 12345 might give us something predictable.
    unsigned int seed = 12345;
    
    auto ui = std::make_unique<MockUI>();
    
    // Setup 500 pseudo-random choices to ensure game reaches conclusion
    std::mt19937 rng(42);
    for(int i=0; i<500; ++i) {
        ui->action_choices.push(rng()); 
        ui->card_choices.push(rng());   
    }
    
    auto game = GameLogic::create(GameType::DEFAULT);
    
    // We need to re-create context with seed because GameLogic::create uses random
    auto factory = std::make_unique<DefaultGameContextFactory>();
    game->set_context(factory->create_game_context(seed));
    game->set_interface(ui.get());
    
    // This will run until GamePhase::END_GAME
    game->run_game_loop();
    
    EXPECT_TRUE(ui->victory_shown || ui->defeat_shown);
    std::cout << "Game ended with score: " << ui->final_score << std::endl;
}

TEST(GameLoopTests, UserQuitMidGame) {
    auto ui = std::make_unique<MockUI>();
    
    // Play one action then quit
    ui->action_choices.push(0);  // First action
    ui->card_choices.push(1);    // First card
    ui->action_choices.push(-1); // Signal QUIT
    
    auto game = GameLogic::create(GameType::DEFAULT);
    auto factory = std::make_unique<DefaultGameContextFactory>();
    game->set_context(factory->create_game_context(12345));
    game->set_interface(ui.get());
    
    game->run_game_loop();
    
    EXPECT_TRUE(ui->defeat_shown);
    EXPECT_FALSE(ui->victory_shown);
}

TEST(GameLoopTests, VictoryPotionBonus) {
    auto ui = std::make_unique<MockUI>();
    
    // Create context directly
    auto factory = std::make_unique<DefaultGameContextFactory>();
    auto ctx_ptr = factory->create_game_context(12345);
    GameContext* ctx = ctx_ptr.get();
    
    auto game = GameLogic::create(GameType::DEFAULT);
    game->set_context(std::move(ctx_ptr));
    game->set_interface(ui.get());
    
    // Set up victory state
    ctx->set_game_over(true); // victory = true by default in set_game_over
    
    // Simulate drinking a potion (5 of hearts = 3 + 2 = 5 healing)
    auto potion = std::make_unique<Potion>(suit::Hearts, face::_5);
    ctx->consume_potion(std::move(potion));
    
    // Set player to max health (simulate the potion brought them to max)
    ctx->get_player()->set_max_health(20);
    ctx->get_player()->gain_life(20); // Set to 20
    
    // Run end_game
    game->end_game(*ctx);
    
    // Should have bonus: 20 + 5 = 25
    EXPECT_EQ(ui->final_score, 25);
}
