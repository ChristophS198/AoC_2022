#include <string>
#include <algorithm>
#include <ostream>
#include <array>

#include "../utility.h"

constexpr size_t D_SIZE{ 1000u };
constexpr char PROP_FIELD{ '?' }; // Sign for a field that an elf wants to move to in next step
constexpr char OCC_FIELD{ '#' }; // Sign for a field that is occupied by an elf
constexpr char FREE_FIELD{ '.' }; // Sign for a free field (not occupied by an elf or proposed by one)

using Field23 = std::vector<std::vector<char>>;

enum class Dir23 :uint8_t
{
    North, 
    South,
    West,
    East
};

struct Elf23
{
    size_t row;
    size_t col;
    size_t prop_row;
    size_t prop_col;
};

std::pair<Field23,std::vector<Elf23>> get_input_data(const std::string &file_path);
std::vector<Elf23*> poposition_step(Field23 &field, std::vector<Elf23> &elves, uint32_t round);
uint32_t move_step(Field23 &field, std::vector<Elf23*> &moving_elf_candidates);
size_t num_empty_tiles(const Field23 &field, const std::vector<Elf23> &elves);
void print_field(std::ostream &out, const Field23 &field);
bool neighbor_check(Field23 &field, Elf23 &elf, uint32_t round);

int32_t day_23_1(const std::string &file_path)
{
    std::pair<Field23,std::vector<Elf23>> data_in = get_input_data(file_path);
    auto &field = data_in.first;
    auto &elves = data_in.second;

    for (size_t i=0; i<10; ++i)
    {
        std::vector<Elf23*> moving_candidates = poposition_step(field, elves, i);
        move_step(field, moving_candidates);
    }
    // std::ofstream out("23/field.txt");
    // print_field(out, data_in.first);
    // out.close();

    return num_empty_tiles(field, elves);
;
}

int32_t day_23_2(const std::string &file_path)
{
    std::pair<Field23,std::vector<Elf23>> data_in = get_input_data(file_path);
    auto &field = data_in.first;
    auto &elves = data_in.second;
    uint32_t moves{ 1u };
    uint32_t round{ 0u };
    while (moves > 0)
    {
        std::vector<Elf23*> moving_candidates = poposition_step(field, elves, round);
        moves = move_step(field, moving_candidates);
        ++round;
    }
    // std::ofstream out("23/field.txt");
    // print_field(out, data_in.first);
    // out.close();

    return round;
}

uint32_t move_step(Field23 &field, std::vector<Elf23*> &moving_elf_candidates)
{
    uint32_t move_ctr{ 0u };
    for (auto &elf : moving_elf_candidates)
    {
        auto &prop_field = field.at(elf->prop_row).at(elf->prop_col);
        switch (prop_field)
        {
        case PROP_FIELD: /*If the field is still marked by only one elf -> move to this field*/
            prop_field = OCC_FIELD;
            field.at(elf->row).at(elf->col) = FREE_FIELD;
            elf->row = elf->prop_row;
            elf->col = elf->prop_col;
            ++move_ctr;
            break;
        case FREE_FIELD: /*Some other elf reset the field to FREE_FIELD so we do not move*/
            elf->prop_col = elf->col;
            elf->prop_row = elf->row;
            break;
        default:
            throw std::runtime_error("Unknown field type: " + prop_field);
        }

    }
    return move_ctr;
}

std::vector<Elf23*> poposition_step(Field23 &field, std::vector<Elf23> &elves, uint32_t round)
{
    std::vector<Elf23*> moving_elf_candidates; // a vector of possible moving elves
    for (auto &elf : elves)
    {
        if (neighbor_check(field, elf, round))
        {
            moving_elf_candidates.push_back(&elf);
        }
    }
    return moving_elf_candidates;
}

// return true if this elf proposes a field that is still free otherwise false
bool neighbor_check(Field23 &field, Elf23 &elf, uint32_t round)
{
    std::array<bool, 4> free_dir{ 1,1,1,1 };
    if (field.at(elf.row-1).at(elf.col) == OCC_FIELD) free_dir[static_cast<size_t>(Dir23::North)] = false;
    if (field.at(elf.row+1).at(elf.col) == OCC_FIELD) free_dir[static_cast<size_t>(Dir23::South)] = false;
    if (field.at(elf.row).at(elf.col-1) == OCC_FIELD) free_dir[static_cast<size_t>(Dir23::West)] = false;
    if (field.at(elf.row).at(elf.col+1) == OCC_FIELD) free_dir[static_cast<size_t>(Dir23::East)] = false;

    if (field.at(elf.row-1).at(elf.col-1) == OCC_FIELD) 
    {
        free_dir[static_cast<size_t>(Dir23::North)] = false;
        free_dir[static_cast<size_t>(Dir23::West)] = false;
    }
    if (field.at(elf.row-1).at(elf.col+1) == OCC_FIELD) 
    {
        free_dir[static_cast<size_t>(Dir23::North)] = false;
        free_dir[static_cast<size_t>(Dir23::East)] = false;
    }
    if (field.at(elf.row+1).at(elf.col-1) == OCC_FIELD) 
    {
        free_dir[static_cast<size_t>(Dir23::South)] = false;
        free_dir[static_cast<size_t>(Dir23::West)] = false;
    }
    if (field.at(elf.row+1).at(elf.col+1) == OCC_FIELD) 
    {
        free_dir[static_cast<size_t>(Dir23::South)] = false;
        free_dir[static_cast<size_t>(Dir23::East)] = false;
    }

    elf.prop_col = elf.col;
    elf.prop_row = elf.row;
    // if no elf is found or all directions are already occupied we do nothing
    if (free_dir == std::array<bool,4>{ 1,1,1,1 } || free_dir == std::array<bool,4>{ 0,0,0,0 })
    {
        return false;
    }

    // Each elf sets a new marker PROP_FIELD to the field he proposes 
    // If this field is already marked with PROP_FIELD the marking is removed so no elf does
    // go to this location
    round = round % 4;
    for (uint32_t i=round; i<round+4; ++i)
    {
        if (i%4 == 0 && true == free_dir[static_cast<size_t>(Dir23::North)])
        {
            --elf.prop_row;
            break;
        }
        if (i%4 == 1 && true == free_dir[static_cast<size_t>(Dir23::South)])
        {
            ++elf.prop_row;
            break;
        }
        if (i%4 == 2 && true == free_dir[static_cast<size_t>(Dir23::West)])
        {
            --elf.prop_col;
            break;
        }
        if (i%4 == 3 && true == free_dir[static_cast<size_t>(Dir23::East)])
        {
            ++elf.prop_col;
            break;
        }
    }
    switch (field.at(elf.prop_row).at(elf.prop_col))
    {
    case OCC_FIELD: /*Already occupied -> should never happen*/
        throw std::runtime_error("There already sits an elf, although we checked before that there is free");
        break;
    case PROP_FIELD: /*Another elf wants to move to this field -> neither elf moves and we reset to FREE_FIELD*/
        field.at(elf.prop_row).at(elf.prop_col) = FREE_FIELD;
        elf.prop_col = elf.col;
        elf.prop_row = elf.row;
        return false;
        break;
    case FREE_FIELD: /*This field is still free, so we mark it as proposed*/
        field.at(elf.prop_row).at(elf.prop_col) = PROP_FIELD;
        break;
    default:
        throw std::runtime_error("Unknown field type found: " + field.at(elf.prop_row).at(elf.prop_col));
        break;
    }

    return true;
}

size_t num_empty_tiles(const Field23 &field, const std::vector<Elf23> &elves)
{
    size_t min_col{ field.at(0).size() };
    size_t max_col{ 0u };
    size_t min_row{ field.size() };
    size_t max_row{ 0u };

    for (const auto &elf : elves)
    {
        if (elf.row < min_row) min_row = elf.row; 
        if (elf.row > max_row) max_row = elf.row; 
        if (elf.col < min_col) min_col = elf.col; 
        if (elf.col > max_col) max_col = elf.col; 
    }

    size_t num_tiles = (max_row - min_row + 1) * (max_col - min_col + 1);
    return num_tiles - elves.size();
}

std::pair<Field23,std::vector<Elf23>> get_input_data(const std::string &file_path)
{
    Field23 field(D_SIZE, std::vector<char>(D_SIZE, '.'));
    std::vector<Elf23> elves;

    Field23 small_field = read_2d_vec_from_file<char>(file_path);
    size_t row{ (D_SIZE-small_field.size())/2 };
    for (size_t i=0; i<small_field.size(); ++i)
    {
        std::copy_n(&small_field[i][0], small_field.at(i).size(), &field[row+i][row]);
        for (size_t j=0; j<small_field.at(i).size(); ++j)
        {
            if (small_field.at(i).at(j) == OCC_FIELD)
            {
                elves.push_back(Elf23{ row+i,row+j,row+i,row+j });
            }
        }
    } 
    return { field, elves };
}

void print_field(std::ostream &out, const Field23 &field)
{
    for (const auto &row : field)
    {
        for (const auto &elem : row)
        {
            out << elem;
        }
        out << "\n";
    }
}