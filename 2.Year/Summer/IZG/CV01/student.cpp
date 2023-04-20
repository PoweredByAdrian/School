/**
 * IZG - Zaklady pocitacove grafiky - FIT VUT
 * Lab 01 - Redukce barevneho prostoru
 *
 * Copyright (c) 2022 by Michal Vlnas, ivlnas@fit.vutbr.cz
 *
 * Tato sablona slouzi vyhradne pro studijni ucely, sireni kodu je bez vedomi autora zakazano.
 */

#include "base.h"

void ImageTransform::grayscale()
{
    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            auto p = getPixel(x, y);
            uint8_t value = std::round(p.r * 0.299 + p.g * 0.587 + p.b * 0.114);
            
            setPixel(x, y, RGB(value));
        }
    }
}

void ImageTransform::threshold()
{
    grayscale();

    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            auto p = getPixel(x, y);
            p.r > cfg->threshold ? setPixel(x, y, COLOR_WHITE) : setPixel(x, y, COLOR_BLACK);
        }
    }
}

void ImageTransform::randomDithering()
{
    grayscale();

    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            auto p = getPixel(x, y);
            uint8_t value = p.r > getRandom() ? 255 : 0;

            setPixel(x, y, RGB(value));
        }
    }
}

void ImageTransform::orderedDithering()
{
    grayscale();

    int M[] = {
    0, 204, 51, 255,
    68, 136, 187, 119,
    34, 238, 17, 221,
    170, 102, 153, 85
    };

    int i, j;
    for (int x = 0; x < cfg->width; x++) {
        for (int y = 0; y < cfg->height; y++) {
            i = x % m_side;
            j = y % m_side;
            uint16_t value = getPixel(x, y).r;

            if (value > M[i + j * m_side]) {
                setPixel(x, y, COLOR_WHITE);
            }
            else {
                setPixel(x, y, COLOR_BLACK);
            }
        }
    }
}

void ImageTransform::updatePixelWithError(uint32_t x, uint32_t y, float err)
{
    if (x < cfg->width && y < cfg->height)
    {
        float value = round(getPixel(x, y).r + err);
        if (value > 255)
        {
            setPixel(x, y, RGB(255));
        }
        else if (value < 0)
        {
            setPixel(x, y, RGB(0));
        }
        else
        {
            setPixel(x, y, RGB(value));
        }
    }
}

void ImageTransform::errorDistribution()
{
    grayscale();
    
    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            int error;
            uint8_t value = getPixel(x, y).r ;
            if (value > cfg->threshold)
            {
                setPixel(x, y, COLOR_WHITE);
                error = value - 255;
            }
            else
            {
                setPixel(x, y, COLOR_BLACK);
                error = value;
            }
            updatePixelWithError(x + 1, y, error * 3.0 / 8);
            updatePixelWithError(x, y + 1, error * 3.0 / 8);
            updatePixelWithError(x + 1, y + 1, error * 2.0 / 8);
        }
    }
}

