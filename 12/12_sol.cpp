#include <string>
#include <set>
#include<algorithm>

#include "../utility.h"

constexpr char start_char{'S'};
constexpr char end_char{'E'};

using HeightMap = std::vector<std::vector<char>>;
using IndPos = Point<size_t>;

// declarations
std::pair<IndPos, IndPos> get_start_end_pos(const HeightMap &height_map, char start, char end);
void update_reachable_pts(const HeightMap &height_map, std::set<IndPos> &reachable_pts, std::set<IndPos> &new_reachable_pts, IndPos pos);
void update_reachable_pts_2(const HeightMap &height_map, std::set<IndPos> &reachable_pts, std::set<IndPos> &new_reachable_pts, IndPos pos);
std::uint32_t breadth_first_search(const HeightMap &height_map, IndPos s_pos, IndPos e_pos);
std::uint32_t breadth_first_search_2(const HeightMap &height_map, IndPos s_pos, char end_char);

std::uint32_t day_12_1(const std::string &file_path)
{
    HeightMap height_map = read_2d_vec_from_file<char>(file_path);
    // set height of start to 'a' and height of end to 'z'
    std::pair<IndPos, IndPos> start_end_pos = get_start_end_pos(height_map, start_char, end_char);
    IndPos &s_pos = start_end_pos.first;
    IndPos &e_pos = start_end_pos.second;    
    height_map[s_pos.x][s_pos.y] = 'a';
    height_map[e_pos.x][e_pos.y] = 'z';

    return breadth_first_search(height_map, s_pos, e_pos);
}

// Idea reverse the search, starting from end until we hit the first 'a'
std::uint32_t day_12_2(const std::string &file_path)
{
    HeightMap height_map = read_2d_vec_from_file<char>(file_path);
    // set height of start to 'a' and height of end to 'z'
    std::pair<IndPos, IndPos> start_end_pos = get_start_end_pos(height_map, start_char, end_char);
    IndPos &s_pos = start_end_pos.first;
    IndPos &e_pos = start_end_pos.second;    
    height_map[s_pos.x][s_pos.y] = 'a';
    height_map[e_pos.x][e_pos.y] = 'z';

    return breadth_first_search_2(height_map, e_pos, 'a');
}

// do adapted variant of breadth first search
std::uint32_t breadth_first_search(const HeightMap &height_map, IndPos s_pos, IndPos e_pos)
{
    std::set<IndPos> reachable_set{ s_pos };
    std::uint32_t iteration{ 0u };
    std::set<IndPos> newly_reach_pts{ s_pos };

    while (newly_reach_pts.find(e_pos) == newly_reach_pts.end())
    {
        std::set<IndPos> newly_reach_pts_tmp = newly_reach_pts;
        newly_reach_pts.clear();
        for (const auto &pos : newly_reach_pts_tmp)
        {
            update_reachable_pts(height_map, newly_reach_pts, newly_reach_pts, pos);
        }
        ++iteration;
    }
    return iteration;
}

std::uint32_t breadth_first_search_2(const HeightMap &height_map, IndPos s_pos, char end_char)
{
    std::set<IndPos> reachable_set{ s_pos };
    std::uint32_t iteration{ 0u };
    std::set<IndPos> newly_reach_pts{ s_pos };

    auto find_end = [&height_map, end_char](const IndPos &pos)
    {
        return height_map.at(pos.x).at(pos.y) == end_char;
    };

    while (std::find_if(newly_reach_pts.begin(), newly_reach_pts.end(), find_end) == newly_reach_pts.end())
    {
        std::set<IndPos> newly_reach_pts_tmp = newly_reach_pts;
        newly_reach_pts.clear();
        for (const auto &pos : newly_reach_pts_tmp)
        {
            update_reachable_pts_2(height_map, newly_reach_pts, newly_reach_pts, pos);
        }
        ++iteration;
    }
    return iteration;
}

/**
 * @brief 
 * 
 * @param height_map 
 * @param reachable_pts set of all checked points 
 * @param new_reachable_pts set of all points newly checked during this iteration -> starting point for neighbro search in next iteration
 * @param pos 
 */
void update_reachable_pts(const HeightMap &height_map, std::set<IndPos> &reachable_pts, std::set<IndPos> &new_reachable_pts, IndPos pos)
{
    const int ref_height = height_map.at(pos.x).at(pos.y) + 2;

    // check left neighbor
    if (pos.x > 0 && static_cast<int>(height_map.at(pos.x - 1).at(pos.y)) < ref_height)
    {
        if ( reachable_pts.insert({ pos.x - 1, pos.y }).second )
        {
            new_reachable_pts.insert({ pos.x - 1, pos.y });
        }
    }
    // check upper neighbor
    if (pos.y > 0 && static_cast<int>(height_map.at(pos.x).at(pos.y - 1)) < ref_height)
    {
        if ( reachable_pts.insert({ pos.x, pos.y - 1 }).second )
        {
            new_reachable_pts.insert({ pos.x, pos.y - 1 });
        }
    }
    // check right neighbor
    if (pos.x < height_map.size()-1 && static_cast<int>(height_map.at(pos.x + 1).at(pos.y)) < ref_height)
    {
        reachable_pts.insert({ pos.x + 1, pos.y });
        if ( reachable_pts.insert({ pos.x + 1, pos.y }).second )
        {
            new_reachable_pts.insert({ pos.x + 1, pos.y });
        }        
    }
    // check below neighbor
    if (pos.y < height_map.at(0).size()-1 && static_cast<int>(height_map.at(pos.x).at(pos.y + 1)) < ref_height)
    {
        if ( reachable_pts.insert({ pos.x, pos.y + 1 }).second )
        {
            new_reachable_pts.insert({ pos.x, pos.y + 1 });
        }        
    }
}

/**
 * @brief 
 * 
 * @param height_map 
 * @param reachable_pts set of all checked points 
 * @param new_reachable_pts set of all points newly checked during this iteration -> starting point for neighbro search in next iteration
 * @param pos 
 */
void update_reachable_pts_2(const HeightMap &height_map, std::set<IndPos> &reachable_pts, std::set<IndPos> &new_reachable_pts, IndPos pos)
{
    const int ref_height = height_map.at(pos.x).at(pos.y) - 2;

    // check left neighbor
    if (pos.x > 0 && static_cast<int>(height_map.at(pos.x - 1).at(pos.y)) > ref_height)
    {
        if ( reachable_pts.insert({ pos.x - 1, pos.y }).second )
        {
            new_reachable_pts.insert({ pos.x - 1, pos.y });
        }
    }
    // check upper neighbor
    if (pos.y > 0 && static_cast<int>(height_map.at(pos.x).at(pos.y - 1)) > ref_height)
    {
        if ( reachable_pts.insert({ pos.x, pos.y - 1 }).second )
        {
            new_reachable_pts.insert({ pos.x, pos.y - 1 });
        }
    }
    // check right neighbor
    if (pos.x < height_map.size()-1 && static_cast<int>(height_map.at(pos.x + 1).at(pos.y)) > ref_height)
    {
        reachable_pts.insert({ pos.x + 1, pos.y });
        if ( reachable_pts.insert({ pos.x + 1, pos.y }).second )
        {
            new_reachable_pts.insert({ pos.x + 1, pos.y });
        }        
    }
    // check below neighbor
    if (pos.y < height_map.at(0).size()-1 && static_cast<int>(height_map.at(pos.x).at(pos.y + 1)) > ref_height)
    {
        if ( reachable_pts.insert({ pos.x, pos.y + 1 }).second )
        {
            new_reachable_pts.insert({ pos.x, pos.y + 1 });
        }        
    }
}


std::pair<IndPos, IndPos> get_start_end_pos(const HeightMap &height_map, char start, char end)
{
    std::pair<IndPos, IndPos> positions;
    bool start_found{false};
    bool end_found{false};
    for (size_t x = 0; x < height_map.size(); ++x)
    {
        for (size_t y = 0; y < height_map[0].size(); ++y)
        {
            if (height_map.at(x).at(y) == start)
            {
                start_found = true;
                positions.first = IndPos{x, y};
            }
            if (height_map.at(x).at(y) == end)
            {
                end_found = true;
                positions.second = IndPos{x, y};
            }
            if (start_found && end_found)
            {
                return positions;
            }
        }
    }
    return positions;
}