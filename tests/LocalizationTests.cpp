#include <gtest/gtest.h>
#include "i18n/LocalizationManager.h"
#include <fstream>
#include <filesystem>

class LocalizationTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a temporary locales directory
        std::filesystem::create_directory("temp_locales");
        
        std::ofstream en_file("temp_locales/en.json");
        en_file << "{\n"
                << "  \"test.key\": \"English Value\",\n"
                << "  \"test.placeholder\": \"Hello {0}!\",\n"
                << "  \"test.multi\": \"{0} and {1}\"\n"
                << "}\n";
        en_file.close();

        std::ofstream pt_file("temp_locales/pt.json");
        pt_file << "{\n"
                << "  \"test.key\": \"Valor em Português\",\n"
                << "  \"test.placeholder\": \"Olá {0}!\"\n"
                << "}\n";
        pt_file.close();
    }

    void TearDown() override {
        std::filesystem::remove_all("temp_locales");
    }
};

TEST_F(LocalizationTests, LoadAndGetString) {
    auto& lm = LocalizationManager::instance();
    EXPECT_TRUE(lm.load_locale("en", "temp_locales"));
    lm.set_locale("en");
    EXPECT_EQ(lm.get_string("test.key"), "English Value");
}

TEST_F(LocalizationTests, LocaleSwitching) {
    auto& lm = LocalizationManager::instance();
    EXPECT_TRUE(lm.load_locale("en", "temp_locales"));
    EXPECT_TRUE(lm.load_locale("pt", "temp_locales"));
    
    lm.set_locale("en");
    EXPECT_EQ(lm.get_string("test.key"), "English Value");
    
    lm.set_locale("pt");
    EXPECT_EQ(lm.get_string("test.key"), "Valor em Português");
}

TEST_F(LocalizationTests, PlaceholderSubstitution) {
    auto& lm = LocalizationManager::instance();
    EXPECT_TRUE(lm.load_locale("en", "temp_locales"));
    lm.set_locale("en");
    
    EXPECT_EQ(lm.get_string("test.placeholder", {"World"}), "Hello World!");
    EXPECT_EQ(lm.get_string("test.multi", {"Alice", "Bob"}), "Alice and Bob");
}

TEST_F(LocalizationTests, FallbackToEnglish) {
    auto& lm = LocalizationManager::instance();
    EXPECT_TRUE(lm.load_locale("en", "temp_locales"));
    EXPECT_TRUE(lm.load_locale("pt", "temp_locales"));
    
    lm.set_locale("pt");
    // "test.multi" is only in English
    EXPECT_EQ(lm.get_string("test.multi", {"Alice", "Bob"}), "Alice and Bob");
}

TEST_F(LocalizationTests, MissingKeyReturnsKey) {
    auto& lm = LocalizationManager::instance();
    lm.set_locale("en");
    EXPECT_EQ(lm.get_string("non.existent.key"), "non.existent.key");
}
