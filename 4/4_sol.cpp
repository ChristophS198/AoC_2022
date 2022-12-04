#include <string>
#include <vector>

#include "../utility.h"


using SectionId = std::uint16_t;
using Section = std::pair<SectionId,SectionId>;

std::vector<std::pair<Section,Section>> parse_to_section_pairs(const std::vector<std::string> &section_vec);
std::uint32_t num_fully_contained_sections(const std::vector<std::pair<Section,Section>> &section_vec);
std::uint32_t num_overlapping_sections(const std::vector<std::pair<Section,Section>> &section_vec);

std::uint32_t day_4_1(const std::string &file_path)
{
    std::vector<std::string> section_vec = read_string_vec_from_file(file_path);
    std::vector<std::pair<Section,Section>> section_pair_vec = parse_to_section_pairs(section_vec);
    return num_fully_contained_sections(section_pair_vec);
}

std::uint32_t day_4_2(const std::string &file_path)
{
    std::vector<std::string> section_vec = read_string_vec_from_file(file_path);
    std::vector<std::pair<Section,Section>> section_pair_vec = parse_to_section_pairs(section_vec);
    return num_overlapping_sections(section_pair_vec);
}

std::uint32_t num_overlapping_sections(const std::vector<std::pair<Section,Section>> &section_vec)
{
    // Calc how many sections have no overlap and substract them from overall number of section pairs
    std::uint32_t non_overlapping_sum{ 0 }; 
    for (const auto &sec : section_vec)
    {
        if (sec.first.first > sec.second.second ||
            sec.first.second < sec.second.first)
        {
            ++non_overlapping_sum;
        }
    }
    return section_vec.size() - non_overlapping_sum;
}

std::uint32_t num_fully_contained_sections(const std::vector<std::pair<Section,Section>> &section_vec)
{
    std::uint32_t sum{ 0 };
    for (const auto &sec : section_vec)
    {
        if (sec.first.first <= sec.second.first &&
            sec.first.second >= sec.second.second)
        {
            ++sum;
        }
        else
        {
            if (sec.second.first <= sec.first.first &&
                sec.second.second >= sec.first.second)
            {
                ++sum;
            }
        }
    }
    return sum;
}

std::vector<std::pair<Section,Section>> parse_to_section_pairs(const std::vector<std::string> &section_vec)
{
    std::vector<std::pair<Section,Section>> section_pairs;
    for (const auto &sec : section_vec)
    {
        std::vector<SectionId> sec_ids = parse_string_to_number_vec<SectionId>(sec);
        section_pairs.push_back(std::make_pair(Section(sec_ids[0], sec_ids[1]), Section(sec_ids[2], sec_ids[3])));
    }
    return section_pairs;
}