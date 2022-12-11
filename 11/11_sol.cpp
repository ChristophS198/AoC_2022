#include <string>
#include <numeric>

#include "monkey.h"
#include "../utility.h"


void play_round(std::vector<Monkey> &monkeys, std::vector<std::uint64_t> &insp_count, bool do_relief=true,  bool do_trick = false, std::uint64_t factor = 1u);

// debug
void print_monkeys(std::ostream &out, const std::vector<Monkey> &monkeys);

std::uint64_t day_11_1(const std::string &file_path)
{
    // set relief after inspection + number of rounds
    WorryOperation relief{ AdaptionType::Division, 3u };
    const size_t round_num{ 20u };

    std::vector<std::string> input_data = read_string_vec_from_file(file_path);
    std::vector<Monkey> monkeys;
    for (size_t i=0u; i<input_data.size()-5; i+=7)
    {
        monkeys.push_back(Monkey::create_monkey(input_data, i, relief));
    }

    // play game
    std::vector<std::uint64_t> insp_count(monkeys.size(), 0u);
    for (size_t i=0u; i<round_num; ++i)
    {
        play_round(monkeys, insp_count);
    }

    std::sort(insp_count.begin(), insp_count.end(), std::greater<decltype(insp_count)::value_type>());

    return insp_count.at(0) * insp_count.at(1);
}

std::uint64_t day_11_2(const std::string &file_path)
{
    // set relief after inspection + number of rounds
    WorryOperation relief{ AdaptionType::Division, 1u };
    const size_t round_num{ 10'000u };

    std::vector<std::string> input_data = read_string_vec_from_file(file_path);
    std::vector<Monkey> monkeys;
    std::uint64_t factor{ 1u };
    for (size_t i=0u; i<input_data.size()-5; i+=7)
    {
        monkeys.push_back(Monkey::create_monkey(input_data, i, relief));
        factor *= monkeys.back().get_test_val();
    }



    // play game
    std::vector<std::uint64_t> insp_count(monkeys.size(), 0u);
    for (size_t i=0u; i<round_num; ++i)
    {
        play_round(monkeys, insp_count, false, true, factor);
    }

    std::sort(insp_count.begin(), insp_count.end(), std::greater<decltype(insp_count)::value_type>());
    return insp_count.at(0) * insp_count.at(1);
}

/**
 * @brief plays a round and updates the number of inspections of each monkey as well as the internals of each monkey
 * 
 * @param monkeys 
 * @return std::vector<std::uint64_t> returns a vector of numbers representing the item inspection count of each monkey (index == id)
 */
void play_round(std::vector<Monkey> &monkeys, std::vector<std::uint64_t> &insp_count, bool do_relief, bool do_trick, std::uint64_t factor)
{
    for (auto &monkey : monkeys)
    {
        insp_count.at(monkey.get_id()) += static_cast<std::uint64_t>(monkey.get_item_len());
        std::vector<Monkey::Throw> throws = monkey.throw_items(do_relief, do_trick, factor);
        // add items to receivers
        for (const auto &s_throw : throws)
        {
            monkeys.at(s_throw.receiver).add_item(s_throw.worry_level);
        }
    }
}

void print_monkeys(std::ostream &out, const std::vector<Monkey> &monkeys)
{
    out << "\n";
    for (const auto &monkey : monkeys)
    {
        monkey.print_stats(out);
        out << "\n";
    }

}
