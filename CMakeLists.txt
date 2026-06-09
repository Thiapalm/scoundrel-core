cmake_minimum_required(VERSION 3.16)
project(ScoundrelCore VERSION 0.1 LANGUAGES CXX)

# Option to enable SDL linkage for consumers that need it (mobile/TUI).
option(BUILD_WITH_SDL "Enable SDL linkage for platforms that require it" OFF)

# 1. ScoundrelCore Library (Pure Logic)
set(CORE_SOURCES
    src/GameLogic.cpp
    src/gamecontext.cpp
    src/DefaultGameLogic.cpp
    src/ExtendedGameLogic.cpp
    src/i18n/LocalizationManager.cpp
    src/PurchaseRegistry.cpp
)

set(ASSET_SOURCES
    src/assets/cards.cpp
    src/assets/dungeon.cpp
    src/assets/graveyard.cpp
    src/assets/player.cpp
    src/assets/room.cpp
    src/assets/Passives.cpp
)

set(ACTION_SOURCES
    src/actions/AttachWeaponAction.cpp
    src/actions/AttackMonsterAction.cpp
    src/actions/AttackMonsterNoWeaponAction.cpp
    src/actions/DrinkPotionAction.cpp
    src/actions/NewRoomAction.cpp
    src/actions/SkipRoomAction.cpp
)

set(CORE_HEADERS
    src/GameLogic.h
    src/gamecontext.h
    src/GameContextFactory.h
    src/DefaultGameLogic.h
    src/ExtendedGameLogic.h
    src/DefaultGameContextFactory.h
    src/PlayerClassFactory.h
    src/AuthManager.h
    src/BillingManager.h
    src/LeaderboardManager.h
    src/PersistenceManager.h
    src/concepts.h
    src/i18n/LocalizationManager.h
    src/assets/cards.h
    src/assets/dungeon.h
    src/assets/graveyard.h
    src/assets/player.h
    src/assets/room.h
    src/assets/passive_ability.h
    src/assets/Passives.h
    src/actions/action.h
    src/actions/AttachWeaponAction.h
    src/actions/AttackMonsterAction.h
    src/actions/AttackMonsterNoWeaponAction.h
    src/actions/DrinkPotionAction.h
    src/actions/NewRoomAction.h
    src/actions/SkipRoomAction.h
    src/classes/HealerClass.h
    src/classes/PeasantClass.h
    src/classes/WarriorClass.h
    src/classes/playerclass.h
    src/ui/UserInterface.h
    src/PurchaseRegistry.h
)

add_library(ScoundrelCore STATIC
    ${CORE_SOURCES}
    ${ASSET_SOURCES}
    ${ACTION_SOURCES}
    ${CORE_HEADERS}
)

target_compile_features(ScoundrelCore PUBLIC cxx_std_20)

# Include directories
target_include_directories(ScoundrelCore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/actions>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/assets>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/classes>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/ui>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/i18n>
)

# SDL linkage is optional; consumers (mobile/TUI) enable when needed
if(BUILD_WITH_SDL)
    if(ANDROID)
        target_link_libraries(ScoundrelCore PUBLIC SDL2 SDL2_mixer SDL2_image log)
    else()
        target_link_libraries(ScoundrelCore PUBLIC SDL2-static SDL2_mixer SDL2_image)
    endif()
    target_compile_definitions(ScoundrelCore PUBLIC BUILD_WITH_SDL=1)
endif()

# Export for tests and other modules
if(NOT ANDROID AND NOT DEFINED BUILD_AS_SUBMODULE)
    add_subdirectory(tests)
endif()
