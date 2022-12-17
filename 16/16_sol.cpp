#include <string>
#include <memory>
#include <vector>
#include <map>
#include <set>

#include "../utility.h"


constexpr std::uint64_t MINUTES{ 20u };

struct Valve
{
    std::string id;
    std::uint64_t flow_rate;
    std::vector<std::string> successors;
    bool is_open{ false };
};

std::map<std::string, Valve> get_valves(const std::string &file_path);
std::uint64_t get_max_pressure_release(std::map<std::string, Valve> &v_map, std::uint64_t time_left);
std::uint64_t get_pressure_rec(std::map<std::string, Valve> &v_map, std::string cur_v, std::uint64_t time_left, std::uint64_t press_rate, std::uint64_t cur_press_vol);

std::uint64_t day_16_1(const std::string &file_path)
{
    std::map<std::string, Valve> v_map = get_valves(file_path);

    return get_max_pressure_release(v_map, MINUTES);
}

std::uint64_t day_16_2(const std::string &file_path)
{

    return 0u;
}

std::uint64_t get_pressure_rec(std::map<std::string, Valve> &v_map, std::string cur_v, std::uint64_t time_left, std::uint64_t press_rate, std::uint64_t cur_press_vol)
{
    if (time_left == 0u)
    {
        return cur_press_vol;
    }
    else
    {
        cur_press_vol += press_rate;
        std::uint64_t max_press{ 0u };
        for (auto &elem : v_map.at(cur_v).successors)
        {
            std::uint64_t tmp{ 0u };
            if (!v_map.at(elem).is_open && time_left > 2u)
            {
                v_map.at(elem).is_open = true;
                std::uint64_t new_rate = press_rate + v_map.at(elem).flow_rate;
                tmp = get_pressure_rec(v_map, elem, time_left-2, new_rate, cur_press_vol + press_rate);
                max_press = std::max(max_press, tmp);
            }
            tmp = get_pressure_rec(v_map, elem, time_left-1, press_rate, cur_press_vol);
            max_press = std::max(max_press, tmp);
        }
        return max_press;
    }
}

std::uint64_t get_max_pressure_release(std::map<std::string, Valve> &v_map, std::uint64_t time_left)
{
    return get_pressure_rec(v_map, "AA", MINUTES, 0, 0);
}

std::map<std::string, Valve> get_valves(const std::string &file_path)
{
    std::map<std::string, Valve> valve_map;
    std::vector<std::string> input_data = read_string_vec_from_file(file_path);

    for (const auto &line : input_data)
    {
        std::uint64_t val = parse_string_to_number<std::uint64_t>(line);
        std::string name = line.substr(6,2);
        std::vector<std::string> words = split_string(line, ", ");
        std::vector<std::string> valves { words.front().substr(words.front().length()-2) };
        for (size_t i=1; i<words.size(); ++i)
        {
            valves.push_back(words.at(i));
        }
        valve_map[name] = Valve{ name, val, valves, false };
    }

    return valve_map;
}