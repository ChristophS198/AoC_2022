#include <string>
#include <vector>
#include <map>

#include "../utility.h"

using CrateId = std::uint16_t;
using StackMap = std::map<CrateId, std::vector<char>>;

struct CrateOp
{
    std::uint16_t num_crates;
    CrateId source;
    CrateId target;
};

struct InputData
{
    StackMap stack_map;
    std::vector<CrateOp> crate_ops;
};

InputData read_input_data(const std::string &file_path);
StackMap parse_stack_data(const std::vector<std::string> &str_vec);
void insert_new_elements(StackMap &stack_data, const std::string &str);
void execute_operation_9000(StackMap &stack_data, const CrateOp &op);
void execute_operation_9001(StackMap &stack_data, const CrateOp &op);

std::string day_5_1(const std::string &file_path)
{
    InputData data_in = read_input_data(file_path);
    for (const auto &op : data_in.crate_ops)
    {
        execute_operation_9000(data_in.stack_map, op);
    }
    std::string top_elem;
    for (size_t i=1; i<=data_in.stack_map.size(); ++i)
    {
        top_elem.push_back(data_in.stack_map[i].back());
    }
    return top_elem;
}

std::string day_5_2(const std::string &file_path)
{
    InputData data_in = read_input_data(file_path);
    for (const auto &op : data_in.crate_ops)
    {
        execute_operation_9001(data_in.stack_map, op);
    }
    std::string top_elem;
    for (size_t i=1; i<=data_in.stack_map.size(); ++i)
    {
        top_elem.push_back(data_in.stack_map[i].back());
    }
    return top_elem;
}

void execute_operation_9001(StackMap &stack_data, const CrateOp &op)
{
    auto &source = stack_data[op.source];
    auto &target = stack_data[op.target];
    target.insert(target.end(), source.end()-op.num_crates, source.end());
    source.erase(source.end()-op.num_crates, source.end());
}

void execute_operation_9000(StackMap &stack_data, const CrateOp &op)
{
    auto &source = stack_data[op.source];
    auto &target = stack_data[op.target];
    for (size_t i=0; i<op.num_crates; ++i)
    {
        
        target.push_back(source.back());
        source.erase(source.end()-1);
    }
}

InputData read_input_data(const std::string &file_path)
{
    InputData data_in;
    std::vector<std::string> string_vec{};
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open()){
        std::string input_line;
        while (getline(input_file, input_line))
        {  
            if (input_line == "" && string_vec.size() > 0u)
            {
                data_in.stack_map = parse_stack_data(string_vec);
                string_vec.clear();
                break;
            }
            string_vec.push_back(input_line);
        }
        // read crate operations
        while (getline(input_file, input_line))
        {  
            std::vector<std::uint16_t> operands = parse_string_to_number_vec<std::uint16_t>(input_line);
            data_in.crate_ops.push_back(CrateOp{operands[0], operands[1], operands[2]});
        }
        input_file.close();   //close the file object.
    }
    return data_in;
}

StackMap parse_stack_data(const std::vector<std::string> &str_vec)
{

    StackMap stack_data;
    size_t i = str_vec.size()-2;
    while (true)
    {
        insert_new_elements(stack_data, str_vec[i]);
        if (i == 0u)
        {
            break;
        }
        --i;
    }
    return stack_data;
}

void insert_new_elements(StackMap &stack_data, const std::string &str)
{
    size_t stack_idx{ 1u };
    size_t pos{ 1u };
    while(pos < str.length())
    {
        if (str[pos] != ' ')
        {
            stack_data[stack_idx].push_back(str[pos]);
        }
        ++stack_idx;
        pos += 4;
    }
}