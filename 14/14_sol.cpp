#include <string>
#include <array>
#include <fstream>

#include "../utility.h"

using SandC = std::uint64_t;
using RockField = std::vector<std::vector<bool>>;
using Pos = Point<std::uint32_t>;
using Line = std::vector<Pos>;

const Pos sand{ 500u,0u };
constexpr size_t X_EXTENSION{ 500u };

RockField get_rock_structure(const std::string &file_path, Pos &sand_source);
RockField get_rock_structure_2(const std::string &file_path, Pos &sand_source);
void draw_hor_vert_line(RockField &rock, Pos start, Pos end);
void resize_grid(RockField &field, const size_t new_dim);
bool drop_sand(RockField &field, Pos source);
bool sand_step(RockField &field, Pos &s, bool &finished);
bool sand_step_2(RockField &field, Pos &s);
bool drop_sand_2(RockField &field, Pos source);

// debug
std::ostream& print_grid(std::ostream& out, const RockField &rock_field);


SandC day_14_1(const std::string &file_path)
{
    Pos sand_source = sand;
    RockField field = get_rock_structure(file_path, sand_source);
    SandC ctr{ 0u };
    while(drop_sand(field, sand_source))
    {
        ++ctr;
    }
    std::ofstream out_stream("14/Rock_2.txt");
    print_grid(out_stream, field);
    out_stream.close();
    return ctr;
}

SandC day_14_2(const std::string &file_path)
{
    Pos sand_source = sand;
    RockField field = get_rock_structure_2(file_path, sand_source);
    SandC ctr{ 1u };
    while(drop_sand_2(field, sand_source))
    {
        ++ctr;
    }
    // std::ofstream out_stream("14/Rock_2.txt");
    // print_grid(out_stream, field);
    // out_stream.close();
    return ctr;
}


bool drop_sand_2(RockField &field, Pos source)
{
    Pos tmp = source;
    while(sand_step_2(field, tmp)) 
    {
        ;
    }
    if (tmp == source)
    {
        return false;
    }
    return true;
}

bool sand_step_2(RockField &field, Pos &s)
{
    try
    {
        if (!field.at(s.y+1).at(s.x)) // check y-dir
        {
            ++s.y;
            return true;
        }
        else 
        {
            if (!field.at(s.y+1).at(s.x-1)) // check left + down
            {
                ++s.y;
                --s.x;
                return true;
            }
            else
            {
                if (!field.at(s.y+1).at(s.x+1)) // check right + down
                {
                    ++s.y;
                    ++s.x;
                    return true;
                }
                else
                {
                    field.at(s.y).at(s.x) = true;
                    return false;
                }
            }
        }
    }
    catch(...)
    {
        throw std::runtime_error("Pos " + std::to_string(s.y) + "," + std::to_string(s.x) + " exceeds limits!");
    }
    throw std::runtime_error("Should not be reached!");
}

bool drop_sand(RockField &field, Pos source)
{
    bool more_sand{ true };
    Pos tmp = source;
    while(sand_step(field, tmp, more_sand)) 
    {
        ;
    }
    return more_sand;
}

bool sand_step(RockField &field, Pos &s, bool &more_sand)
{
    try
    {
        if (!field.at(s.y+1).at(s.x)) // check y-dir
        {
            ++s.y;
            return true;
        }
        else 
        {
            if (!field.at(s.y+1).at(s.x-1)) // check left + down
            {
                ++s.y;
                --s.x;
                return true;
            }
            else
            {
                if (!field.at(s.y+1).at(s.x+1)) // check right + down
                {
                    ++s.y;
                    ++s.x;
                    return true;
                }
                else
                {
                    field.at(s.y).at(s.x) = true;
                    return false;
                }
            }
        }
    }
    catch(...)
    {
        more_sand = false;
        return false;
    }
    throw std::runtime_error("Should not be reached!");
}

void draw_hor_vert_line(RockField &rock, Pos start, Pos end)
{
    auto dim_y = rock.size() - 1;
    auto dim_x = rock.front().size() - 1;
    if (start.x > dim_x || end.x > dim_x)
    {
        throw std::runtime_error("Grid size in x dimension not large enough!");
        // resize_grid(rock, max_pos);
    }
    if (start.y > dim_y || end.y > dim_y)
    {
        throw std::runtime_error("Grid size in y dimension not large enough!");
        // resize_grid(rock, max_pos);
    }    
    if (start.x != end.x)
    {
        int inc = static_cast<int>(end.x) - static_cast<int>(start.x);
        inc = inc / std::abs(inc);
        while (start.x != end.x)
        {
            rock.at(start.y).at(start.x) = true;
            start.x += inc;
        }
        rock.at(end.y).at(end.x) = true;
    }
    else
    {
        if (start.y != end.y)
        {
            int inc = static_cast<int>(end.y) - static_cast<int>(start.y);
            inc = inc / std::abs(inc);
            while (start.y != end.y)
            {
                rock.at(start.y).at(start.x) = true;
                start.y += inc;
            }
            rock.at(end.y).at(end.x) = true;
        }
    }
}

RockField get_rock_structure_2(const std::string &file_path, Pos &sand_source)
{
    std::vector<std::string> rock_strings = read_string_vec_from_file(file_path);
    std::vector<Pos> rock_lines;
    std::uint32_t max_x{ 0u };
    std::uint32_t max_y{ 0u };
    std::uint32_t min_x{ sand.x };
    std::uint32_t min_y{ sand.y };
    std::vector<Line> line_vec;

    for (const auto &line : rock_strings)
    {
        std::vector<std::uint32_t> line_num = parse_string_to_number_vec<std::uint32_t>(line);
        size_t i{ 0u };
        Line line_v;
        while (i < (line_num.size() - 1))
        {
            Pos pos{ line_num.at(i),line_num.at(i+1) };
            if (pos.x < min_x) min_x = pos.x;
            if (pos.x > max_x) max_x = pos.x;
            if (pos.y < min_y) min_y = pos.y;
            if (pos.y > max_y) max_y = pos.y;
            line_v.push_back(pos);
            i += 2;
        }
        line_vec.push_back(line_v);
    }
    
    RockField rock_field(max_y+2, std::vector<bool>(max_x+X_EXTENSION, false));
    rock_field.push_back(std::vector<bool>(max_x+X_EXTENSION, true));

    for (auto &line : line_vec)
    {
        for (size_t i=0; i<line.size()-1; ++i)
        {
            draw_hor_vert_line(rock_field, line.at(i), line.at(i+1));
        }
    }

    // std::ofstream out_stream("14/Rock_1.txt");
    // print_grid(out_stream, rock_field);
    // out_stream.close();
    return rock_field;
}

RockField get_rock_structure(const std::string &file_path, Pos &sand_source)
{
    std::vector<std::string> rock_strings = read_string_vec_from_file(file_path);
    std::vector<Pos> rock_lines;
    std::uint32_t max_x{ 0u };
    std::uint32_t max_y{ 0u };
    std::uint32_t min_x{ sand.x };
    std::uint32_t min_y{ sand.y };
    std::vector<Line> line_vec;

    for (const auto &line : rock_strings)
    {
        std::vector<std::uint32_t> line_num = parse_string_to_number_vec<std::uint32_t>(line);
        size_t i{ 0u };
        Line line_v;
        while (i < (line_num.size() - 1))
        {
            Pos pos{ line_num.at(i),line_num.at(i+1) };
            if (pos.x < min_x) min_x = pos.x;
            if (pos.x > max_x) max_x = pos.x;
            if (pos.y < min_y) min_y = pos.y;
            if (pos.y > max_y) max_y = pos.y;
            line_v.push_back(pos);
            i += 2;
        }
        line_vec.push_back(line_v);
    }
    RockField rock_field(max_y-min_y+1, std::vector<bool>(max_x-min_x+1));
    Pos offset{ min_x, min_y };
    sand_source = sand_source - offset;
    for (auto &line : line_vec)
    {
        line[0] = line[0] - offset;
        for (size_t i=0; i<line.size()-1; ++i)
        {
            line[i+1] = line[i+1] - offset;
            draw_hor_vert_line(rock_field, line.at(i), line.at(i+1));
        }
    }

    std::ofstream out_stream("14/Rock_1.txt");
    print_grid(out_stream, rock_field);
    out_stream.close();
    return rock_field;
}

void resize_grid(RockField &field, const size_t new_dim)
{
    field.resize(new_dim);
    for (auto& vec : field) 
    {
        vec.resize(new_dim);
    }
}

std::ostream& print_grid(std::ostream& out, const RockField &rock_field)
{
    for (const auto& row : rock_field)
    {
        for (const auto& val : row) 
        {
            out << val << " ";
        }
        out << "\n";
    }
    return out;
}