#include "cell.hpp"

void Cell::updateDamage(bDamage b_damage) {
    this->b_damage = b_damage;
}

void Cell::setCell(int x, int y, bDamage b_damage) {
    this->b_damage = b_damage;
    this->x = x;
    this->y = y;
}