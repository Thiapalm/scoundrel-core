#include "dungeon.h"
#include "cards.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <ctime>

Dungeon::Dungeon()
{
    for (int i = 0; i < static_cast<int>(suit::InvalidSuit); ++i)
    {
        for (int j = 0; j < static_cast<int>(face::InvalidFace); ++j)
        {
            switch (static_cast<suit>(i))
            {
            case suit::Diamonds:
                dung.push_back(std::make_unique<Weapon>(static_cast<suit>(i), static_cast<face>(j)));
                break;
            case suit::Hearts:
                dung.push_back(std::make_unique<Potion>(static_cast<suit>(i), static_cast<face>(j)));
                break;
            case suit::Clubs:
            case suit::Spades:
                dung.push_back(std::make_unique<Monster>(static_cast<suit>(i), static_cast<face>(j)));
                break;
            default:
                break;
            }
        }
    }
}

void Dungeon::shuffle(unsigned int seed)
{
    std::mt19937 rng(seed == 0 ? std::random_device{}() : seed);
    std::ranges::shuffle(dung, rng);
}

std::unique_ptr<Card> Dungeon::get_card()
{
    if (dung.empty()) return nullptr;
    auto this_card = std::move(dung.front());
    dung.pop_front();
    return this_card;
}

void Dungeon::return_card(std::unique_ptr<Card> to_dungeon)
{
    dung.push_back(std::move(to_dungeon));
}

void Dungeon::return_top(std::unique_ptr<Card> to_dungeon)
{
    dung.push_front(std::move(to_dungeon));
}

unsigned Dungeon::dungeon_size() const
{
    return static_cast<unsigned>(dung.size());
}

void Dungeon::remove(std::vector<std::unique_ptr<Card>> &removal)
{
    std::erase_if(dung, [&](const std::unique_ptr<Card> &x)
                  { return std::ranges::any_of(removal, [&](const auto &it) { return *it == *x; }); });
}

const Card &Dungeon::card_at(size_t pos) const
{
    return *dung.at(pos);
}

void print_dungeon(const Dungeon &dg)
{
    std::cout << "{ ";
    for (unsigned i = 0; i < dg.dungeon_size(); ++i)
    {
        std::cout << dg.card_at(i);
    }
    std::cout << "}" << std::endl;
}
