#include "room.h"

std::unique_ptr<Card> Room::pop_card(int pos)
{
    auto card = std::move(cards.at(pos));
    cards.erase(cards.begin() + pos);
    return card;
}

std::unique_ptr<Card> Room::pop_last()
{
    //std::cout << "Pop last called. Cards before: " << cards.size() << std::endl;
    if (cards.empty())
        return nullptr;

    auto card = std::move(cards.back());
    cards.pop_back();
    
    //std::cout << "Cards after: " << cards.size() << std::endl;
    return card;
}

int Room::cards_in_room() const
{
    return cards.size();
}

const Card &Room::look_card(int pos)
{
    return *cards.at(pos);
}

const bool Room::room_empty() const
{
    return cards.empty();
}