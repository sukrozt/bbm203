#include "Isle.h"

Isle::Isle(std::string name) : name(name)
{
}

const std::string& Isle::getName() const
{
    return this->name;
}
Item Isle::getItem()
{
    return item;
}

void Isle::setItem(Item item)
{
    this->item = item;
}

int Isle::getShaperCount()
{
    return shaperCount;
}

bool Isle::increaseShaperCount()
{
    bool isFull = false;

    if(shaperCount >= capacity)
        isFull = true;

    return isFull;
}

bool Isle::decreaseShaperCount()
{
    bool isEmpty = true;

    if(shaperCount < capacity && 0 < shaperCount)
        isEmpty = true;

    return isEmpty;
}

bool Isle::operator==(const Isle &other) const
{
    if(other.name == name)
        return true;
    return false;
}

bool Isle::operator<(const Isle &other) const
{
    if(other.name < name)
        return true;
    return false;
}

bool Isle::operator>(const Isle &other) const
{
    if(other.name > name)
        return true;
    return false;
}

// Implementation of readFromFile
std::vector<Isle *> Isle::readFromFile(const std::string &filename)
{
    std::vector<Isle *> isles;
    std::ifstream file(filename);

    if (!file)
    {
        std::cerr << "[Error] Unable to open file: " << filename << std::endl;
        return isles; // Return an empty vector in case of an error
    }

    std::string isleName;
    while (std::getline(file, isleName))
    {
        if (!isleName.empty()) // Avoid adding empty lines as Isles
        {
            Isle *newIsle = new Isle(isleName); // Assuming Isle has a constructor that accepts a name
            isles.push_back(newIsle);
        }
    }

    file.close();
    return isles;
}

std::ostream &operator<<(std::ostream &os, const Isle &p)
{
    // Prints to terminal with color
    // This function might cause some problems in terminals that are not Linux based
    // If so, comment out the colors while testing on local machine
    // But open them back up while submitting or using TurBo

    std::string einsteiniumColor = "\033[38;2;6;134;151m";
    std::string goldiumColor = "\033[38;2;255;198;5m";
    std::string amazoniteColor = "\033[38;2;169;254;255m";
    std::string resetColorTag = "\033[0m";

    if (p.item == EINSTEINIUM)
        return (os << einsteiniumColor << p.name << resetColorTag);
    if (p.item == GOLDIUM)
        return (os << goldiumColor << p.name << resetColorTag);
    if (p.item == AMAZONITE)
        return (os << amazoniteColor << p.name << resetColorTag);
    return (os << p.name);
}