#include <gtest/gtest.h>
#include "assets/player.h"
#include "classes/HealerClass.h"
#include "classes/WarriorClass.h"
#include "i18n/LocalizationManager.h"
#include <fstream>
#include <filesystem>

class PlayerLocalizationTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a temporary locales directory
        std::filesystem::create_directory("temp_locales_player");
        
        std::ofstream en_file("temp_locales_player/en.json");
        en_file << "{\n"
                << "  \"ui.class.healer_buff\": \"+1 Health gain\",\n"
                << "  \"ui.class.healer_debuff\": \"-5 Max Life\",\n"
                << "  \"ui.class.warrior_buff\": \"+1 Weapon Damage\",\n"
                << "  \"ui.class.warrior_debuff\": \"-1 Healing\"\n"
                << "}\n";
        en_file.close();

        auto& lm = LocalizationManager::instance();
        lm.load_locale("en", "temp_locales_player");
        lm.set_locale("en");
    }

    void TearDown() override {
        std::filesystem::remove_all("temp_locales_player");
    }
};

TEST_F(PlayerLocalizationTests, HealerLocalization) {
    Player player(std::make_unique<HealerClass>());
    GameContext dummy_ctx(nullptr, nullptr, nullptr, nullptr, {});
    player.on_game_start(dummy_ctx);

    auto buffs = player.getBuffs();
    auto debuffs = player.getDeBuffs();

    ASSERT_EQ(buffs.size(), 1);
    EXPECT_EQ(buffs[0], "+1 Health gain");

    ASSERT_EQ(debuffs.size(), 1);
    EXPECT_EQ(debuffs[0], "-5 Max Life");
}

TEST_F(PlayerLocalizationTests, HealerMaxLifePenalty) {
    Player player(std::make_unique<HealerClass>());
    GameContext dummy_ctx(nullptr, nullptr, nullptr, nullptr, {});
    
    EXPECT_EQ(player.get_max_health(), 20);
    player.on_game_start(dummy_ctx);
    EXPECT_EQ(player.get_max_health(), 15); // 20 - 5
}

TEST_F(PlayerLocalizationTests, HealerPotionBonus) {
    Player player(std::make_unique<HealerClass>());
    GameContext dummy_ctx(nullptr, nullptr, nullptr, nullptr, {});
    player.on_game_start(dummy_ctx);
    
    // Max health is now 15 (20 - 5)
    player.set_current_life(5);
    player.gain_life(5); // Should gain 5 + 1 = 6
    EXPECT_EQ(player.current_life(), 11);
}

TEST_F(PlayerLocalizationTests, ThemeSpecificClassNames) {
    auto& lm = LocalizationManager::instance();
    
    // Mock space and dungeon localizations
    std::ofstream en_file("temp_locales_player/en.json");
    en_file << "{\n"
            << "  \"ui.class.healer.dungeon\": \"Healer\",\n"
            << "  \"ui.class.healer.space\": \"Medic\",\n"
            << "  \"ui.class.warrior.dungeon\": \"Warrior\",\n"
            << "  \"ui.class.warrior.space\": \"Soldier\",\n"
            << "  \"ui.class.peasant.dungeon\": \"Peasant\",\n"
            << "  \"ui.class.peasant.space\": \"Civilian\"\n"
            << "}\n";
    en_file.close();
    lm.load_locale("en", "temp_locales_player");
    lm.set_locale("en");

    // Dungeon theme
    std::string theme = "dungeon";
    EXPECT_EQ(lm.get_string("ui.class.healer." + theme), "Healer");
    EXPECT_EQ(lm.get_string("ui.class.warrior." + theme), "Warrior");
    EXPECT_EQ(lm.get_string("ui.class.peasant." + theme), "Peasant");
    
    // Space theme
    theme = "space";
    EXPECT_EQ(lm.get_string("ui.class.healer.space"), "Medic");
    EXPECT_EQ(lm.get_string("ui.class.warrior.space"), "Soldier");
    EXPECT_EQ(lm.get_string("ui.class.peasant.space"), "Civilian");
    }

    TEST_F(PlayerLocalizationTests, PortugueseSpaceTheme) {
    auto& lm = LocalizationManager::instance();

    std::ofstream pt_file("temp_locales_player/pt.json");
    pt_file << "{\n"
            << "  \"ui.class.healer.dungeon\": \"Curandeiro\",\n"
            << "  \"ui.class.healer.space\": \"Médico\"\n"
            << "}\n";
    pt_file.close();

    lm.load_locale("pt", "temp_locales_player");
    lm.set_locale("pt");

    EXPECT_EQ(lm.get_string("ui.class.healer.space"), "Médico");
    }

TEST_F(PlayerLocalizationTests, WarriorLocalization) {
    Player player(std::make_unique<WarriorClass>());
    GameContext dummy_ctx(nullptr, nullptr, nullptr, nullptr, {});
    player.on_game_start(dummy_ctx);

    auto buffs = player.getBuffs();
    auto debuffs = player.getDeBuffs();

    ASSERT_EQ(buffs.size(), 1);
    EXPECT_EQ(buffs[0], "+1 Weapon Damage");

    ASSERT_EQ(debuffs.size(), 1);
    EXPECT_EQ(debuffs[0], "-1 Healing");
}
