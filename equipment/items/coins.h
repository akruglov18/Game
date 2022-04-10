#pragma once
#include "items.h"

class Coins : public Items {
public:
    Coins(int value, const sf::Vector2f& position);
    Coins(const Coins&) = default;
    Coins& operator=(const Coins& other) = default;
    ~Coins() = default;

    int getValue() const { return value; }
    void changeValue(int added);
    virtual json to_json() const override { return json(); }
private:
    int value;
    void chooseTexture();
};