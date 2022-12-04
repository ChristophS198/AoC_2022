#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "../utility.h"

using Calories = std::uint32_t;

struct Elf
{
    std::vector<Calories> calories_vec{};
    Calories sum_calories{ 0 };
};

// function declarations
std::vector<Elf> get_elves_from_file(const std::string &file_path);
Calories get_max_calory_vals(std::vector<Elf> &elves, const std::uint16_t num=1u);

Calories day_1_1(const std::string &file_path)
{
    std::vector<Elf> elves = get_elves_from_file(file_path);
    return get_max_calory_vals(elves,1u);
}

Calories day_1_2(const std::string &file_path)
{
    std::vector<Elf> elves = get_elves_from_file(file_path);
    return get_max_calory_vals(elves,3u);
}

Calories get_max_calory_vals(std::vector<Elf> &elves, const std::uint16_t num)
{
    Calories max_vals{ 0 };
    std::sort(elves.begin(), elves.end(), [](const Elf &a, const Elf &b)
    {
        return a.sum_calories > b.sum_calories;
    });

    for (size_t i=0; i<num; ++i)
    {
        max_vals += elves[i].sum_calories;
    }
    return max_vals;
}

std::vector<Elf> get_elves_from_file(const std::string &file_path)
{
    std::vector<Elf> elves_vec{};
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    std::vector<Calories> tmp{};
    Calories sum{ 0u };
    if (input_file.is_open()){
        std::string input_line;

        while(getline(input_file, input_line)){  
            if (input_line == "")
            {
                if (!tmp.empty())
                {
                    elves_vec.push_back(Elf{tmp, sum});
                }
                tmp.clear();
                sum = 0u;
                continue;
            }

            Calories calory = parse_string_to_number<Calories>(input_line);
            tmp.push_back(calory);
            sum += calory;
        }
        input_file.close();   //close the file object.
    }
    return elves_vec;
}
