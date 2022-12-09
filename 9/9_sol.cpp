#include <string>
#include <vector>

#include "../utility.h"

constexpr std::int32_t DEFAULT_SIZE = 1000;

using Steps = std::uint16_t;
using RopeField = std::vector<std::vector<bool>>;

enum class Move : std::uint8_t
{
    Left, 
    Right, 
    Up, 
    Down
};

struct MoveOp
{
    Move move;
    Steps steps;
};

std::vector<MoveOp> read_in_move_operations(const std::string &file_path);
std::uint32_t get_num_visited_fields(RopeField &field, const std::vector<MoveOp> &moves, std::int32_t x=0, std::int32_t y=0, size_t num_knots=1u);
void follow_knot(const std::int32_t h_x, const std::int32_t h_y, std::int32_t &t_x, std::int32_t &t_y);

std::uint32_t day_9_1(const std::string &file_path)
{
    std::vector<MoveOp> move_ops = read_in_move_operations(file_path);
    RopeField field(DEFAULT_SIZE, std::vector<bool>(DEFAULT_SIZE,false));
    
    return get_num_visited_fields(field, move_ops, 500, 500, 1u);
}

std::uint32_t day_9_2(const std::string &file_path)
{
    std::vector<MoveOp> move_ops = read_in_move_operations(file_path);
    RopeField field(DEFAULT_SIZE, std::vector<bool>(DEFAULT_SIZE,false));
    
    return get_num_visited_fields(field, move_ops, 500, 500, 9u);
}


std::uint32_t get_num_visited_fields(RopeField &field, const std::vector<MoveOp> &moves, std::int32_t h_x, std::int32_t h_y, size_t num_knots)
{
    std::uint32_t num_v{ 1u };
    std::vector<std::int32_t> k_x(num_knots, h_x); // x pos of knots after head
    std::vector<std::int32_t> k_y(num_knots, h_y); // y pos of knots after head
    std::int32_t &t_x = k_x.back(); 
    std::int32_t &t_y = k_y.back(); 
    field[t_x][t_y] = true;

    for (const auto &move : moves)
    {
        Steps num_steps = move.steps;
        while (num_steps > 0)
        {
            switch (move.move)
            {
            case Move::Left:
                --h_x;
                break;
            case Move::Right:
                ++h_x;
                break;
            case Move::Up:
                ++h_y;
                break;
            case Move::Down:
                --h_y;
                break;
            }
            --num_steps;
            if (h_x < 0 || h_y < 0)
            {
                std::string coords = "(x=" + std::to_string(h_x) + ",y=" + std::to_string(h_y) + ")";
                throw std::runtime_error(coords + "\nNegative index! Default field size too small!");
            }
            if (h_x >= DEFAULT_SIZE || h_y >= DEFAULT_SIZE)
            {
                std::string coords = "(x=" + std::to_string(h_x) + ",y=" + std::to_string(h_y) + ")";
                throw std::runtime_error(coords + "\nIndex too large! Increase default field size.");
            }
            follow_knot(h_x, h_y, k_x.front(), k_y.front());
            for (int32_t i=0; i<static_cast<int32_t>(num_knots)-1; ++i)
            {
                follow_knot(k_x.at(i), k_y.at(i), k_x.at(i+1), k_y.at(i+1));
            }
            if (!field.at(t_x).at(t_y))
            {
                field.at(t_x).at(t_y) = true;
                ++num_v;
            }
        }
    }
    return num_v;
}

void follow_knot(const std::int32_t h_x, const std::int32_t h_y, std::int32_t &t_x, std::int32_t &t_y)
{
    if (h_x == t_x)
    {
        t_y +=(h_y - t_y) / 2; // only if diff is 2 t_y is changed
    }
    else
    {
        if (h_y == t_y)
        {
            t_x += (h_x - t_x) / 2; // only if diff is 2 t_x is changed
        }
        else
        {
            if (std::abs(h_x - t_x) > 1 || std::abs(h_y - t_y) > 1)
            {
                t_x = h_x > t_x ? t_x+1 : t_x-1;
                t_y = h_y > t_y ? t_y+1 : t_y-1;
            }
        }
    }
}


std::vector<MoveOp> read_in_move_operations(const std::string &file_path)
{
    std::vector<MoveOp> move_ops;
    std::vector<std::string> str_vec = read_string_vec_from_file(file_path);
    for (const auto &str : str_vec)
    {
        MoveOp new_move;
        switch (str.at(0))
        {
        case 'L':
            new_move.move = Move::Left;
            break;
        case 'R':
            new_move.move = Move::Right;
            break;
        case 'U':
            new_move.move = Move::Up;
            break;
        case 'D':
            new_move.move = Move::Down;
            break;
        default:
            throw std::runtime_error("Undefined move operation!");
            break;
        }
        new_move.steps = parse_string_to_number<Steps>(str);
        move_ops.push_back(new_move);
    }
    return move_ops;
}
