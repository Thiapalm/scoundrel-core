#pragma once
#include "HealerClass.h"
#include "PeasantClass.h"
#include "WarriorClass.h"

enum class PlayerClassType { PEASANT, HEALER, WARRIOR  };

inline std::unique_ptr<Player> create_player(PlayerClassType type) {
    switch (type) {
        case PlayerClassType::HEALER:
                return std::make_unique<Player>(std::make_unique<HealerClass>());
        case PlayerClassType::WARRIOR:
                return std::make_unique<Player>(std::make_unique<WarriorClass>());
        default:
            return std::make_unique<Player>(std::make_unique<PeasantClass>());
    }
}