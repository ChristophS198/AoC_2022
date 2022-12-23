#include <string>
#include <map>
#include <array>
#include <algorithm>

#include "../utility.h"

using RNum = uint8_t;
using MNum = uint8_t;

constexpr uint8_t ROBOT_TYPES{ 4u };
constexpr uint8_t RESSOURCE_TYPES{ 4u };
constexpr int16_t TIME_LIMIT{ 24 };
constexpr int16_t TIME_LIMIT_2{ 32 };

enum class Robot : uint8_t
{
    Ore,
    Clay, 
    Obsid,
    Geode,
    Count
};

enum class Res : uint8_t
{
    ResOre,
    ResClay, 
    ResObsid,
    ResGeode,
    ResCount
};

struct State
{
    std::array<uint8_t,ROBOT_TYPES> robots; 
    std::array<uint8_t,RESSOURCE_TYPES> ress;    
};

struct Blueprint
{
    uint8_t id;
    uint8_t ore_robot;
    uint8_t clay_robot;
    std::pair<uint8_t,uint8_t> obsid_robot;
    std::pair<uint8_t,uint8_t> geode_robot;
};

struct QualityLevel
{
    uint8_t id;
    uint16_t geode_count;
};

std::vector<Blueprint> get_blueprints(const std::string &file_path, int num=-1);
QualityLevel get_quality_level(const Blueprint blueprint, int16_t time);
uint16_t build_robot_rec(State state, const Blueprint blueprint, int16_t time, const Robot r_type);
void build_robot(State &state, const Blueprint blueprint, const Robot r_type);
bool is_buildable(const State &state, const Blueprint blueprint, const Robot r_type);

uint32_t day_19_1(const std::string &file_path)
{
    std::vector<Blueprint> blueprints = get_blueprints(file_path);
    uint32_t sum_qual_level{ 0u }; 

    for (const auto &blueprint : blueprints)
    {
        QualityLevel qual_lev = get_quality_level(blueprint, TIME_LIMIT);
        sum_qual_level += qual_lev.id * qual_lev.geode_count;
    }

    return sum_qual_level;
}

uint32_t day_19_2(const std::string &file_path)
{
    std::vector<Blueprint> blueprints = get_blueprints(file_path,3);
    uint32_t geod_mult{ 1u }; 

    for (const auto &blueprint : blueprints)
    {
        QualityLevel qual_lev = get_quality_level(blueprint, TIME_LIMIT_2);
        geod_mult *= qual_lev.geode_count;
    }

    return geod_mult;
}

uint16_t build_robot_rec(State state, const Blueprint blueprint, int16_t time, const Robot r_type)
{
    --time;
    std::array<bool,static_cast<size_t>(Robot::Count)> r_buildable{ 1,1,0,0 }; // is set to true for all robot types for which we can generate the necessary ressources
    std::array<bool,static_cast<size_t>(Robot::Count)> r_built{ 0,0,0,0 };

    // increase ressources
    for (size_t i=0; i<state.robots.size(); ++i)
    {
        state.ress[i] += state.robots[i];
    }
    build_robot(state, blueprint, r_type);

    // check if we can return due to time limit
    if (time < 2)
    {
        return state.ress[static_cast<size_t>(Res::ResGeode)] + time * state.robots[static_cast<size_t>(Robot::Geode)];
    }

    // check if it makes sense to wait for more ressources in order to build obsidian and Geode robot 
    r_buildable[static_cast<size_t>(Robot::Obsid)] = state.ress[static_cast<size_t>(Res::ResClay)] > 0;
    r_buildable[static_cast<size_t>(Robot::Geode)] = state.ress[static_cast<size_t>(Res::ResObsid)] > 0;
    // if we currently already get more ore per time unit than we need for the most expensive robot, it makes no sense to build another ore robot
    uint8_t max_ore_need = std::max(blueprint.geode_robot.first, std::max(blueprint.clay_robot, blueprint.obsid_robot.first));
    if (state.robots[static_cast<size_t>(Robot::Ore)] >= max_ore_need)
    {
        r_buildable[static_cast<size_t>(Robot::Ore)] = false;
    }

    std::array<std::uint16_t,static_cast<size_t>(Robot::Count)> max_geodes{ 0u };
    while (time > 0 && (r_built != r_buildable))
    {
        // check for each robot type if we can build it or it makes sense to build it
        // if we can build it -> we do
        for (size_t i=0; i<static_cast<size_t>(Robot::Count); ++i)
        {
            if (r_buildable[i] && !r_built[i] && is_buildable(state, blueprint, static_cast<Robot>(i)))
            {
                r_built[i] = true;
                max_geodes[i] = build_robot_rec(state, blueprint, time, static_cast<Robot>(i));
            }        
        }
        --time;
        // increase ressources
        for (size_t i=0; i<state.robots.size(); ++i)
        {
            state.ress[i] += state.robots[i];
        }
        if (time < 2)
        {
            uint16_t tmp = state.ress[static_cast<size_t>(Res::ResGeode)] + time * state.robots[static_cast<size_t>(Robot::Geode)];
            return std::max(tmp, *std::max_element(max_geodes.begin(), max_geodes.end()));
        }
    }

    return *std::max_element(max_geodes.begin(), max_geodes.end());
}

QualityLevel get_quality_level(const Blueprint blueprint, int16_t time)
{
    QualityLevel qual_level{ blueprint.id, 0u };
    State state{};
    ++state.robots[static_cast<size_t>(Robot::Ore)];
    std::uint16_t clay_max{ 0u };
    std::uint16_t ore_max{ 0u };

    while (state.ress[static_cast<size_t>(Res::ResOre)] < std::max(blueprint.clay_robot, blueprint.ore_robot))
    {
        --time;
        ++state.ress[static_cast<size_t>(Res::ResOre)];
        if (state.ress[static_cast<size_t>(Res::ResOre)] == blueprint.clay_robot)
        {
            clay_max = build_robot_rec(state, blueprint, time, Robot::Clay);
        }
        if (state.ress[static_cast<size_t>(Res::ResOre)] == blueprint.ore_robot)
        {
            ore_max = build_robot_rec(state, blueprint, time, Robot::Ore);
        }
    }
    qual_level.geode_count = std::max(clay_max, ore_max);

    return qual_level;
}

bool is_buildable(const State &state, const Blueprint blueprint, const Robot r_type)
{
    switch (r_type)
    {
    case Robot::Ore:
        if( state.ress.at(static_cast<size_t>(Res::ResOre)) >= blueprint.ore_robot) return true;
        break;
    case Robot::Clay:
        if( state.ress.at(static_cast<size_t>(Res::ResOre)) >= blueprint.clay_robot) return true;
        break;
    case Robot::Obsid:
        if( state.ress.at(static_cast<size_t>(Res::ResOre)) >= blueprint.obsid_robot.first &&
            state.ress.at(static_cast<size_t>(Res::ResClay)) >= blueprint.obsid_robot.second) 
            {
                return true;
            }
        break;
    case Robot::Geode:
        if( state.ress.at(static_cast<size_t>(Res::ResOre)) >= blueprint.geode_robot.first &&
            state.ress.at(static_cast<size_t>(Res::ResObsid)) >= blueprint.geode_robot.second) 
            {
                return true;
            }
        break;
    default:
        throw std::runtime_error("Unknown robot type detected!");
        break;
    }

    return false;
}

void build_robot(State &state, const Blueprint blueprint, const Robot r_type)
{
    switch (r_type)
    {
    case Robot::Ore:
        state.ress[static_cast<size_t>(Res::ResOre)] -= blueprint.ore_robot;
        break;
    case Robot::Clay:
        state.ress[static_cast<size_t>(Res::ResOre)] -= blueprint.clay_robot;
        break;
    case Robot::Obsid:
        state.ress[static_cast<size_t>(Res::ResOre)] -= blueprint.obsid_robot.first;
        state.ress[static_cast<size_t>(Res::ResClay)] -= blueprint.obsid_robot.second;
        break;
    case Robot::Geode:
        state.ress[static_cast<size_t>(Res::ResOre)] -= blueprint.geode_robot.first;
        state.ress[static_cast<size_t>(Res::ResObsid)] -= blueprint.geode_robot.second;
        break;
    
    default:
        throw std::runtime_error("Unknown robot type detected!");
        break;
    }

    ++state.robots[static_cast<size_t>(r_type)];
}

std::vector<Blueprint> get_blueprints(const std::string &file_path, const int num)
{
    std::vector<Blueprint> blueprints;
    std::vector<std::string> input_data = read_string_vec_from_file(file_path);
    for (const auto &line : input_data)
    {
        std::vector<uint8_t> data = parse_string_to_number_vec<uint8_t>(line);
        Blueprint new_print{ data[0], data[1], data[2], { data[3],data[4] }, { data[5],data[6] } };
        blueprints.push_back(new_print);
        if (num > 0 && blueprints.size() == static_cast<size_t>(num))
        {
            break;
        }
    }

    return blueprints;
}
