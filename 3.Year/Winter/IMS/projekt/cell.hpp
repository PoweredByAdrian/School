#ifndef CELL_HPP
#define CELL_HPP

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

enum bDamage{
    psi_1 = 2,      //GREEN
    psi_20 = 4,     //RED
    psi_3 = 10,     //LIGHTGREEN
    psi_10 = 12,    //LIGHTRED
    psi_5 = 14,     //YELLOW
    none = 15,      //WHITE
} b_damage;

class Cell {
public:
    Cell(int x, int y, bDamage b_damage ) {
        setCell(x,y, b_damage);
    }
    Cell(void) {
        setCell(0,0,none);
    }
    void updateDamage(bDamage b_damage);
    void setCell(int x, int y, bDamage b_damage);
    bDamage b_damage;
    int x;
    int y;
};
#endif