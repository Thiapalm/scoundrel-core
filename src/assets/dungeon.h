#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include "cards.h"
#include <deque>
#include <vector>
#include <memory>

class Dungeon
{
private:
    std::deque<std::unique_ptr<Card>> dung;

public:
    Dungeon();
    void shuffle(unsigned int seed = 0);
    void remove(std::vector<std::unique_ptr<Card>> &removal);
    unsigned dungeon_size() const;
    std::unique_ptr<Card> get_card();
    void return_card(std::unique_ptr<Card> to_dungeon);
    void return_top(std::unique_ptr<Card> to_dungeon);
    const Card &card_at(size_t pos) const;
};

void print_dungeon(const Dungeon &dg);

#endif