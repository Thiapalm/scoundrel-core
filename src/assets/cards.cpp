#include "cards.h"
#include <sstream>

std::string Card::to_string() const
{
    return "[" + transform_face(my_face) + "-" + transform_suit(my_suit) + "]";
}

std::ostream &operator<<(std::ostream &os, const Card &card)
{
    os << card.to_string() << " ";
    return os;
}

int Monster::player_damage(int weapon_value)
{
    return damage - weapon_value > 0 ? damage - weapon_value : 0;
}

int Monster::get_damage() const
{
    return damage;
}

int Potion::drink_potion()
{
    return restore;
}

int Weapon::weapon_damage()
{
    return damage;
}

void Weapon::kill_monster(std::unique_ptr<Monster> enemy, int effective_damage)
{
    const int stored_damage = effective_damage >= 0 ? effective_damage : enemy->get_damage();
    monsters.push(std::move(enemy));
    monster_effective_damages.push(stored_damage);
}

const bool Weapon::has_monster()
{
    return !monsters.empty();
}

const int Weapon::last_monster_damage()
{
    if (monster_effective_damages.empty())
        return 0;
    return monster_effective_damages.top();
}

const Monster *Weapon::get_last_killed_monster() const
{
    if (monsters.empty())
        return nullptr;
    return monsters.top().get();
}

std::unique_ptr<Monster> Weapon::remove_monster()
{
    auto last = std::move(monsters.top());
    monsters.pop();
    if (!monster_effective_damages.empty())
    {
        monster_effective_damages.pop();
    }
    return last;
}

int Weapon::number_of_monsters()
{
    return monsters.size();
}

std::string transform_face(face val)
{
    switch (val)
    {
    case face::_2:
        return "2";
        break;
    case face::_3:
        return "3";
        break;
    case face::_4:
        return "4";
        break;
    case face::_5:
        return "5";
        break;
    case face::_6:
        return "6";
        break;
    case face::_7:
        return "7";
        break;
    case face::_8:
        return "8";
        break;
    case face::_9:
        return "9";
        break;
    case face::_10:
        return "10";
        break;
    case face::_J:
        return "J";
        break;
    case face::_Q:
        return "Q";
        break;
    case face::_K:
        return "K";
        break;
    case face::_A:
        return "A";
        break;
    case face::_JK:
        return "JK";
        break;
    default:
        break;
    }
    return " ";
}

std::string transform_suit(suit value)
{
    switch (value)
    {
    case suit::Diamonds:
        return "♦";
        break;
    case suit::Clubs:
        return "♣";
        break;
    case suit::Hearts:
        return "♥";
        break;
    case suit::Spades:
        return "♠";
        break;
    default:
        break;
    }
    return " ";
}
