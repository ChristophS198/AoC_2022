#include <string>
#include <vector>
#include <ostream>

#include "../utility.h"

constexpr int64_t DECRYPTION_KEY{ 811'589'153 };

struct Number
{
    int64_t val;
    int64_t pos;
};

void execute_mixing(std::vector<Number> &nums);
std::vector<Number> get_numbers(const std::string &file_path);
std::vector<Number> re_order_vec(const std::vector<Number> &nums);
void apply_decryption_key(std::vector<Number> &nums, int64_t key);
size_t get_0_pos(const std::vector<Number> &nums);
void print_nums(std::ostream &out, const std::vector<Number> &nums, bool with_pos=false);
int64_t mod(int64_t k, int64_t n);

int64_t day_20_1(const std::string &file_path)
{
    std::vector<Number> mix_nums = get_numbers(file_path);
    execute_mixing(mix_nums);
    std::vector<Number> ordered_nums = re_order_vec(mix_nums);

    size_t pos_0 = get_0_pos(ordered_nums);
    int64_t val_1{ ordered_nums.at((pos_0 + 1000)%ordered_nums.size()).val };
    int64_t val_2{ ordered_nums.at((pos_0 + 2000)%ordered_nums.size()).val };
    int64_t val_3{ ordered_nums.at((pos_0 + 3000)%ordered_nums.size()).val };

    return val_1 + val_2 + val_3;
}

int64_t day_20_2(const std::string &file_path)
{
    std::vector<Number> mix_nums = get_numbers(file_path);
    apply_decryption_key(mix_nums, DECRYPTION_KEY);
    for (size_t i=0; i<10; ++i)
    {
        execute_mixing(mix_nums);
    }
    std::vector<Number> ordered_nums = re_order_vec(mix_nums);
    // std::ofstream out_file("20/new_order.txt");
    // print_nums(out_file, ordered_nums, false);
    // out_file.close();

    size_t pos_0 = get_0_pos(ordered_nums);
    int64_t val_1{ ordered_nums.at((pos_0 + 1000)%ordered_nums.size()).val };
    int64_t val_2{ ordered_nums.at((pos_0 + 2000)%ordered_nums.size()).val };
    int64_t val_3{ ordered_nums.at((pos_0 + 3000)%ordered_nums.size()).val };

    return val_1 + val_2 + val_3;
}

void apply_decryption_key(std::vector<Number> &nums, int64_t key)
{
    for (auto &elem : nums)
    {
        elem.val *= key;
    }
}

void execute_mixing(std::vector<Number> &nums)
{
    for (auto &mix_num : nums)
    {
        if (mix_num.val == 0) continue;

        auto new_pos = mod(mix_num.pos + mix_num.val, nums.size()-1);
        if (new_pos == 0) new_pos = nums.size() - 1;

        for (auto &elem : nums)
        {
            if (mix_num.pos < elem.pos && new_pos >= elem.pos)
            {
                --elem.pos;
            }
            else
            {
                if (mix_num.pos > elem.pos && new_pos <= elem.pos)
                {
                    ++elem.pos;
                }
            }
        }
        mix_num.pos = new_pos;
    }
}

size_t get_0_pos(const std::vector<Number> &nums)
{
    for (const auto &elem : nums)
    {
        if (elem.val == 0)
        {
            return static_cast<size_t>(elem.pos);
        }
    }
    throw std::runtime_error("Element 0 not found!");
}

int64_t mod(int64_t k, int64_t n)
{
    return ((k %= n) < 0) ? k+n : k;
}

void print_nums(std::ostream &out, const std::vector<Number> &nums, bool with_pos)
{
    for (const auto &elem : nums)
    {
        if (with_pos)
        {
            out << elem.val << "\t" << elem.pos << "\n";
        }
        else
        {
            out << elem.val << ", ";
        }
    }
}

std::vector<Number> re_order_vec(const std::vector<Number> &nums)
{
    std::vector<Number> ordered_nums(nums.size());
    for (const auto &elem : nums)
    {
        ordered_nums[elem.pos] = elem;
    }
    return ordered_nums;
}

std::vector<Number> get_numbers(const std::string &file_path)
{
    std::vector<Number> number_vec{};
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    size_t idx{ 0u };
    if (input_file.is_open())
    {
        std::string input_line;
        while(getline(input_file, input_line))
        { 
            int64_t num_in_line = parse_string_to_number<int64_t>(input_line);
            number_vec.push_back({ num_in_line, static_cast<int64_t>(idx++) });
        }
        input_file.close();   
    }
    return number_vec;    
}
