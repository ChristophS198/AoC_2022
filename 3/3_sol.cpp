#include <string>
#include <vector>
#include <algorithm>

#include "../utility.h"

using Priority = std::uint32_t;

std::vector<char> get_common_compartment_items(const std::vector<std::string> &rucksack_vec);
Priority get_summed_priority(const std::vector<char> &item_vec);
std::vector<char> get_badge_items(const std::vector<std::string> &rucksack_vec);

Priority day_3_1(const std::string &file_path)
{
    std::vector<std::string> rucksack_vec = read_string_vec_from_file(file_path);
    std::vector<char> common_item = get_common_compartment_items(rucksack_vec);
    return get_summed_priority(common_item);
}

Priority day_3_2(const std::string &file_path)
{
    std::vector<std::string> rucksack_vec = read_string_vec_from_file(file_path);
    std::vector<char> common_item = get_badge_items(rucksack_vec);
    return get_summed_priority(common_item);
}

Priority get_summed_priority(const std::vector<char> &item_vec)
{
    Priority sum{ 0 };
    for (const auto &item : item_vec)
    {
        if (item >= 'A' && item <= 'Z') 
        {
            sum += item - 'A' + 27;
        }
        else 
        {
            if (item >= 'a' && item <= 'z') 
            {
                sum += item - 'a' + 1;   
            }
            else
            {
                std::string err_str = "Unknown charachter" + item + std::string(" found");
                throw std::invalid_argument(err_str);
            }
        }
    }
    return sum;
}

std::vector<char> get_common_compartment_items(const std::vector<std::string> &rucksack_vec)
{
    std::vector<char> common_items;
    for (const auto &rucksack : rucksack_vec)
    {
        std::string compart_1 = rucksack.substr(0,rucksack.length()/2);
        std::string compart_2 = rucksack.substr(rucksack.length()/2,rucksack.length()/2);        
        for (const auto &item : compart_1)
        {
            if (std::string::npos != compart_2.find(item))
            {
                common_items.push_back(item);
                break;
            }
        }
    }
    return common_items;
}

std::vector<char> get_badge_items(const std::vector<std::string> &rucksack_vec)
{
    std::vector<char> badge_vec;
    for (size_t i=0u; i<rucksack_vec.size(); i+=3)
    {
        std::string ruck_1 = rucksack_vec[i];
        std::string ruck_2 = rucksack_vec[i+1];
        std::string ruck_3 = rucksack_vec[i+2];
        for (const auto &item : ruck_1)
        {
            if (std::string::npos != ruck_2.find(item) && std::string::npos != ruck_3.find(item))
            {
                badge_vec.push_back(item);
                break;
            }
        }
    }
    return badge_vec;
}
