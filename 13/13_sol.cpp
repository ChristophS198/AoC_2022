#include <string>
#include <algorithm>
#include <numeric>

#include "../utility.h"

const std::string Divider1 = "[[2]]";
const std::string Divider2 = "[[6]]";

std::vector<std::pair<std::string,std::string>> read_string_pair_vec_from_file(const std::string& file_path);
bool is_order_correct(const std::string &p1, const std::string &p2, size_t pos1=0, size_t pos2=0);
int get_num(const std::string &p, size_t &pos);
std::vector<std::string> read_string_vec_from_file_no_empty_lines(const std::string& file_path);
bool custom_sort(const std::string &s1, const std::string &s2)
{
    return is_order_correct(s1, s2);
}

std::uint32_t day_13_1(const std::string &file_path)
{
    std::vector<std::pair<std::string,std::string>> packet_pairs = read_string_pair_vec_from_file(file_path);
    std::vector<size_t> correct_pair_ind;
    for (size_t i=0; i<packet_pairs.size(); ++i)
    {
        if (is_order_correct(packet_pairs.at(i).first, packet_pairs.at(i).second, 0, 0))
        {
            correct_pair_ind.push_back(i+1);
        }
    }
    return std::accumulate(correct_pair_ind.begin(), correct_pair_ind.end(), decltype(correct_pair_ind)::value_type(0));
}

std::uint32_t day_13_2(const std::string &file_path)
{
    std::vector<std::string> packets = read_string_vec_from_file_no_empty_lines(file_path);
    packets.push_back(Divider1);
    packets.push_back(Divider2);

    std::sort(packets.begin(), packets.end(),custom_sort);
  
    int index1{ 0 };
    int index2{ 0 };
    auto it = std::find(packets.begin(), packets.end(), Divider1);
    index1 = it - packets.begin() + 1;

    it = std::find(packets.begin(), packets.end(), Divider2);
    index2 = it - packets.begin() + 1;


    return index1 * index2;
}

std::vector<std::string> read_string_vec_from_file_no_empty_lines(const std::string& file_path)
{
    std::vector<std::string> string_vec{};
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open()){
        std::string input_line;
        while(getline(input_file, input_line)){  //read data from file object and put it into string.
            if (input_line.length() > 0)
            {
                string_vec.push_back(input_line);
            }
        }
        input_file.close();   //close the file object.
    }
    return string_vec;
}

int get_num(const std::string &p, size_t &pos)
{
    if (pos > p.length()- 2)
    {
        throw std::invalid_argument("Position in string too large!");
    }
    if (std::isdigit(static_cast<unsigned char>(p.at(pos+1))))
    {
        ++pos;
        return static_cast<int>(p.at(pos-1)-'0') * 10 + static_cast<int>(p.at(pos)-'0');
    }
    else
    {
        return static_cast<int>(p.at(pos)-'0');
    }
}


bool is_order_correct(const std::string &p1, const std::string &p2, size_t pos1, size_t pos2)
{

    while(true)
    {
        const auto c1 = p1.at(pos1);
        const auto c2 = p2.at(pos2);

        if (c1 == c2)
        {
            if ( std::isdigit(static_cast<unsigned char>(c1)) )
            {
                int v1 = get_num(p1, pos1); // conversion necessary for some numbers contain two digits ('10')
                int v2 = get_num(p2, pos2);
                if (v1 == v2)
                {
                    ++pos1;
                    ++pos2;
                    continue;
                }
                else
                {
                    return v1 < v2;
                }
            }
            else
            {
                ++pos1;
                ++pos2;
                continue;
            }
        }
        if (c1 == ']') // list of packet 1 ends first (c2 cannot be ']' as well)
        {
            return true;
        }
        if (c2 == ']') // list of packet 2 ends first
        {
            return false;
        }
        if (std::isdigit(static_cast<unsigned char>(c1)) && std::isdigit(static_cast<unsigned char>(c2)))
        {
            int v1 = get_num(p1, pos1); // conversion necessary for some numbers contain two digits ('10')
            int v2 = get_num(p2, pos2);
            return v1 < v2; // equality is not possible due to first if clause
        }
        if (std::isdigit(static_cast<unsigned char>(c1)))
        {
            size_t len { 1u };
            const char c11 = p1.at(pos1+1);
            std::string new_str = p1.substr(0, pos1) + "[" + c1;
            if (std::isdigit(static_cast<unsigned char>(c11)))
            {
                len = 2u;
                new_str = new_str + c11;
            }
            new_str = new_str + "]" + p1.substr(pos1+len);
            return is_order_correct(new_str, p2, ++pos1, ++pos2);
        }
        if (std::isdigit(static_cast<unsigned char>(c2)))
        {
            size_t len { 1u };
            const char c21 = p2.at(pos2+1);
            std::string new_str = p2.substr(0, pos2) + "[" + c2;
            if (std::isdigit(static_cast<unsigned char>(c21)))
            {
                len = 2u;
                new_str = new_str + c21;
            }
            new_str = new_str + "]" + p2.substr(pos2+len);
            return is_order_correct(p1, new_str, ++pos1, ++pos2);
        }
    }
    throw std::runtime_error("Should not reach end of function");
    return true;
}

std::vector<std::pair<std::string,std::string>> read_string_pair_vec_from_file(const std::string& file_path)
{
    std::vector<std::pair<std::string,std::string>> string_vec{};
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open()){
        std::string input_line;
        std::string input_line_2;
        while(getline(input_file, input_line))
        {  
            getline(input_file, input_line_2);
            string_vec.push_back({ input_line, input_line_2 });
            getline(input_file, input_line_2);
        }
        input_file.close();   //close the file object.
    }
    return string_vec;
}