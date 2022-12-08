#include <string>
#include <vector>

#include "../utility.h"

using Trees = std::uint32_t;
using Height = std::int16_t;
using TreeField = std::vector<std::vector<Height>>;
using BitField = std::vector<std::vector<bool>>;
using ScenicScore = std::uint64_t;

constexpr Height MAX_HEIGHT = 9;

BitField get_visible_trees_left(const TreeField &trees);
BitField get_visible_trees_right(const TreeField &trees);
BitField get_visible_trees_up(const TreeField &trees);
BitField get_visible_trees_down(const TreeField &trees);
BitField combine_views(const BitField &left, const BitField &right, const BitField &up, const BitField &down, Trees &sum);

ScenicScore get_max_scenic_score(const TreeField &trees);
ScenicScore get_scenic_score(const TreeField &trees, size_t row, size_t col);

Trees day_8_1(const std::string &file_path)
{
    TreeField field = read_2d_vec_from_file<Height>(file_path);
    BitField left = get_visible_trees_left(field);
    BitField right = get_visible_trees_right(field);
    BitField up = get_visible_trees_up(field);
    BitField down = get_visible_trees_down(field);

    Trees sum{ 0u };
    BitField comb_view = combine_views(left, right, up, down, sum);

    return sum;
}

Trees day_8_2(const std::string &file_path)
{
    TreeField field = read_2d_vec_from_file<Height>(file_path);

    return get_max_scenic_score(field);    
}

ScenicScore get_scenic_score(const TreeField &trees, size_t row, size_t col)
{
    if (row == 0 || col == 0 || row == trees.size()-1 || col == trees.at(0).size()-1) 
    {
        return 0u;
    }
    const Height cur_height = trees.at(row).at(col);
    Trees left{ 0 }; // count visible trees left
    Trees right{ 0 }; // count visible trees left
    Trees up{ 0 }; // count visible trees left
    Trees down{ 0 }; // count visible trees left
    size_t r{ row };
    while (r>0)
    {
        ++up;
        --r;
        if (trees.at(r).at(col) >= cur_height) break;
    } 
    r = row;
    while (r<trees.size()-1)
    {
        ++down;
        ++r;
        if (trees.at(r).at(col) >= cur_height) break;
    } 

    size_t c{ col };
    while (c>0)
    {
        ++left;
        --c;
        if (trees.at(row).at(c) >= cur_height) break;
    } 
    c = col;
    while (c<trees.at(0).size()-1)
    {
        ++right;
        ++c;
        if (trees.at(row).at(c) >= cur_height) break;
    } 
    return left * right * up * down;
}

ScenicScore get_max_scenic_score(const TreeField &trees)
{
    ScenicScore max_score{ 0u };
    for (size_t row=0; row<trees.size(); ++row)
    {
        for (size_t col=0; col<trees.at(0).size(); ++col)
        {
            ScenicScore score = get_scenic_score(trees, row, col);
            if (max_score < score)
            {
                max_score = score;
            }
        }
    }
    return max_score;
}


BitField combine_views(const BitField &left, const BitField &right, const BitField &up, const BitField &down, Trees &sum)
{
    BitField comb_view(left.size(), std::vector<bool>(left.at(0).size(),false));
    sum = 0u;
    for (size_t row=0; row<left.size(); ++row)
    {
        for (size_t col=0; col<left.at(0).size(); ++col)
        {
            if (left.at(row).at(col) || right.at(row).at(col) || up.at(row).at(col) || down.at(row).at(col))
            {
                comb_view.at(row).at(col) = true;
                ++sum;
            }
        }
    }
    return comb_view;
}

BitField get_visible_trees_left(const TreeField &trees)
{
    BitField bit_field(trees.size(), std::vector<bool>(trees.at(0).size(),false));

    for (size_t row=0; row<trees.size(); ++row)
    {
        Height max_height{ -1 };
        for (size_t col=0; col<trees.at(0).size(); ++col)
        {
            if (trees[row][col] > max_height)
            {
                bit_field[row][col] = true;
                max_height = trees[row][col];
            }
            if (trees[row][col] == MAX_HEIGHT)
            {
                break;
            }
        }
    }
    return bit_field;
}

BitField get_visible_trees_right(const TreeField &trees)
{
    BitField bit_field(trees.size(), std::vector<bool>(trees.at(0).size(),false));

    for (int32_t row=trees.size()-1; row>=0; --row)
    {
        Height max_height{ -1 };
        for (int32_t col=trees.at(0).size()-1; col>=0; --col)
        {
            if (trees[row][col] > max_height)
            {
                bit_field[row][col] = true;
                max_height = trees[row][col];
            }
            if (trees[row][col] == MAX_HEIGHT)
            {
                break;
            }
        }
    }
    return bit_field;
}

BitField get_visible_trees_up(const TreeField &trees)
{
    BitField bit_field(trees.size(), std::vector<bool>(trees.at(0).size(),false));

    for (size_t col=0; col<trees.at(0).size(); ++col)
    {
        Height max_col_height{ -1 };
        for (size_t row=0; row<trees.size(); ++row)
        {
            if (trees[row][col] > max_col_height)
            {
                bit_field[row][col] = true;
                max_col_height = trees[row][col];
            }
            if (trees[row][col] == MAX_HEIGHT)
            {
                break;
            }
        }
    }
    return bit_field;
}

BitField get_visible_trees_down(const TreeField &trees)
{
    BitField bit_field(trees.size(), std::vector<bool>(trees.at(0).size(),false));

    for (int32_t col=trees.at(0).size()-1; col>=0; --col)
    {
        Height max_col_height{ -1 };
        for (int32_t row=trees.size()-1; row>=0; --row)
        {
            if (trees[row][col] > max_col_height)
            {
                bit_field[row][col] = true;
                max_col_height = trees[row][col];
            }
            if (trees[row][col] == MAX_HEIGHT)
            {
                break;
            }
        }
    }
    return bit_field;
}

