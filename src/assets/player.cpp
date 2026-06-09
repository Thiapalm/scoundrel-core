#include "player.h"
#include "i18n/LocalizationManager.h"
#include <algorithm>

void Player::on_game_start(GameContext &ctx) {
    if (player_class)
        player_class->on_game_start(*this, ctx);
}

int Player::current_life()
{
    return life;
}

int Player::gain_life(int val)
{   
    int modified{val};
    if(player_class)
        modified = player_class->on_gain_life(*this, val);
    life = std::min(life + modified, max_life);
    return life;
}

int Player::modify_outgoing_damage(int base_damage) {
    if (player_class)
        return player_class->modify_outgoing_damage(base_damage);
    return base_damage;
}

int Player::modify_incoming_damage(int base_damage) {
    if (player_class)
        return player_class->modify_incoming_damage(*this, base_damage);
    return base_damage;
}

void Player::on_pre_room(GameContext &ctx) {
    trigger_passives(ctx, PassiveTrigger::START_ROOM);
    if (player_class)
        player_class->on_pre_room(*this, ctx);
}

int Player::lose_life(int val)
{
    int modified = modify_incoming_damage(val);
    if (player_class) 
        modified = player_class->on_lose_life(*this, modified);
    life -= modified;
    return life;
}

void Player::set_current_life(int val) {
    life = val;
}

void Player::add_passive(std::unique_ptr<PassiveAbility> passive) {
    passives.push_back(std::move(passive));
}

void Player::trigger_passives(GameContext& ctx, PassiveTrigger trigger) {
    for (const auto& passive : passives) {
        passive->execute(ctx, trigger);
    }
}

std::vector<std::string> Player::getPassiveDescriptions() const {
    std::vector<std::string> descriptions;
    auto& lm = LocalizationManager::instance();
    for (const auto& p : passives) {
        std::string name = p->name();
        std::string description = p->description();
        if (!name.empty() && name.rfind("ui.", 0) == 0) {
            name = lm.get_string(name);
        }
        if (!description.empty() && description.rfind("ui.", 0) == 0) {
            description = lm.get_string(description);
        }
        descriptions.push_back(name + ": " + description);
    }
    return descriptions;
}
