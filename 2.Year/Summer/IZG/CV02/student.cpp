﻿/******************************************************************************
 * Laborator 02 - Zaklady pocitacove grafiky - IZG
 * isolony@fit.vutbr.cz
 *
 * $Id: $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * ipolasek@fit.vutbr.cz
 * ivelas@fit.vutbr.cz
 * itomesek@fit.vutbr.cz
 */

#include "student.h"
#include "globals.h"

// Header standardni knihovny obsahujici funci swap.
#include <memory>
// Zjednoduseni zapisu, napr. std::swap -> swap.
using namespace std;

/**
 * Provede vykresleni pixelu na dane pozici ve vystupnim rasteru. Pokud je
 * souradnice mimo hranice rasteru, potom tato funkce neprovede zadnou zmenu.
 */
S_RGBA getPixel(int x, int y)
{
    if (x >= 0 && y >= 0 && x < gWidth && y < gHeight)
    { return gFrameBuffer[y * gWidth + x]; }
    else
    { return makeBlackColor(); }
}

/** 
 * Vycte barvu pixelu na dane pozici ve vystupnim rasteru. Pokud je souradnice
 * mimo hranice rasteru, potom funkce vraci barvu (0, 0, 0, 0).
 */
void setPixel(int x, int y, S_RGBA color)
{
    if (x >= 0 && y >= 0 && x < gWidth && y < gHeight)
    { gFrameBuffer[y * gWidth + x] = color; }
}


////////////////////////////////////////////////////////////////////////////////
// Ukol za 2b
////////////////////////////////////////////////////////////////////////////////
#define FRAC_BITS 8
/* Vykresli usecku od [x1, y1] po [x2, y2] vcetne koncovych bodu */
void drawLine(int x1, int y1, int x2, int y2, S_RGBA color)
{
    /*
     * Doplnte do funkce na oznacena mista (#) kod tak, aby po stisku klavesy
     * T aplikace spravne vykreslila cely testovaci vzor se vsemi kvadranty: 
     *
     *   ╲  ┃  ╱
     *    ╲ ┃ ╱
     *  ━━━━╋━━━━
     *    ╱ ┃ ╲ 
     *   ╱  ┃  ╲ 
     */

    // Namisto "int dx = x2 - x1", lze v modernim c++ pouzit: 
    auto dx{ x2 - x1 };
    auto dy{ y2 - y1 };


    //#1
    if (x1 == x2 && y1 == y2) return;
    //#2
    if (abs(dx) >= abs(dy))
    {   
        //#3
        if (x1 > x2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        auto y{ y1 << FRAC_BITS };
        auto k{ (dy << FRAC_BITS) / dx };
        for (int x = x1; x <= x2; ++x)
        {
            setPixel(x, y >> FRAC_BITS, color);
            y += k;
        }
    }//#2
    else
    {
        //#3
        if (y1 > y2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        auto x{ x1 << FRAC_BITS };
        auto k{ (dx << FRAC_BITS) / dy };
        for (int y = y1; y <= y2; ++y) {
            setPixel(x >> FRAC_BITS, y, color);
            x += k;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Ukol za 1b
////////////////////////////////////////////////////////////////////////////////
void put8PixelsOfCircle(int x, int y, int s1, int s2, S_RGBA color)
{
    /*
     * Doplnte do funkce kod tak, aby po stisku klavesy T aplikace spravne
     * vykreslila testovaci vzor s kruznici ve vsech kvadrantech: 
     *
     * ╭────┳────╮
     * │    ┃    │
     * │    ┃    │
     * ┣━━━━╋━━━━┫
     * │    ┃    │
     * │    ┃    │
     * ╰────┻────╯
     */

    //#3
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            //#2
            setPixel(s1 + x, s2 + y, color);//#1
            setPixel(s1 + x, s2 - y, color);//#1
            swap(x, y);
        }
        x *= -1;
        y *= -1;
    }
}

/* Vykresli kruznici se stredem v [s1, s2] o polomeru radius */
void drawCircle(int s1, int s2, float radius, S_RGBA color)
{
    const auto r{ static_cast<int>(radius) };

    /* Zaciname na pozici [r, 0] a iterujeme pres nejmensi zmenu, tedy y! */
    auto x{ r };
    auto y{ 0 };

    auto P{ 1 - r };
    auto X2{ 2 - 2 * r };
    auto Y2{ 3 };

    while (x >= y) 
    {
        put8PixelsOfCircle(x, y, s1, s2, color);

        if (P >= 0) 
        {
            P += X2;
            X2 += 2;
            --x;
        }

        P += Y2;
        Y2 += 2;
        ++y;
    }
}

/*****************************************************************************/
/*****************************************************************************/