#include <string>
#include <map>
#include <vector>
#include <numeric>

#include "../utility.h"

std::map<char, int64_t> snafu2dec{ {'2',2}, {'1',1}, {'0',0}, {'-',-1}, {'=',-2} };
std::map<int64_t,char> dec2snafu{ {2,'2'}, {1,'1'}, {0,'0'}, {4,'-'}, {3,'='} };

std::vector<int64_t> get_dec_numbers(const std::vector<std::string> &snafu_nums);
int64_t snafu_to_dec(const std::string &snafu);
std::string dec_to_snafu(const int64_t dec);

std::string day_25_1(const std::string &file_path)
{
    std::vector<std::string> data_in = read_string_vec_from_file(file_path);
    std::vector<int64_t> dec_nums = get_dec_numbers(data_in);
    int64_t sum = std::accumulate(dec_nums.begin(), dec_nums.end(), decltype(dec_nums)::value_type(0));
    std::string snafu = dec_to_snafu(sum);

    return snafu;
}

int64_t snafu_to_dec(const std::string &snafu)
{
    int64_t dec{ 0u };
    int64_t factor{ 1u };
    for (auto i=snafu.rbegin(); i!=snafu.rend(); ++i ) 
    {
        dec += snafu2dec.at(*i) * factor;
        factor *= 5;
    }
    return dec;
}

std::string dec_to_snafu(const int64_t dec)
{
    std::string reverse_snafu{};
    int64_t x_new{ dec };
    while(x_new > 0)
    {
        int64_t z = x_new % 5;
        reverse_snafu.push_back(dec2snafu.at(z));
        x_new = (x_new - snafu2dec.at(reverse_snafu.back())) / 5;
    }

    return std::string(reverse_snafu.rbegin(), reverse_snafu.rend());
}

std::vector<int64_t> get_dec_numbers(const std::vector<std::string> &snafu_nums)
{
    std::vector<int64_t> dec_nums;

    for (const auto &snafu : snafu_nums)
    {
        dec_nums.push_back(snafu_to_dec(snafu));
    }

    return dec_nums;
}