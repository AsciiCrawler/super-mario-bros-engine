#include "src/spriteSheet/spriteSheetMario.hpp"
#include "iostream"

SpriteSheetMario::SpriteSheetMario(const std::string file, const float spriteWidth, const float spriteHeight, const float quadWidth, const float quadHeight) : SpriteSheet(file, spriteWidth, spriteHeight, quadWidth, quadHeight)
{
    auto getSpriteIndex = [](int index)
    { return index * 6; };

    this->spriteIndexes.insert(std::make_pair<std::string, int>("running-left-giant-0", getSpriteIndex(0)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("running-left-giant-1", getSpriteIndex(1)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("running-left-giant-2", getSpriteIndex(2)));

    this->spriteIndexes.insert(std::make_pair<std::string, int>("running-left-small-0", getSpriteIndex(3)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("running-left-small-1", getSpriteIndex(4)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("running-left-small-2", getSpriteIndex(5)));

    /*  */

    this->spriteIndexes.insert(std::make_pair<std::string, int>("running-right-giant-0", getSpriteIndex(8)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("running-right-giant-1", getSpriteIndex(9)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("running-right-giant-2", getSpriteIndex(10)));

    this->spriteIndexes.insert(std::make_pair<std::string, int>("running-right-small-0", getSpriteIndex(11)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("running-right-small-1", getSpriteIndex(12)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("running-right-small-2", getSpriteIndex(13)));

    /*  */

    this->spriteIndexes.insert(std::make_pair<std::string, int>("idle-right-giant", getSpriteIndex(16)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("crunch-right-giant", getSpriteIndex(17)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("jump-right-giant", getSpriteIndex(18)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("dash-right-giant", getSpriteIndex(19)));

    this->spriteIndexes.insert(std::make_pair<std::string, int>("idle-left-giant", getSpriteIndex(20)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("crunch-left-giant", getSpriteIndex(21)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("jump-left-giant", getSpriteIndex(22)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("dash-left-giant", getSpriteIndex(23)));

    /*  */

    this->spriteIndexes.insert(std::make_pair<std::string, int>("idle-right-small", getSpriteIndex(24)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("jump-right-small", getSpriteIndex(25)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("dash-right-small", getSpriteIndex(26)));

    this->spriteIndexes.insert(std::make_pair<std::string, int>("idle-left-small", getSpriteIndex(27)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("jump-left-small", getSpriteIndex(28)));
    this->spriteIndexes.insert(std::make_pair<std::string, int>("dash-left-small", getSpriteIndex(29)));

    this->spriteIndexes.insert(std::make_pair<std::string, int>("game-over", getSpriteIndex(30)));
}