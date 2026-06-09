#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <memory>

enum class suit
{
    Diamonds,
    Spades,
    Clubs,
    Hearts,
    InvalidSuit
};

enum class face
{
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _10,
    _J,
    _Q,
    _K,
    _A,
    _JK,
    InvalidFace
};

enum class CardType
{
    Weapon,
    Monster,
    Potion
};

class Card
{
protected:
    face my_face;
    suit my_suit;

public:
    Card() : Card(suit::InvalidSuit, face::InvalidFace) {}
    Card(suit my_suit, face my_face) : my_suit{my_suit}, my_face{my_face} {}
    Card(const Card &) = delete;
    Card &operator=(const Card &) = delete;
    Card(Card &&) noexcept = default;
    Card &operator=(Card &&) noexcept = default;
    virtual ~Card() = default;
    virtual CardType getType() const = 0;
    std::string to_string() const;
    suit get_suit() const
    {
        return my_suit;
    }

    face get_face() const
    {
        return my_face;
    }

    bool operator==(const Card &other) const
    {
        return (this->my_face == other.my_face && this->my_suit == other.my_suit);
    }
};

class Monster : public Card
{
private:
    int damage;

public:
    Monster(suit my_suit, face my_face) : Card(my_suit, my_face), damage{static_cast<int>(my_face) + 2} {}
    Monster(const Monster &) = delete;
    Monster &operator=(const Monster &) = delete;
    Monster(Monster &&) noexcept = default;
    Monster &operator=(Monster &&) noexcept = default;
    int player_damage(int weapon_value);
    int get_damage() const;
    CardType getType() const override { return CardType::Monster; }
};

class Potion : public Card
{
private:
    int restore;

public:
    Potion(suit my_suit, face my_face) : Card(my_suit, my_face), restore{static_cast<int>(my_face) + 2} {}
    int drink_potion();
    CardType getType() const override { return CardType::Potion; }
};

class Weapon : public Card
{
private:
    int damage;
    std::stack<std::unique_ptr<Monster>> monsters;
    int number_of_killed_monsters;

public:
    Weapon() : Card(), damage{0}, number_of_killed_monsters{0} {}
    Weapon(suit my_suit, face my_face) : Card(my_suit, my_face), damage{static_cast<int>(my_face) + 2}, number_of_killed_monsters{0} {}
    Weapon(const Weapon &) = delete;
    Weapon &operator=(const Weapon &) = delete;
    Weapon(Weapon &&) noexcept = default;
    Weapon &operator=(Weapon &&) noexcept = default;
    CardType getType() const override { return CardType::Weapon; }
    int weapon_damage();
    void kill_monster(std::unique_ptr<Monster> enemy);
    const bool has_monster();
    const int last_monster_damage();
    const Monster* get_last_killed_monster() const;
    std::unique_ptr<Monster> remove_monster();
    int number_of_monsters();
};

std::string transform_face(face val);
std::string transform_suit(suit value);
std::ostream &operator<<(std::ostream &os, const Card &card);
