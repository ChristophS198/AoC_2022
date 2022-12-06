#include <string>
#include <map>

#include "../utility.h"


std::uint32_t get_first_pos_of_x_dist_chars(const std::uint16_t x, const std::string &data);

std::uint32_t day_6_1(const std::string &file_path)
{
    std::string data = read_string_from_file(file_path);
    return get_first_pos_of_x_dist_chars(4, data);
}


std::uint32_t day_6_2(const std::string &file_path)
{
    std::string data = read_string_from_file(file_path);
    return get_first_pos_of_x_dist_chars(14, data);
}

std::uint32_t get_first_pos_of_x_dist_chars(const std::uint16_t x, const std::string &data)
{
    std::map<char, std::uint8_t> char_count;
    size_t i=0u;
    while (i < x)
    {
        ++char_count[data[i++]]; 
    }
    std::uint32_t pos{ x };
    while (char_count.size() != x)
    {
        if (char_count[data[pos-x]] == 1) 
        {
            char_count.erase(data[pos-x]);
        }
        else
        {
            --char_count[data[pos-x]];
        }
        ++char_count[data[pos]]; 
        ++pos;
    }
    return pos;
}