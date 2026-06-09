#pragma once
#include <string>
#include <vector>
#include <functional>

namespace scoundrel
{

    struct LeaderboardEntry
    {
        int rank;
        std::string name;
        std::string player_class;
        int score;
        bool is_current_player;
        bool is_founder;
    };

    enum class LeaderboardType
    {
        STANDARD,
        EXTENDED
    };

    class LeaderboardManager
    {
    public:
        virtual ~LeaderboardManager() = default;

        virtual void initialize() = 0;
        virtual void sign_in(bool manual = false) = 0;
        virtual bool is_signed_in() const = 0;

        virtual void submit_score(LeaderboardType type, int score, const std::string &player_class = "") = 0;

        using FetchCallback = std::function<void(bool success, const std::vector<LeaderboardEntry> &top_scores, int player_rank)>;
        virtual void fetch_scores(LeaderboardType type, FetchCallback callback) = 0;
    };

} // namespace scoundrel
