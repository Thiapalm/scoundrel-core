
#ifndef _GRAVEYARD_H_
#define _GRAVEYARD_H_

#include "cards.h"
#include <deque>
#include <vector>
#include <memory>

class Graveyard
{
private:
    std::deque<std::unique_ptr<Card>> graves;

public:
    Graveyard() = default;
    unsigned graveyard_size() const;
    void put_into(std::unique_ptr<Card> to_graveyard);
    const Card &card_at(size_t pos) const;
};

void print_graveyard(const Graveyard &grave);

#endif