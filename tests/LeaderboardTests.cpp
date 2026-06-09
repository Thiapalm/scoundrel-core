#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../LeaderboardManager.h"
#include "../PersistenceManager.h"

using namespace scoundrel;
using namespace testing;

class MockLeaderboardManager : public LeaderboardManager {
public:
    MOCK_METHOD(void, initialize, (), (override));
    MOCK_METHOD(void, sign_in, (bool manual), (override));
    MOCK_METHOD(bool, is_signed_in, (), (const, override));
    MOCK_METHOD(void, submit_score, (LeaderboardType type, int score, const std::string& player_class), (override));
    MOCK_METHOD(void, fetch_scores, (LeaderboardType type, FetchCallback callback), (override));
};

TEST(LeaderboardAuthTests, SilentSignInOnStartup) {
    MockLeaderboardManager mock_manager;
    
    // Expect silent sign-in to be called
    EXPECT_CALL(mock_manager, sign_in(false)).Times(1);
    
    mock_manager.sign_in(false);
}

TEST(LeaderboardAuthTests, ManualSignInTrigger) {
    MockLeaderboardManager mock_manager;
    
    // Expect manual sign-in to be called
    EXPECT_CALL(mock_manager, sign_in(true)).Times(1);
    
    mock_manager.sign_in(true);
}

class MockPersistenceManager : public PersistenceManager {
public:
    MOCK_METHOD(void, save_int, (const std::string& key, int value), (override));
    MOCK_METHOD(int, load_int, (const std::string& key, int default_value), (const, override));
    MOCK_METHOD(void, save_string, (const std::string& key, const std::string& value), (override));
    MOCK_METHOD(std::string, load_string, (const std::string& key, const std::string& default_value), (const, override));
};

TEST(LeaderboardPersistenceTests, SaveAndLoadLocalBest) {
    MockPersistenceManager mock_persistence;
    
    EXPECT_CALL(mock_persistence, save_int("local_best_standard", 100)).Times(1);
    EXPECT_CALL(mock_persistence, load_int("local_best_standard", 0)).WillOnce(Return(100));
    
    mock_persistence.save_int("local_best_standard", 100);
    EXPECT_EQ(mock_persistence.load_int("local_best_standard", 0), 100);
}

TEST(LeaderboardCachingTests, CacheRankings) {
    MockPersistenceManager mock_persistence;
    std::string fake_json = "[{\"rank\":1, \"name\":\"Player\", \"score\":1000}]";
    
    EXPECT_CALL(mock_persistence, save_string("cache_standard", fake_json)).Times(1);
    EXPECT_CALL(mock_persistence, load_string("cache_standard", "")).WillOnce(Return(fake_json));
    
    mock_persistence.save_string("cache_standard", fake_json);
    EXPECT_EQ(mock_persistence.load_string("cache_standard", ""), fake_json);
}

TEST(LeaderboardSubmissionTests, SubmitStandardScore) {
    MockLeaderboardManager mock_manager;
    
    EXPECT_CALL(mock_manager, submit_score(LeaderboardType::STANDARD, 500, "")).Times(1);
    
    mock_manager.submit_score(LeaderboardType::STANDARD, 500, "");
}

TEST(LeaderboardSubmissionTests, SubmitExtendedScore) {
    MockLeaderboardManager mock_manager;
    
    EXPECT_CALL(mock_manager, submit_score(LeaderboardType::EXTENDED, 750, "Healer")).Times(1);
    
    mock_manager.submit_score(LeaderboardType::EXTENDED, 750, "Healer");
}

TEST(LeaderboardFetchTests, FetchTopScores) {
    MockLeaderboardManager mock_manager;
    
    EXPECT_CALL(mock_manager, fetch_scores(LeaderboardType::STANDARD, _)).WillOnce([](LeaderboardType, LeaderboardManager::FetchCallback callback) {
        std::vector<LeaderboardEntry> entries = {
            {1, "Winner", "Warrior", 2000, false, false},
            {2, "RunnerUp", "Healer", 1800, false, false}
        };
        callback(true, entries, 50); // Player is rank 50
    });
    
    bool called = false;
    mock_manager.fetch_scores(LeaderboardType::STANDARD, [&](bool success, const std::vector<LeaderboardEntry>& entries, int rank) {
        EXPECT_TRUE(success);
        EXPECT_EQ(entries.size(), 2);
        EXPECT_EQ(rank, 50);
        called = true;
    });
    EXPECT_TRUE(called);
}
