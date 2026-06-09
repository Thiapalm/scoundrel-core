#include <gtest/gtest.h>
#include "i18n/LocalizationManager.h"
#include <vector>
#include <string>
#include <filesystem>

class TutorialLocalizationTests : public ::testing::Test {
protected:
    void SetUp() override {
        auto repo_root = std::filesystem::absolute(std::filesystem::path(__FILE__)).parent_path().parent_path();
        auto locales_path = repo_root / "assets" / "locales";

        auto& lm = LocalizationManager::instance();
        lm.load_locale("en", locales_path);
        lm.load_locale("es", locales_path);
        lm.load_locale("pt", locales_path);
    }
};

TEST_F(TutorialLocalizationTests, AllTutorialKeysPresentInAllLanguages) {
    auto& lm = LocalizationManager::instance();
    std::vector<std::string> languages = {"en", "es", "pt"};
    std::vector<std::string> keys = {
        "ui.tutorial.title",
        "ui.tutorial.setup_title",
        "ui.tutorial.setup_text",
        "ui.tutorial.rules_title",
        "ui.tutorial.rules_text",
        "ui.tutorial.scoring_title",
        "ui.tutorial.scoring_text",
        "ui.tutorial.gameplay_title",
        "ui.tutorial.gameplay_text",
        "ui.tutorial.combat_title",
        "ui.tutorial.combat_text",
        "ui.tutorial.back",
        "ui.splash.tutorial"
    };

    for (const auto& lang : languages) {
        lm.set_locale(lang);
        for (const auto& key : keys) {
            std::string translated = lm.get_string(key);
            EXPECT_NE(translated, key) << "Key '" << key << "' missing in locale '" << lang << "'";
            EXPECT_FALSE(translated.empty()) << "Key '" << key << "' is empty in locale '" << lang << "'";
        }
    }
}
