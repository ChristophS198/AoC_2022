#include <string>
#include <vector>
#include <ostream>
#include <algorithm>
#include <limits>
#include <map>
#include <set>

#include "../utility.h"

using LDrop = Point3D<int32_t>;
using LCoord = LDrop::type;
using MinMaxMap = std::map<std::pair<LCoord,LCoord>,std::pair<LCoord,LCoord>>;

std::vector<LDrop> get_cubes(const std::string &file_path, MinMaxMap &min_max_x, MinMaxMap &min_max_y, MinMaxMap &min_max_z);
uint32_t get_num_connected_sides(const std::vector<LDrop> &cubes);
std::set<LDrop> get_air_candidates(std::vector<LDrop> &cubes, const MinMaxMap &min_max_x, const MinMaxMap &min_max_y, const MinMaxMap &min_max_z);
uint32_t get_num_air_cubes(const std::vector<LDrop> &cubes, std::set<LDrop> &cand, const MinMaxMap &min_max_x, const MinMaxMap &min_max_y, const MinMaxMap &min_max_z);
bool is_outside_range(const LDrop &drop, const MinMaxMap &min_max_x, const MinMaxMap &min_max_y, const MinMaxMap &min_max_z);


int32_t day_18_1(const std::string &file_path)
{
    MinMaxMap min_max_x;
    MinMaxMap min_max_y;
    MinMaxMap min_max_z;
    std::vector<LDrop> cubes = get_cubes(file_path, min_max_x, min_max_y, min_max_z);
    uint32_t total_sides = cubes.size() * 6;
    uint32_t inner_sides = get_num_connected_sides(cubes);

    return total_sides - inner_sides;
}

int32_t day_18_2(const std::string &file_path)
{
    MinMaxMap min_max_x;
    MinMaxMap min_max_y;
    MinMaxMap min_max_z;
    std::vector<LDrop> cubes = get_cubes(file_path, min_max_x, min_max_y, min_max_z);
    uint32_t total_sides = cubes.size() * 6;
    uint32_t inner_sides = get_num_connected_sides(cubes);
    std::set<LDrop> candidates = get_air_candidates(cubes, min_max_x, min_max_y, min_max_z);
    uint32_t air_cube_sides = get_num_air_cubes(cubes, candidates, min_max_x, min_max_y, min_max_z);

    return total_sides - inner_sides - air_cube_sides;
}

// The candidate set might still contain some air cubes that are not entirely enclosed in lava
// In order to find those, we start with any air cube and iteratively find all surrounding air cubes
// forming a larger air bubble. If a connecting cube is outside of the candidates and is no lava
// the corresponding subset of air cubes is connected to the outside and not fully enclosed
uint32_t get_num_air_cubes(const std::vector<LDrop> &cubes, std::set<LDrop> &cand, const MinMaxMap &min_max_x, const MinMaxMap &min_max_y, const MinMaxMap &min_max_z)
{
    uint32_t num_inner_side{ 0u };
    while (!cand.empty())
    {
        LDrop pt = *cand.begin();
        bool is_inner{ true };
        std::vector<LDrop> new_pts{ pt };
        std::set<LDrop> cur_set{ pt }; // cur set tracks all air cubes of current bubble (only connecting air cubes are included!)
        while (!new_pts.empty())
        {
            LDrop cur_pt = *new_pts.begin();
            new_pts.erase(new_pts.begin());
            if (std::binary_search(cubes.begin(), cubes.end(), cur_pt))
            {
                cur_set.erase(cur_pt);
                continue;
            }
            if (cand.count(cur_pt) == 0) // if we find a neighbor that is no candidate and no member of cubes, we found a way out of lava cube
            {                            // and all connecting points are not wholly surrounded by lava -> no inner points
                is_inner = false;
                continue;
            }
            else
            {
                cand.erase(cur_pt);
            }
            if (cur_set.insert(LDrop{cur_pt.x+1, cur_pt.y, cur_pt.z}).second) new_pts.push_back(LDrop{cur_pt.x+1, cur_pt.y, cur_pt.z});
            if (cur_set.insert(LDrop{cur_pt.x-1, cur_pt.y, cur_pt.z}).second) new_pts.push_back(LDrop{cur_pt.x-1, cur_pt.y, cur_pt.z});
            if (cur_set.insert(LDrop{cur_pt.x, cur_pt.y+1, cur_pt.z}).second) new_pts.push_back(LDrop{cur_pt.x, cur_pt.y+1, cur_pt.z});
            if (cur_set.insert(LDrop{cur_pt.x, cur_pt.y-1, cur_pt.z}).second) new_pts.push_back(LDrop{cur_pt.x, cur_pt.y-1, cur_pt.z});
            if (cur_set.insert(LDrop{cur_pt.x, cur_pt.y, cur_pt.z+1}).second) new_pts.push_back(LDrop{cur_pt.x, cur_pt.y, cur_pt.z+1});
            if (cur_set.insert(LDrop{cur_pt.x, cur_pt.y, cur_pt.z-1}).second) new_pts.push_back(LDrop{cur_pt.x, cur_pt.y, cur_pt.z-1});

        }
        if (is_inner)
        {
            std::vector<LDrop> inner_vec{ cur_set.begin(), cur_set.end() };
            num_inner_side += 6 * inner_vec.size() - get_num_connected_sides(inner_vec);
        }

    }
    return num_inner_side;
}

std::set<LDrop> get_air_candidates(std::vector<LDrop> &cubes, const MinMaxMap &min_max_x, const MinMaxMap &min_max_y, const MinMaxMap &min_max_z)
{
    std::sort(cubes.begin(), cubes.end());
    std::set<LDrop> candidates;
    uint32_t air_ctr{ 0u };

    for (const auto &elem : min_max_x)
    {
        LCoord y = elem.first.first;
        LCoord z = elem.first.second;
        LCoord x = elem.second.first + 1;
        for (; x<elem.second.second; ++x)
        {
            auto ity = min_max_y.find({ x,z });
            if (ity == min_max_y.end()) continue;
            auto itz = min_max_z.find({ x,y });
            if (itz == min_max_z.end()) continue;
            if (z > itz->second.first && z < itz->second.second && 
                y > ity->second.first && y < ity->second.second ) 
            {
                LDrop tmp{ x,y,z };
                if (std::binary_search(cubes.begin(), cubes.end(), tmp) == false)
                {
                    candidates.insert(tmp);
                    ++air_ctr;
                }
            }
        }
    }
    return candidates;
}

uint32_t get_num_connected_sides(const std::vector<LDrop> &cubes)
{
    uint32_t num_conn{ 0u };

    for (size_t i=0; i<cubes.size(); ++i)
    {
        for (size_t j=i+1; j<cubes.size(); ++j)
        {
            // if (dist_squ<decltype(cubes.at(0).x)>(cubes.at(i) - cubes.at(j)) == 1)
            if (dist_squ<LCoord>(cubes.at(i) - cubes.at(j)) == 1)
            {
                ++num_conn;
            }
        }
    }
    return 2 * num_conn; // multiply with 2 for each connection is only counted once in for loop above
}


std::vector<LDrop> get_cubes(const std::string &file_path, MinMaxMap &min_max_x, MinMaxMap &min_max_y, MinMaxMap &min_max_z)
{
    std::vector<LDrop> cubes;
    std::vector<std::string> data = read_string_vec_from_file(file_path);
    for (const auto &line : data)
    {
        LDrop tmp{ parse_string_to_number_vec<LCoord>(line) };
        cubes.push_back(tmp);
        std::pair<LCoord,LCoord> id{tmp.y, tmp.z};
        auto it = min_max_x.find(id);
        if (it == min_max_x.end())
        {
            min_max_x[id] = { tmp.x, tmp.x };
        }
        else
        {
            if (tmp.x < min_max_x[id].first)  min_max_x[id].first = tmp.x;
            if (tmp.x > min_max_x[id].second) min_max_x[id].second = tmp.x;
        }
        std::pair<LCoord,LCoord> idy{tmp.x, tmp.z};
        it = min_max_y.find(idy);
        if (it == min_max_y.end())
        {
            min_max_y[idy] = { tmp.y, tmp.y };
        }
        else
        {
            if (tmp.y < min_max_y[idy].first)  min_max_y[idy].first = tmp.y;
            if (tmp.y > min_max_y[idy].second) min_max_y[idy].second = tmp.y;
        }
        std::pair<LCoord,LCoord> idz{tmp.x, tmp.y};
        it = min_max_z.find(idz);
        if (it == min_max_z.end())
        {
            min_max_z[idz] = { tmp.z, tmp.z };
        }
        else
        {
            if (tmp.z < min_max_z[idz].first)  min_max_z[idz].first = tmp.z;
            if (tmp.z > min_max_z[idz].second) min_max_z[idz].second = tmp.z;
        }
    }
    return cubes;
}

// void print_cubes(std::ostream &out, const std::vector<LDrop> &cubes)
// {
//     std::sort(cubes.begin(), cubes.end(), std::greater<LDrop>());


// }