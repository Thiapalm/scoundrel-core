#pragma once
#include <concepts>
#include "assets/cards.h"

template<typename T>
concept Combatant = requires(T a) {
    { a.get_damage() } -> std::same_as<int>;
    { a.getType() } -> std::same_as<CardType>;
};

template<typename T>
concept Consumable = requires(T a) {
    { a.drink_potion() } -> std::same_as<int>;
    { a.getType() } -> std::same_as<CardType>;
};

template<typename T>
concept Equippable = requires(T a) {
    { a.weapon_damage() } -> std::same_as<int>;
    { a.getType() } -> std::same_as<CardType>;
};
