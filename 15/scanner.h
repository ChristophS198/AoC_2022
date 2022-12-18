#include <cstdint>

#include "../utility.h"


using BCount = std::uint64_t;
using SPos = Point<std::int64_t>;

struct Range
{
    std::int64_t start;
    std::int64_t end;
    bool operator<(const Range &other) const 
    {
        return start < other.start;
    }
    bool operator==(const Range &other) const 
    {
        return end == other.end && start == other.start;
    }
};

class Scanner
{
private:
    SPos pos{};
    SPos nearest_beacon_pos{};
    std::uint64_t man_dist{};
public:
    Scanner(SPos s_pos, SPos b_pos, std::uint64_t d) : pos{ s_pos }, nearest_beacon_pos{ b_pos }, man_dist{ d } {};

    bool is_row_in_range(std::int64_t y) const;
    Range get_range_in_row(std::int64_t y) const;
};

Range Scanner::get_range_in_row(std::int64_t y) const
{
    std::int64_t d = static_cast<std::int64_t>(man_dist) - std::abs(pos.y - y);
    return { pos.x - d, pos.x + d };
}

bool Scanner::is_row_in_range(std::int64_t y) const
{
    return std::abs(pos.y - y) <= static_cast<std::int64_t>(man_dist);
}
