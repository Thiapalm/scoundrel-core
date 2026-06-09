#include "graveyard.h"
#include "cards.h"
#include <algorithm>
#include <iostream>

unsigned Graveyard::graveyard_size() const
{
    return static_cast<unsigned>(graves.size());
}

void Graveyard::put_into(std::unique_ptr<Card> to_graveyard)
{
    graves.push_front(std::move(to_graveyard));
}

const Card &Graveyard::card_at(size_t pos) const
{
    return *graves.at(pos);
}

void print_graveyard(const Graveyard &gv)
{
    std::cout << "{ ";
    for (unsigned i = 0; i < gv.graveyard_size(); ++i)
    {
        std::cout << gv.card_at(i);
    }
    std::cout << "}" << std::endl;
}