#include <cstdint>
#include <vector>
#include <iostream>

#include "../utility.h"

constexpr std::uint8_t LEFT_WALL{ 0u };
constexpr std::uint8_t RIGHT_WALL{ 8u };

using THeight = std::uint64_t;
using RockPts = std::vector<Point<THeight>>;
using Chamber = std::vector<std::vector<char>>;

enum class JetDir : std::uint8_t
{
    Left,
    Right,
    Down
};

void move_rock(const JetDir dir, RockPts &rock);
bool does_overlap(const RockPts &rock, const Chamber &chamber);
THeight set_stone(const RockPts &rock, Chamber &chamber, THeight max_height);
void extend_chamber(const THeight new_height, const std::vector<char> level, Chamber &chamber);

// Used to store whenever a new level consisting of 7 stones ('#######')
// through this we can detect any cycles in the stone falls 
struct Snapshot
{
    uint64_t rock_idx;
    uint64_t jet_idx;
    uint64_t rock_ctr;
    uint64_t jet_ctr;
    THeight height;
    std::vector<std::vector<char>> n_highest_levels;
};
bool operator<(const Snapshot &s1, const Snapshot &s2)
{
    return s1.rock_idx < s2.rock_idx ? true : s1.jet_idx < s2.jet_idx;
}
bool operator==(const Snapshot &s1, const Snapshot &s2) // used to compare if two snapshots coincide
{
    return s1.rock_idx == s2.rock_idx && s1.jet_idx == s2.jet_idx && s1.n_highest_levels == s2.n_highest_levels;
}

class StoneForm
{
private:
    RockPts rock_pts; ///< coordinates of this form

public:
    StoneForm(const RockPts form_layout);
    void do_gas_jet(const char dir, const Chamber &chamber);
    bool fall_down(const Chamber &chamber);
    RockPts get_rock_pts() { return rock_pts; }
};

StoneForm::StoneForm(const RockPts form_layout) : rock_pts{ form_layout } { };

void StoneForm::do_gas_jet(const char dir, const Chamber &chamber)
{
    RockPts new_rock_pts{ rock_pts };
    switch (dir)
    {
    case '<':
        move_rock(JetDir::Left, new_rock_pts);
        if (!does_overlap(new_rock_pts, chamber)) 
        {
            rock_pts = new_rock_pts;
            return;
        }
        break;
    case '>':
        move_rock(JetDir::Right, new_rock_pts);
        if (!does_overlap(new_rock_pts, chamber)) 
        {
            rock_pts = new_rock_pts;
            return;
        }
        break;
    default:
        throw std::runtime_error("Unknown jet sign detected: " + dir);
        break;
    }
}

bool StoneForm::fall_down(const Chamber &chamber)
{
    RockPts new_rock_pts{ rock_pts };
    move_rock(JetDir::Down, new_rock_pts);
    if (!does_overlap(new_rock_pts, chamber)) 
    {
        rock_pts = new_rock_pts;
        return true;
    }
    return false;
}

void extend_chamber(const THeight new_height, const std::vector<char> level, Chamber &chamber)
{
    while (chamber.size() < new_height)
    {
        chamber.push_back(level);
    }
}

THeight set_stone(const RockPts &rock, Chamber &chamber, THeight max_height)
{
    for (const auto &pt : rock)
    {
        chamber.at(pt.y).at(pt.x) = '#';
        if (pt.y > max_height)
        {
            max_height = pt.y;
        }
    }
    return max_height;
}

bool does_overlap(const RockPts &rock, const Chamber &chamber)
{
    for (const auto &pt : rock)
    {
        if (chamber.at(pt.y).at(pt.x) == '#')
        {
            return true;
        }
    }
    return false;
}

void move_rock(const JetDir dir, RockPts &rock)
{
    switch (dir)
    {
    case JetDir::Left:
        for (auto &pt : rock)
        {
            --pt.x;
        }
        break;
    case JetDir::Right:
        for (auto &pt : rock)
        {
            ++pt.x;
        }
        break;
    case JetDir::Down:
        for (auto &pt : rock)
        {
            --pt.y;
        }
        break;
    
    default:
        break;
    }
}