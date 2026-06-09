#ifndef _ROOM_H_
#define _ROOM_H_

#include "cards.h"
#include <vector>
#include <memory>

class Room
{
private:
    std::vector<std::unique_ptr<Card>> cards;

public:
    explicit Room(std::vector<std::unique_ptr<Card>> cards) : cards(std::move(cards)) {}
    Room(const Room &) = delete;
    Room &operator=(const Room &) = delete;
    Room(Room &&) noexcept = default;
    Room &operator=(Room &&) noexcept = default;
    std::unique_ptr<Card> pop_card(int pos);
    std::unique_ptr<Card> pop_last();
    int cards_in_room() const;
    const Card &look_card(int pos);
    const bool room_empty() const;
};

#endif