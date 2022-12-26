/**
The solution for part 2 only works for cubes that have the following layout:
  AABB
  AABB
  CC
  CC
EEDD
FF
Each cube face must also have dimenstion 50x50!
 * 
 */
#include <string>
#include <map>

#include "../utility.h"

constexpr char WALL{ '#'};
constexpr char FREE{ '.'};
constexpr char NO_MAP{ ' '};
constexpr char LEFT{ 'L' };
constexpr char RIGHT{ 'R' };

enum class MDir : uint8_t
{
    Left,
    Right,
    Empty
};

enum class MFacing : uint8_t
{
    Right, 
    Down,
    Left,
    Up
};

struct Instruction
{
    uint16_t steps;
    MDir dir;
};

struct State22
{
    uint32_t row;
    uint32_t col;
    MFacing facing;
    char cube_face; // denotes on whiche cube face we are on: A, B, ..., F
};
 
struct LinearFunc
{
    int32_t m;
    int32_t t;
    uint32_t operator()(int32_t x) { return static_cast<uint32_t>(m * x + t); }
};

struct Transformation
{
    LinearFunc row_trafo;
    LinearFunc col_trafo;
    uint8_t facing_offset;
    State22 operator()(State22 s1) 
    { 
        State22 new_state;
        if (facing_offset % 2 == 0)
        {
            new_state.row = row_trafo(s1.row);
            new_state.col = col_trafo(s1.col);
        }
        else 
        {
            new_state.row = row_trafo(s1.col);
            new_state.col = col_trafo(s1.row);
        }
        new_state.facing = static_cast<MFacing>((static_cast<uint8_t>(s1.facing)+facing_offset)%4);
        new_state.cube_face = s1.cube_face; // is set here to old face
        return new_state;
    }
};

using MMap = std::vector<std::vector<char>>; // Monkey Map is a 2-D vector
using CubeTrafos = std::map<std::pair<char, char>, Transformation>;

std::pair<MMap, std::vector<Instruction>> get_data_in(const std::string &file_path);
std::vector<Instruction> parse_instructions(const std::string &ins_string);
void resize_monkey_map(MMap &monkey_map, const size_t max_len);
State22 get_init_state(const MMap &monkey_map);
State22 execute_instructions(const MMap &monkey_map, const std::vector<Instruction> ins_vec, const State22 init_state, bool do_cube_wrapping=false);
State22 do_single_instruction(const MMap &monkey_map, const Instruction ins, const State22 cur_state, const CubeTrafos &trafos, bool do_cube_wrapping=false);
bool do_wrap_around(const MMap &monkey_map, State22 &cur_state);
bool do_cube_wrapping(const MMap &monkey_map, const CubeTrafos &trafos, State22 &cur_state);
char get_cube_face(const State22 &cur_state);
char get_nxt_cube_face(const State22 &cur_state);
CubeTrafos get_transformations();

int32_t day_22_1(const std::string &file_path)
{
    std::pair<MMap, std::vector<Instruction>> data = get_data_in(file_path);
    State22 init_state = get_init_state(data.first);

    State22 end_state = execute_instructions(data.first, data.second, init_state);

    return 1000u * end_state.row + 4u * end_state.col + static_cast<uint32_t>(end_state.facing);
}

int32_t day_22_2(const std::string &file_path)
{
    std::pair<MMap, std::vector<Instruction>> data = get_data_in(file_path);
    State22 init_state = get_init_state(data.first);

    State22 end_state = execute_instructions(data.first, data.second, init_state, true);

    return 1000u * end_state.row + 4u * end_state.col + static_cast<uint32_t>(end_state.facing);
}

State22 do_single_instruction(const MMap &monkey_map, const Instruction ins, const State22 cur_state, const CubeTrafos &trafos, bool is_cube_wrapping)
{
    State22 new_state{ cur_state };
    State22 tmp_state{ };
    uint32_t col{ cur_state.col };
    uint32_t row{ cur_state.row };

    for (auto steps=0u; steps<ins.steps; ++steps)
    {
        switch (new_state.facing)
        {
        case MFacing::Left:
            --col;
            break;
        case MFacing::Right:
            ++col;
            break;
        case MFacing::Up:
            --row;
            break;
        case MFacing::Down:
            ++row;
            break;
        }
        bool wrap_successful{ false };
        switch (monkey_map.at(row).at(col))
        {
        case NO_MAP: /*We would enter a empty tile and need to wrap around if there is no Wall on the other side*/
            tmp_state = new_state;
            if (is_cube_wrapping) 
            {
                wrap_successful = do_cube_wrapping(monkey_map, trafos, tmp_state);
            }
            else 
            {
                wrap_successful = do_wrap_around(monkey_map, tmp_state); 
            }
            if (false == wrap_successful)
            {
                steps = ins.steps; // If we cannot wrap around stop moving
            }
            else
            {
                row = tmp_state.row;
                col = tmp_state.col;
                new_state = tmp_state;
            }
            break;
        case WALL: /*We hit a wall and stop moving -> set steps to ins.steps so end condition of loop is met*/
            steps = ins.steps;
            break;
        case FREE: /*Do nothing special*/
            new_state.row = row;
            new_state.col = col;
            break;
        }

        // set new cube face (is done after each step)
        new_state.cube_face = get_cube_face(new_state);
    }

    // set new heading 
    uint8_t old_facing = static_cast<uint8_t>(new_state.facing);
    switch (ins.dir)
    {
    case MDir::Left:
        new_state.facing = static_cast<MFacing>((old_facing + 3) % 4); // +3 cooresponds to -1, but avoids negative numbers that cause problems with %
        break;
    case MDir::Right:
        new_state.facing = static_cast<MFacing>((old_facing + 1) % 4); 
        break;
    case MDir::Empty:
        /**Do nothing*/
        break;
    }


    return new_state;
}

bool do_cube_wrapping(const MMap &monkey_map, const CubeTrafos &trafos, State22 &cur_state)
{
    char cur_face = get_cube_face(cur_state);
    char nxt_face = get_nxt_cube_face(cur_state);

    Transformation trafo = trafos.at({ cur_face, nxt_face });
    State22 new_state = trafo(cur_state);

    if (monkey_map.at(new_state.row).at(new_state.col) == FREE)
    {
        cur_state = new_state;
        return true;
    }
    else
    {
        return false;
    }
}

/* In case of cube transitions: Provides new cube face based on current cube face and our facing direction
Cube faces for a smaller 2x2 map (we have 50x50):
  AABB
  AABB
  CC
  CC
EEDD
FF
*/
char get_nxt_cube_face(const State22 &cur_state)
{
    switch (cur_state.cube_face)
    {
    case 'A':
        if (cur_state.facing == MFacing::Left) return 'E'; 
        if (cur_state.facing == MFacing::Up) return 'F'; 
        else throw std::runtime_error("A: Normal transition, no cube wrapping!"); 
        break;
    case 'B':
        if (cur_state.facing == MFacing::Up) return 'F'; 
        if (cur_state.facing == MFacing::Right) return 'D'; 
        if (cur_state.facing == MFacing::Down) return 'C'; 
        else throw std::runtime_error("B: Normal transition, no cube wrapping!"); 
        break;
    case 'C':
        if (cur_state.facing == MFacing::Left) return 'E'; 
        if (cur_state.facing == MFacing::Right) return 'B'; 
        else throw std::runtime_error("C: Normal transition, no cube wrapping!"); 
        break;
    case 'D':
        if (cur_state.facing == MFacing::Right) return 'B'; 
        if (cur_state.facing == MFacing::Down) return 'F'; 
        else throw std::runtime_error("A: Normal transition, no cube wrapping!"); 
        break;
    case 'E':
        if (cur_state.facing == MFacing::Up) return 'C'; 
        if (cur_state.facing == MFacing::Left) return 'A'; 
        else throw std::runtime_error("B: Normal transition, no cube wrapping!"); 
        break;
    case 'F':
        if (cur_state.facing == MFacing::Left) return 'A'; 
        if (cur_state.facing == MFacing::Right) return 'D'; 
        if (cur_state.facing == MFacing::Down) return 'B'; 
        else throw std::runtime_error("B: Normal transition, no cube wrapping!"); 
        break;
    default:
        throw std::runtime_error("Unknown cube facing!");
        break;
    }
}

/*
Cube faces for a smaller 2x2 map (we have 50x50):
  AABB
  AABB
  CC
  CC
EEDD
FF
The original map is 50x50 although i appended NO_MAP tiles at the upper, lower, left and right
end so we have no issues with out of bounds errors -> 
*/
char get_cube_face(const State22 &cur_state)
{
    if (cur_state.row < 51 && cur_state.col < 101) return 'A';
    if (cur_state.row < 51 && cur_state.col >= 101) return 'B';
    if (cur_state.row >= 51 && cur_state.row < 101) return 'C';
    if (cur_state.row >= 101 && cur_state.row < 151 && cur_state.col >= 51) return 'D';
    if (cur_state.row >= 101 && cur_state.row < 151 && cur_state.col < 51) return 'E';
    if (cur_state.row >= 151) return 'F';
    throw std::runtime_error("Invalid Position! Row: " + std::to_string(cur_state.row) + ", Col: " + std::to_string(cur_state.col));
}

bool do_wrap_around(const MMap &monkey_map, State22 &cur_state)
{
    uint32_t col{ cur_state.col };
    uint32_t row{ cur_state.row };

    switch (cur_state.facing)
    {
    case MFacing::Left: /*Move position to the right end and move inwards until a tile != NO_MAP is hit*/
        col = monkey_map.at(row).size() - 1;
        while(monkey_map.at(row).at(col) == NO_MAP)
        {
            --col;
        }
        break;
    case MFacing::Right: /*Move position to the left end and move inwards until a tile != NO_MAP is hit*/
        col = 0;
        while(monkey_map.at(row).at(col) == NO_MAP)
        {
            ++col;
        }
        break;
    case MFacing::Up: /*Move position to the bottom end and move upwards until a tile != NO_MAP is hit*/
        row = monkey_map.size() - 1;
        while(monkey_map.at(row).at(col) == NO_MAP)
        {
            --row;
        }
        break;
    case MFacing::Down: /*Move position to the top end and move downwards until a tile != NO_MAP is hit*/
        row = 0;
        while(monkey_map.at(row).at(col) == NO_MAP)
        {
            ++row;
        }
        break;
    }

    if (monkey_map.at(row).at(col) == FREE)
    {
        cur_state.row = row;
        cur_state.col = col;
        return true;
    }
    else
    {
        return false;
    }
}

// hardcoded transformations for my! input map
CubeTrafos get_transformations()
{
    CubeTrafos t_map;
    
    t_map[{ 'B', 'C' }] = Transformation{ {  1,-50 }, {  0, 100 }, 1 };
    t_map[{ 'C', 'B' }] = Transformation{ {  0, 50 }, {  1,  50 }, 3 };
    t_map[{ 'B', 'D' }] = Transformation{ { -1,151 }, {  0, 100 }, 2 };
    t_map[{ 'D', 'B' }] = Transformation{ { -1,151 }, {  0, 150 }, 2 };
    t_map[{ 'B', 'F' }] = Transformation{ {  0,200 }, {  1,-100 }, 0 };
    t_map[{ 'F', 'B' }] = Transformation{ {  0,  1 }, {  1, 100 }, 0 };
    t_map[{ 'E', 'C' }] = Transformation{ {  1, 50 }, {  0,  51 }, 1 };
    t_map[{ 'C', 'E' }] = Transformation{ {  0,101 }, {  1, -50 }, 3 };
    t_map[{ 'E', 'A' }] = Transformation{ { -1,151 }, {  0,  51 }, 2 };
    t_map[{ 'A', 'E' }] = Transformation{ { -1,151 }, {  0,   1 }, 2 };
    t_map[{ 'F', 'A' }] = Transformation{ {  0,  1 }, {  1,-100 }, 3 };
    t_map[{ 'A', 'F' }] = Transformation{ {  1,100 }, {  0,   1 }, 1 };
    t_map[{ 'D', 'F' }] = Transformation{ {  1,100 }, {  0,  50 }, 1 };
    t_map[{ 'F', 'D' }] = Transformation{ {  0,150 }, {  1,-100 }, 3 };

    return t_map;
}

State22 execute_instructions(const MMap &monkey_map, const std::vector<Instruction> ins_vec, const State22 init_state, bool do_cube_wrapping)
{
    State22 cur_state{ init_state };
    CubeTrafos cube_trafos = get_transformations();
    for (const auto &ins : ins_vec)
    {
        cur_state = do_single_instruction(monkey_map, ins, cur_state, cube_trafos, do_cube_wrapping);
    }
    return cur_state;
}

State22 get_init_state(const MMap &monkey_map)
{
    uint32_t pos{ 0u };
    uint32_t row{ 0u };
    uint32_t col{ 0u };
    uint32_t row_len{ static_cast<uint32_t>(monkey_map.at(0).size()) };
    while (monkey_map.at(row).at(col) != FREE)
    {
        ++pos;
        row = pos / row_len;
        col = pos - row * row_len;
    }
    return State22{ row, col, MFacing::Right , 'A'};
}

std::vector<Instruction> parse_instructions(const std::string &ins_string)
{
    uint16_t num{ 0u };
    std::pair<size_t, uint16_t> nxt_num = parse_next_number_in_str(ins_string, 0, num);

    std::vector<Instruction> ins_vec;
    while (nxt_num.first != std::string::npos && (nxt_num.first + nxt_num.second) < ins_string.length())
    {
        Instruction ins;
        ins.steps = num;
        switch (ins_string.at(nxt_num.first + nxt_num.second))
        {
        case LEFT:
            ins.dir = MDir::Left;
            break;
        case RIGHT:
            ins.dir = MDir::Right;
            break;
        
        default:
        throw std::runtime_error("Unknown direction char found: '" + ins_string.at(nxt_num.first + nxt_num.second) + std::string("'!"));
            break;
        }   
        ins_vec.push_back(ins);
        nxt_num = parse_next_number_in_str(ins_string, nxt_num.first + nxt_num.second, num);
    }

    // If last element in string is a number without direction change, insert it here
    if ((nxt_num.first + nxt_num.second) != (ins_string.length()-1))
    {
        ins_vec.push_back(Instruction{ num, MDir::Empty });
    }
    return ins_vec;
}

// resize monkey map so each row as same len (append NO_MAP until row lengths match)
void resize_monkey_map(MMap &monkey_map, const size_t max_len)
{
    for (auto &row : monkey_map)
    {
        while (row.size() < max_len)
        {
            row.push_back(NO_MAP);
        }
    }
}

std::pair<MMap, std::vector<Instruction>> get_data_in(const std::string &file_path)
{
    MMap monkey_map;
    std::vector<Instruction> ins;
    size_t max_len{ 0u };
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    monkey_map.push_back({ NO_MAP }); // first row contains only NO_MAP so we do not have to check ranges later
    if (input_file.is_open())
    {
        std::string input_line;

        // read first half of input (monkey map)
        getline(input_file, input_line);
        while(input_line != "")
        {  
            if (input_line.length() > max_len)
            {
                max_len = input_line.length();
            }
            monkey_map.push_back(std::vector<char>(input_line.begin(), input_line.end()));
            // Insert tile end in front and at the end of each line so we do not need to check
            // the map limits later (left and right)
            monkey_map.back().insert(monkey_map.back().begin(),NO_MAP);
            monkey_map.back().push_back(NO_MAP);
            getline(input_file, input_line);
        }
        monkey_map.push_back({ NO_MAP }); // last row contains only NO_MAP so we do not have to check ranges later
        // resize map so it is rectangular
        resize_monkey_map(monkey_map, max_len);

        // read seond part of input (last line)
        getline(input_file, input_line);
        ins = parse_instructions(input_line);

        input_file.close();   //close the file object.
    }

    return { monkey_map, ins };
}