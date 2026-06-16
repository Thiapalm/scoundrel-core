#pragma once
#include "HealerClass.h"
#include "PeasantClass.h"
#include "WarriorClass.h"
#include "WizardClass.h"

enum class PlayerClassType { PEASANT, HEALER, WARRIOR, WIZARD  };

inline std::unique_ptr<Player> create_player(PlayerClassType type) {
    switch (type) {
        case PlayerClassType::HEALER:
                return std::make_unique<Player>(std::make_unique<HealerClass>());
        case PlayerClassType::WARRIOR:
                return std::make_unique<Player>(std::make_unique<WarriorClass>());
        case PlayerClassType::WIZARD:
                return std::make_unique<Player>(std::make_unique<WizardClass>());
        default:
            return std::make_unique<Player>(std::make_unique<PeasantClass>());
    }
}