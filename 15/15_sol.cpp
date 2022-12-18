#include <string>
#include <vector>
#include <algorithm>

#include "../utility.h"
#include "scanner.h"

// constexpr std::int64_t Y_ROW{ 10u };
// constexpr std::int64_t BEACON_LIMIT_LOW{ 0u };
// constexpr std::int64_t BEACON_LIMIT_HIGH{ 20u };
constexpr std::int64_t Y_ROW{ 2'000'000u };
constexpr std::int64_t BEACON_LIMIT_LOW{ 0u };
constexpr std::int64_t BEACON_LIMIT_HIGH{ 4'000'000u };
constexpr std::int64_t X_FACTOR{ 4'000'000u };

std::vector<Scanner> read_scanners_in(const std::string &file_path);
std::uint64_t count_elements(const std::vector<Range> &ranges);
std::vector<Range> range_merge(std::vector<Range> &ranges);
std::uint64_t find_vacant_col(const std::vector<Range> &merged_ranges);

std::uint64_t day_15_1(const std::string &file_path)
{
    std::vector<Scanner> scanner_vec = read_scanners_in(file_path);
    std::vector<Range> range_vec;
    for (const auto &scanner : scanner_vec)
    {
        if (scanner.is_row_in_range(Y_ROW))
        {
            range_vec.push_back(scanner.get_range_in_row(Y_ROW));   
        }
    }
    std::vector<Range> merged_ranges = range_merge(range_vec);

    return count_elements(merged_ranges);
}

std::uint64_t day_15_2(const std::string &file_path)
{
    std::vector<Scanner> scanner_vec = read_scanners_in(file_path);
    std::vector<Range> merged_ranges;
    std::uint64_t row{ BEACON_LIMIT_LOW };
    
    for (; row<=BEACON_LIMIT_HIGH; ++row)
    {
        std::vector<Range> range_vec;
        for (const auto &scanner : scanner_vec)
        {
            if (scanner.is_row_in_range(row))
            {
                Range new_range = scanner.get_range_in_row(row);
                if (new_range.start < BEACON_LIMIT_LOW) new_range.start = BEACON_LIMIT_LOW;
                if (new_range.end > BEACON_LIMIT_HIGH) new_range.end = BEACON_LIMIT_HIGH;
                range_vec.push_back(new_range);   
            }
        }
        merged_ranges = range_merge(range_vec);
        if (count_elements(merged_ranges) != BEACON_LIMIT_HIGH)
        {
            break;
        }
    }
    std::uint64_t col = find_vacant_col(merged_ranges);
    return col * X_FACTOR + row;
}

std::vector<Range> range_merge(std::vector<Range> &ranges)
{
    std::sort(ranges.begin(), ranges.end());
    std::vector<Range> merged_ranges{ ranges.at(0) };
    for (size_t i=1; i<ranges.size(); ++i)
    {
        Range &r_cur = ranges[i]; 
        if (r_cur.start <= merged_ranges.back().end) // overlap
        {
            if (r_cur.end > merged_ranges.back().end)
            {
                merged_ranges.back().end = r_cur.end;
            }
        }
        else // no overlap -> add new range
        {
            merged_ranges.push_back(r_cur);
        }
    }
    return merged_ranges;
}

std::uint64_t find_vacant_col(const std::vector<Range> &merged_ranges)
{
    if (merged_ranges.front().start > BEACON_LIMIT_LOW) 
    {
        return BEACON_LIMIT_LOW;
    }
    if (merged_ranges.back().end < BEACON_LIMIT_HIGH)
    {
        return BEACON_LIMIT_HIGH;
    }
    if (merged_ranges.size() != 2)
    {
        throw std::runtime_error("Range should have size of exactly two!");
    }
    return merged_ranges.front().end + 1;
}

std::uint64_t count_elements(const std::vector<Range> &ranges)
{
    std::uint64_t count{ 0u };
    for (const auto &range : ranges)
    {
        count += range.end - range.start;
    }
    return count;
}

std::vector<Scanner> read_scanners_in(const std::string &file_path)
{
    std::vector<std::string> str_vec = read_string_vec_from_file(file_path);
    std::vector<Scanner> scanner_vec;

    for (const auto &line : str_vec)
    {
        std::vector<std::int64_t> s_data = parse_string_to_number_vec<std::int64_t>(line);
        std::uint64_t dist = std::abs(s_data.at(0) - s_data.at(2)) + std::abs(s_data.at(1) - s_data.at(3));
        SPos s{ s_data.at(0), s_data.at(1) };
        SPos b{ s_data.at(2), s_data.at(3) };
        scanner_vec.push_back({ s, b, dist });
    }
    return scanner_vec;
}



