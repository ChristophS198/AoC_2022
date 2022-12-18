#include <string>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include <limits>
#include <algorithm>
#include <chrono>

#include "../utility.h"

// A fast solution would be to reduce the overall map to a smaller one only conatining the start valve
// + any valves with a flow_rate > 0. In order to incorporate the valves with no flow rate and the tunnels
// we need to increase the distance between all remaining valves
// Afterwards we get a smaller graph and could just calculate any combinations of opening the valves in all
// different orders


constexpr std::int32_t MINUTES{ 30 }; 
constexpr std::int32_t EL_TRAIN_TIME { 4 };

using VDist = std::uint16_t;
using VId = std::uint16_t;
using VTime = std::int32_t;

struct Valve
{
    std::string id;
    bool is_open{ false };
    std::uint64_t flow_rate;
    std::vector<VId> successors;
};

struct RedValve // Reduced Valve
{
    bool is_open{ false };
    std::uint64_t flow_rate;
    std::vector<std::pair<VId, VDist>> successors;
};

std::map<VId, Valve> get_valves(const std::string &file_path);
std::map<VId, RedValve> get_reduced_dist_mat(const std::map<VId, Valve> &valve_map, const std::set<VId> &int_valves, VId start);
std::vector<VDist> do_bfs(const std::map<VId, Valve> &valve_map, const std::set<VId> &targets, const VId start);
std::set<VId> get_interesting_valves(const std::map<VId, Valve> &valve_map);
std::uint64_t get_max_pressure_release(std::map<VId, RedValve> &v_map, VTime time_left);
std::uint64_t get_max_pressure_release_2(std::map<VId, RedValve> &v_map, VTime time_left);
std::uint64_t get_pressure_rec(std::map<VId, RedValve> &v_map, const std::uint16_t cur_v, const VTime time_left, const std::uint64_t press_rate, const std::uint64_t cur_press_vol);
std::uint64_t get_pressure_rec_2(std::map<VId, RedValve> &v_map, const std::uint16_t cur_v, const std::pair<VId, VDist> travel_v, const VTime time_left, const std::uint64_t press_rate, const std::uint64_t cur_press_vol);
void remove_valve_from_succ_lists(std::map<VId, RedValve> &v_map, VId valve);
std::uint64_t comb_approach(std::map<VId, RedValve> &v_map, VTime time_left);

std::uint64_t day_16_1(const std::string &file_path)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::map<std::uint16_t, Valve> v_map = get_valves(file_path);
    std::set<std::uint16_t> int_valves = get_interesting_valves(v_map);
    std::map<VId, RedValve> dist_map = get_reduced_dist_mat(v_map, int_valves, 0u);
    std::uint64_t max_press = get_max_pressure_release(dist_map, MINUTES);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";
    return max_press;
}

std::uint64_t day_16_2(const std::string &file_path)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::map<std::uint16_t, Valve> v_map = get_valves(file_path);
    std::set<std::uint16_t> int_valves = get_interesting_valves(v_map);
    std::map<VId, RedValve> dist_map = get_reduced_dist_mat(v_map, int_valves, 0u);
    std::uint64_t max_press = comb_approach(dist_map, MINUTES - EL_TRAIN_TIME);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";
    return max_press;
}


std::uint64_t get_pressure_rec_2(std::map<VId, RedValve> &v_map, const std::uint16_t cur_v, const std::pair<VId, VDist> travel_v, const VTime time_left, const std::uint64_t press_rate, const std::uint64_t cur_press_vol)
{
    if (time_left < 3) // we need at least three time slots to go, open another valve and wait for its first pressure release
    {
        auto v2 = time_left - static_cast<VTime>(travel_v.second) > 0 ? time_left - static_cast<VTime>(travel_v.second) : 0;
        auto tmp = cur_press_vol + (time_left * press_rate) + v_map.at(travel_v.first).flow_rate * v2;
        return tmp;
    }
    if (time_left < 0) throw std::runtime_error("Negative time should not be possible!");
    else
    {
        auto succ_pairs_short = v_map.at(cur_v).successors;
        v_map.erase(cur_v);
        std::uint64_t max_press{ 0u };
        if (succ_pairs_short.empty())
        {
            auto v2 = time_left - static_cast<VTime>(travel_v.second) > 0 ? time_left - static_cast<VTime>(travel_v.second) : 0;
            return cur_press_vol + (time_left * press_rate) + v_map.at(travel_v.first).flow_rate * v2;
        }

        for (auto &elem : succ_pairs_short)
        {
            std::uint64_t tmp{ 0u };
            
            if (time_left < static_cast<VTime>(elem.second) + 1) 
            { // we need at least the time to travel + open this valve to seee a change in the overall release
                auto v2 = time_left - static_cast<VTime>(travel_v.second) > 0 ? time_left - static_cast<VTime>(travel_v.second) : 0;
                tmp = cur_press_vol + (time_left * press_rate) + v_map.at(travel_v.first).flow_rate * v2;
            }
            else
            {
                auto &near_v = elem.second < travel_v.second ? elem : travel_v; // check which valve is nearer
                auto &dist_v = elem.second < travel_v.second ? travel_v : elem;

                VTime dt = near_v.second; 

                std::uint64_t new_press_vol = cur_press_vol + dt * press_rate; // Increase released pressure value by time needed to travel to nearer valve
                std::uint64_t new_press_rate = press_rate + v_map[near_v.first].flow_rate;
                std::map<VId, RedValve> new_map{ v_map };
                remove_valve_from_succ_lists(new_map, elem.first); // We do not want to re-visit this valve, since it is already open 
                std::pair<VId, VDist> second_v{ dist_v.first, dist_v.second - dt };

                tmp = get_pressure_rec_2(new_map, near_v.first, second_v, time_left - dt, new_press_rate, new_press_vol);
                
                // Was not neccessary, but I'm not sure if the order of starting new recursion might
                // impact the result (for the case of both reaching a valve at the same time)
                // if (near_v.second == dist_v.second)
                // {
                //     second_v.first = near_v.first;
                //     auto tmp2 = get_pressure_rec_2(new_map, dist_v.first, second_v, time_left - dt, new_press_rate, new_press_vol);
                //     tmp = std::max(tmp, tmp2);
                // }
            }

            max_press = std::max(tmp, max_press);
        }
        return max_press;
    }
}

std::uint64_t comb_approach(std::map<VId, RedValve> &v_map, VTime time_left)
{
    VId start_id{ 0u };
    std::vector<std::pair<VId, VDist>> valves;
    for (const auto &elem : v_map.at(start_id).successors)
    {
        valves.push_back(elem);
    }
    std::vector<std::pair<VId, VDist>> closed_valves_second{ valves };
    std::uint64_t max_press{ 0u };
    v_map.erase(start_id);
    
    for (size_t i=0; i<valves.size(); ++i)
    {
        auto first_v = valves[i];
        for (size_t j=i+1; j<valves.size(); ++j)
        {
            auto second_v = valves[j];
            auto &near_v = first_v.second < second_v.second ? first_v : second_v;
            auto &dist_v = first_v.second < second_v.second ? second_v : first_v;
            std::uint64_t press_vol = 0;
            std::uint64_t press_rate = v_map[near_v.first].flow_rate;
            VTime dt = near_v.second; 

            std::map<VId, RedValve> new_map{ v_map };
            remove_valve_from_succ_lists(new_map, near_v.first); // We do not want to re-visit this valve, since it is already open 
            remove_valve_from_succ_lists(new_map, dist_v.first); // We do not want to re-visit this valve, since it is already open 
            std::pair<VId, VDist> second_copy{ dist_v.first, dist_v.second - dt }; // reduce distance to distant valve

            std::uint64_t tmp = get_pressure_rec_2(new_map, near_v.first, second_copy, time_left - dt, press_rate, press_vol);
            
            max_press = std::max(tmp, max_press);
        }
    }
    return max_press;
}


std::uint64_t get_pressure_rec(std::map<VId, RedValve> &v_map, const std::uint16_t cur_v, const VTime time_left, const std::uint64_t press_rate, const std::uint64_t cur_press_vol)
{
    if (time_left < 3) // we need at least three time slots to go, open another valve and wait for its first pressure release
    {
        return cur_press_vol + (time_left * press_rate);
    }
    if (time_left < 0) throw std::runtime_error("Negative time should not be possible!");
    else
    {
        auto succ_pairs = v_map.at(cur_v).successors;
        v_map.erase(cur_v);
        std::uint64_t max_press{ 0u };
        if (succ_pairs.empty())
        {
            return cur_press_vol + time_left * press_rate;
        }
        for (auto &elem : succ_pairs)
        {
            auto &succ_valve = v_map.at(elem.first);
            std::uint64_t tmp{ 0u };
            
            if (time_left < static_cast<VTime>(elem.second)+1) 
            { // we need at least the time to travel + open this valve to seee a change in the overall release
                tmp = cur_press_vol + time_left * press_rate;
            }
            else
            {
                std::uint64_t new_press_vol = cur_press_vol + press_rate * elem.second; // Increase released pressure value by time needed to travel to this valve
                std::uint64_t new_rate = press_rate + succ_valve.flow_rate;
                std::map<VId, RedValve> new_map{ v_map };
                remove_valve_from_succ_lists(new_map, elem.first); // We do not want to re-visit this valve, since it is already open 
                tmp = get_pressure_rec(new_map, elem.first, time_left-elem.second, new_rate, new_press_vol);
            }

            max_press = std::max(tmp, max_press);
        }
        return max_press;
    }
}

std::uint64_t get_max_pressure_release(std::map<VId, RedValve> &v_map, VTime time_left)
{
    std::uint64_t max_press{ 0u };
    VId start_id{ 0u };
    auto succ_pairs = v_map.at(start_id).successors;
    v_map.erase(0); // Afterwards the information about the start node are not needed anymore

    for (auto &succ : succ_pairs)
    {
        std::uint64_t tmp{ 0u };
        auto &succ_valve = v_map.at(succ.first);
        if (time_left < static_cast<VTime>(succ.second)+2) // we need at least the time to travel + open this valve to seee a change in the overall release
        {
            continue;
        }
        std::uint64_t new_rate = succ_valve.flow_rate;
        std::map<VId, RedValve> new_map{ v_map };
        remove_valve_from_succ_lists(new_map, succ.first); // We do not want to re-visit this valve, since it is already open 
        tmp = get_pressure_rec(new_map, succ.first, time_left-static_cast<VTime>(succ.second), new_rate, 0u);
        
        max_press = std::max(max_press,tmp);
    }
    return max_press;
}

void remove_valve_from_succ_lists(std::map<VId, RedValve> &v_map, VId valve)
{
    auto find_end = [&valve](const std::pair<VId, VDist> &succ)
    {
        return valve == succ.first;
    };
    
    for (auto &entry : v_map)
    {
        auto it = std::find_if(entry.second.successors.begin(), entry.second.successors.end(), find_end);
        if (it != entry.second.successors.end())
        {
            entry.second.successors.erase(it);
        }
    }
}

std::map<VId, RedValve> get_reduced_dist_mat(const std::map<VId, Valve> &valve_map, const std::set<VId> &int_valves, VId start)
{
    std::map<VId, RedValve> dist_map;
    for (auto cur_v_id : int_valves)
    {
        std::vector<VDist> shortest_d = do_bfs(valve_map, int_valves, cur_v_id);
        RedValve new_v{ false, valve_map.at(cur_v_id).flow_rate, {} };
        for (const auto &v : int_valves)
        {
            if (shortest_d.at(v) == 0) continue;
            new_v.successors.push_back({ v,shortest_d.at(v) + 1}); // +1 so opening is already included in distance
        }
        dist_map[cur_v_id] = new_v;
    }

    // Add starting valve
    std::vector<VDist> shortest_d = do_bfs(valve_map, int_valves, 0);
    RedValve new_v{ false, valve_map.at(0).flow_rate, {} };
    for (const auto &v : int_valves)
    {
        new_v.successors.push_back({ v,shortest_d.at(v) + 1}); // +1 so opening is already included in distance
    }
    dist_map[0] = new_v;

    return dist_map;
}

// returns shortest distances from valve 'start' to all valves in targets
std::vector<VDist> do_bfs(const std::map<VId, Valve> &valve_map, const std::set<VId> &targets, const VId start)
{
    std::vector<VDist> shortest_d(valve_map.size(), std::numeric_limits<VDist>::max());
    size_t num_dists_found{ 0u };
    if (std::find(targets.begin(), targets.end(), start) != targets.end())
    {
        shortest_d.at(start) = 0u;
        ++num_dists_found;
    }
    std::set<VId> newly_reached_valves{ start }; // is equivalent to valves for which the shortest distance is already known
    VDist step{ 0u };
    while (num_dists_found < targets.size() || newly_reached_valves.size() > 0)
    {
        ++step;
        std::set<VId> tmp;
        for (const auto &valve : newly_reached_valves)
        {
            const std::vector<VId> &succ = valve_map.at(valve).successors;
            for (const auto &succ_id : succ)
            {
                if (shortest_d.at(succ_id) == std::numeric_limits<VDist>::max())
                {
                    shortest_d.at(succ_id) = step;
                    tmp.insert(succ_id);
                    if (targets.find(succ_id) != targets.end())
                    {
                        ++num_dists_found;
                    }
                }
            }
        }
        newly_reached_valves = tmp;
    }
    return shortest_d;
}

// returns a list of vlaves that have a flowrate > 0 and are still closed
std::set<VId> get_interesting_valves(const std::map<VId, Valve> &valve_map)
{
    std::set<VId> valves_with_flow_rate;
    for (const auto &valve : valve_map)
    {
        if (valve.second.flow_rate > 0 && !valve.second.is_open)
        {
            valves_with_flow_rate.insert(valve.first);
        }
    }
    return valves_with_flow_rate;
}

std::map<VId, Valve> get_valves(const std::string &file_path)
{
    std::map<VId, Valve> valve_map;
    std::map<std::string, VId> id_map;
    std::vector<std::string> input_data = read_string_vec_from_file(file_path);
    id_map["AA"] = 0u; // start always at node 0
    VId nxt_id{ 1u };
    for (const auto &line : input_data)
    {
        std::uint64_t flow_rate = parse_string_to_number<std::uint64_t>(line);
        std::string name = line.substr(6,2);
        if (id_map.find(name) == id_map.end())
        {
            id_map[name] = nxt_id++;
        }
        std::vector<std::string> words = split_string(line, ", ");
        std::string first_succ = words.front().substr(words.front().length()-2);
        if (id_map.find(first_succ) == id_map.end())
        {
            id_map[first_succ] = nxt_id++;
        }
        std::vector<VId> valves { id_map[first_succ] };
        for (size_t i=1; i<words.size(); ++i)
        {            
            if (id_map.find(words.at(i)) == id_map.end())
            {
                id_map[words.at(i)] = nxt_id++;
            }
            valves.push_back(id_map[words.at(i)]);
        }
        valve_map[id_map.at(name)] = Valve{ name, false, flow_rate, valves };
    }

    return valve_map;
}