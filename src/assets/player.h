#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cards.h"
#include "playerclass.h"
#include "passive_ability.h"
#include "i18n/LocalizationManager.h"
#include <stack>
#include <vector>
#include <memory>
#include <string>

class Player
{
private:
    int life;
    int max_life;
    std::vector<std::unique_ptr<PassiveAbility>> passives;

public:
    std::unique_ptr<PlayerClass> player_class;
    Player(std::unique_ptr<PlayerClass> pc) : life{20}, max_life{20}, player_class(std::move(pc)) {}
    Player(int max_life = 20, std::unique_ptr<PlayerClass> pclass = nullptr) : life{max_life}, max_life{max_life}, player_class(std::move(pclass)) {}
    
    std::string getName() { if (player_class) return player_class->name(); return "";}
    std::vector<std::string> getBuffs() const {
        if (!player_class) return {};
        auto& lm = LocalizationManager::instance();
        std::vector<std::string> localized;
        localized.reserve(player_class->buffs.size());
        for (auto const& buff : player_class->buffs) {
            if (!buff.empty() && buff.rfind("ui.", 0) == 0) {
                localized.push_back(lm.get_string(buff));
            } else {
                localized.push_back(buff);
            }
        }
        return localized;
    }
    std::vector<std::string> getDeBuffs() const {
        if (!player_class) return {};
        auto& lm = LocalizationManager::instance();
        std::vector<std::string> localized;
        localized.reserve(player_class->debuffs.size());
        for (auto const& debuff : player_class->debuffs) {
            if (!debuff.empty() && debuff.rfind("ui.", 0) == 0) {
                localized.push_back(lm.get_string(debuff));
            } else {
                localized.push_back(debuff);
            }
        }
        return localized;
    }
    int get_max_health() { return max_life; }
    void set_max_health(int val) { max_life = val; }
    int modify_outgoing_damage(int base_damage);
    int modify_incoming_damage(int base_damage);
    void on_pre_room(GameContext &ctx);
    void on_game_start(GameContext &ctx);
    int current_life();
    int gain_life(int val);
    int lose_life(int val);
    void set_current_life(int val);

    std::vector<std::string> getPassiveDescriptions() const;
    void add_passive(std::unique_ptr<PassiveAbility> passive);
    void trigger_passives(GameContext& ctx, PassiveTrigger trigger);
    const std::vector<std::unique_ptr<PassiveAbility>>& get_passives() const { return passives; }
};

#endif