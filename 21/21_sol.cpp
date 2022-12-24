#include <string>
#include <vector>
#include <map>
#include <list>
#include <limits>

#include "../utility.h"

constexpr size_t OP_START_1{ 6u }; // char position of the first monkey name in open_ops monkey
constexpr size_t OP_START_2{ 13u }; // char position of the first monkey name in open_ops monkey
constexpr size_t OP_POS{ 11u }; // char position of the operation sign
constexpr size_t MONKEY_NAME_LEN{ 4u }; // Length of monkey name
const std::string ROOT_NAME{ "root" }; // Name of root monkey
const std::string MY_NAME{ "humn" }; // Name of myself

enum class MOperation : uint8_t
{
    Add,
    Sub,
    Mult,
    Div,
};

struct OpMonkey
{
    std::string name;
    std::string first_op;
    std::string second_op;
    MOperation operation;
};

struct LastOp
{
    int64_t first_val;
    int64_t second_val;
    MOperation operation;
};

std::pair<std::list<OpMonkey>, std::map<std::string, int64_t>> get_input(const std::string &file_path);
LastOp do_monkey_calc(std::list<OpMonkey> open_ops, std::map<std::string, int64_t> numbers);
int64_t exec_operation(const int64_t op_1, const int64_t op_2, const MOperation operation);

int64_t day_21_1(const std::string &file_path)
{
    std::pair<std::list<OpMonkey>, std::map<std::string, int64_t>> data = get_input(file_path);
    LastOp root_op = do_monkey_calc(data.first, data.second);

    return exec_operation(root_op.first_val, root_op.second_val, root_op.operation);
}

int64_t day_21_2(const std::string &file_path)
{
    std::pair<std::list<OpMonkey>, std::map<std::string, int64_t>> data = get_input(file_path);
    LastOp root_op;
    int64_t lower_bound{ 0 };
    int64_t upper_bound{ 10 };
    int64_t factor{ 2 };

    // First step is to find an approximate lower/upper bound where the root lies in between
    data.second[MY_NAME] = lower_bound;
    LastOp root_op_1 = do_monkey_calc(data.first, data.second);
    data.second[MY_NAME] = upper_bound;
    LastOp root_op_2 = do_monkey_calc(data.first, data.second);
    auto diff_lower = (root_op_1.first_val - root_op_1.second_val);
    auto diff_upper = (root_op_2.first_val - root_op_2.second_val);

    while ((diff_lower < 0 && diff_upper < 0) || (diff_lower > 0 && diff_upper > 0))
    {
        upper_bound *= factor;
        data.second[MY_NAME] = upper_bound;
        root_op_2 = do_monkey_calc(data.first, data.second);
        std::cout << data.second[MY_NAME] << " - " << root_op_2.first_val - root_op_2.second_val << std::endl;
        diff_upper = (root_op_2.first_val - root_op_2.second_val);
    }
    lower_bound = upper_bound / factor;

    // Now we have a good approximation of where the root must be
    // Under the assumption that we linearly increase/decrease the diff based on the value in 'humn'
    // we can iteratively narrow the upper/lower bounds so the diff at lower bound always has a 
    // different sign than the diff at upper bound 
    while (true)
    {
        auto middle_val = (upper_bound + lower_bound) / 2; // choose value between lower and upper bound
        data.second[MY_NAME] = middle_val;
        root_op = do_monkey_calc(data.first, data.second);
        std::cout << data.second[MY_NAME] << " - " << root_op.first_val - root_op.second_val << std::endl;
        auto diff_middle = (root_op.first_val - root_op.second_val);
        if (diff_middle == 0)
        {
            return data.second.at(MY_NAME);
        }
        // if sign does not change when compared to lower bound -> use middle point as new lower bound 
        if ((diff_middle < 0 && diff_lower < 0) || (diff_middle > 0 && diff_lower > 0))
        {
            lower_bound = middle_val;
            diff_lower = diff_middle;
        }
        else // if root lies between lower bound and middle point -> use middle point as new upper bound
        {
            upper_bound = middle_val;
            diff_upper = diff_middle;
        }
    }

}

LastOp do_monkey_calc(std::list<OpMonkey> open_ops, std::map<std::string, int64_t> numbers)
{
    uint64_t done_calcs{ 0u };
    while (true)
    {

        for (auto it=open_ops.begin(); it!=open_ops.end(); )
        {
            auto it_first = numbers.find(it->first_op);
            auto it_second = numbers.find(it->second_op);
            if (it_first != numbers.end() && it_second != numbers.end())
            {
                auto result = exec_operation(it_first->second, it_second->second, it->operation);
                if (numbers.insert({it->name, result}).second == false)
                {
                    throw std::runtime_error("Trying to insert same monkey name twice: " + it->name);
                }
                // value of root monkey calculated -> stop calcs 
                if (it->name == ROOT_NAME)
                {
                    return LastOp{ it_first->second, it_second->second, it->operation};
                }
                // remove this operation from vector of open open_ops
                it = open_ops.erase(it);
                ++done_calcs;
            }
            else
            {
                ++it;
            }
        }

        if (done_calcs == 0)
        {
            throw std::runtime_error("No operations could be executed! Something is off!");
        }
    }
}

int64_t exec_operation(const int64_t op_1, const int64_t op_2, const MOperation operation)
{
    switch (operation)
    {
    case MOperation::Add:
        return op_1 + op_2;
        break;
    case MOperation::Sub:
        return op_1 - op_2;
        break;
    case MOperation::Div:
        return op_1 / op_2;
        break;
    case MOperation::Mult:
        return op_1 * op_2;
        break;
    
    default:
        throw std::invalid_argument("Invalid operation!");
        break;
    }
}

std::pair<std::list<OpMonkey>, std::map<std::string, int64_t>> get_input(const std::string &file_path)
{
    std::list<OpMonkey> monkey_ops;
    std::map<std::string, int64_t> numbers;

    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open()){
        std::string input_line;
        while(getline(input_file, input_line))
        {  //read data from file object and put it into string.
            std::string name = input_line.substr(0,MONKEY_NAME_LEN);
            std::vector<int64_t> num = parse_string_to_number_vec<int64_t>(input_line);
            if (num.size() > 0)
            {
                numbers[name] = num[0];
            }
            else
            {
                OpMonkey new_op;
                new_op.name = name;
                new_op.first_op = input_line.substr(OP_START_1,MONKEY_NAME_LEN);
                new_op.second_op = input_line.substr(OP_START_2,MONKEY_NAME_LEN);
                switch (input_line.substr(OP_POS,1).at(0))
                {
                case '+':
                    new_op.operation = MOperation::Add;
                    break;
                case '-':
                    new_op.operation = MOperation::Sub;
                    break;
                case '/':
                    new_op.operation = MOperation::Div;
                    break;
                case '*':
                    new_op.operation = MOperation::Mult;
                    break;
                
                default:
                    throw std::runtime_error("Invalid operation detected: " + input_line.substr(OP_POS,1).at(0));
                    break;
                }
                monkey_ops.push_back(new_op);
            }
        }
        input_file.close();   //close the file object.
    }
    return {monkey_ops, numbers};
}