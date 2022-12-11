#include <vector>
#include <cstdint>
#include <string>
#include <map>

#include "../utility.h"

enum class AdaptionType : std::uint8_t
{
    Division, 
    Multiplication, 
    Addition, 
    Square
};

struct WorryOperation
{
    AdaptionType type;
    std::int64_t val;
};

WorryOperation get_adaption_from_op_str(const std::string &str);
WorryOperation get_adaption_from_test_str(const std::string &str);
std::vector<std::uint64_t> execute_op(const std::vector<std::uint64_t> &worry_level, const WorryOperation &op);
bool do_test(const std::uint64_t worry_level, const WorryOperation &op);

class Monkey
{
public:
    using Item = std::uint64_t;
    using MonkeyId = std::uint64_t;

    struct Throw
    {
        MonkeyId receiver;
        Item worry_level;
    };

    static Monkey create_monkey(const std::vector<std::string> &monkey_str, size_t pos, const WorryOperation relief);
    std::vector<Throw> throw_items(bool do_relief = false, bool do_trick = false, std::uint64_t factor = 1u);
    void add_item(Item worry_level);

    void print_stats(std::ostream &out) const;
    MonkeyId get_id() const { return id; } 
    std::uint64_t get_item_len() const { return items_worry_level.size(); }
    std::uint64_t get_test_val() const { return test.val; }

private:
    Monkey() = default;

    MonkeyId id;
    std::vector<Item> items_worry_level;
    WorryOperation inspection;
    WorryOperation test;
    std::map<bool, MonkeyId> throw_decision;
    WorryOperation relief_after_insp;
};

void Monkey::add_item(Item worry_level)
{
    items_worry_level.push_back(worry_level);
}

std::vector<Monkey::Throw> Monkey::throw_items(bool do_relief, bool do_trick, std::uint64_t factor) 
{
    std::vector<Throw> receivers;
    // do inspection
    items_worry_level = execute_op(items_worry_level, inspection);

    // relief after inspection
    if (do_relief)
    {
        items_worry_level = execute_op(items_worry_level, relief_after_insp);
    }

    // get receivers by executing test
    for (const auto &worry : items_worry_level)
    {
        bool decision = do_test(worry, test);
        Throw s_throw;
        s_throw.receiver = throw_decision[decision];
        s_throw.worry_level = worry;
        if (do_trick)
        {
            s_throw.worry_level = s_throw.worry_level % (factor); // Each test tests against a sub-factor of factor
        }
        receivers.push_back(s_throw);
    }
    // clear all items after throwing
    items_worry_level.clear();

    return receivers;
}

void Monkey::print_stats(std::ostream &out) const
{
    out << "Monkey " << id << ": ";
    for (const auto &item : items_worry_level)
    {
        out << item << ", " ;
    }
}

Monkey Monkey::create_monkey(const std::vector<std::string> &monkey_str, size_t pos, const WorryOperation relief)
{
    Monkey monkey;
    monkey.id = parse_string_to_number<MonkeyId>(monkey_str.at(pos));
    monkey.items_worry_level = parse_string_to_number_vec<Item>(monkey_str.at(pos+1));
    monkey.inspection = get_adaption_from_op_str(monkey_str.at(pos+2));
    monkey.test = get_adaption_from_test_str(monkey_str.at(pos+3));
    monkey.throw_decision[true] = parse_string_to_number<MonkeyId>(monkey_str.at(pos+4));
    monkey.throw_decision[false] = parse_string_to_number<MonkeyId>(monkey_str.at(pos+5));
    monkey.relief_after_insp = relief;

    return monkey;
}

bool do_test(const std::uint64_t worry_level, const WorryOperation &op)
{
    switch (op.type)
    {
    case AdaptionType::Division:
        return !(static_cast<std::uint64_t>(worry_level) % op.val);
        break;
    
    default:
        throw std::invalid_argument("Invalid operation type during test execution found!");
        break;
    }
    return false;
}

std::vector<std::uint64_t> execute_op(const std::vector<std::uint64_t> &worry_level, const WorryOperation &op)
{
    std::vector<std::uint64_t> new_worries;
    for (const auto &item_worry : worry_level)
    {
        std::uint64_t worry{ 0u };
        switch (op.type)
        {
        case AdaptionType::Addition:
            worry = item_worry + op.val;
            break;
        case AdaptionType::Multiplication:
            worry = item_worry * op.val;
            break;
        case AdaptionType::Square:
            worry = item_worry * item_worry;
            break;
        case AdaptionType::Division:
            worry = item_worry / op.val; // After division we round down to nearest integer val
            break;
        default:
            throw std::invalid_argument("Invalid operation type in operation found!");
            break;
        }
        new_worries.push_back(worry);
    }
    return new_worries;
}

WorryOperation get_adaption_from_test_str(const std::string &str)
{
    WorryOperation worry_adaption;
    size_t pos = str.find("divisible");

    if (pos != std::string::npos)
    {
        worry_adaption.type = AdaptionType::Division;
        worry_adaption.val = get_number_after_str<std::int64_t>(str, "divisible");
    }
    else
    {
        throw std::invalid_argument("Invalid operation type in test string found!\n" + str);
    }
    return worry_adaption;
}

WorryOperation get_adaption_from_op_str(const std::string &str)
{
    WorryOperation worry_adaption;
    size_t pos = str.find('*');

    if (pos != std::string::npos)
    {
        if (std::string::npos != str.find("old", pos)) // operation: new = old * old
        {
            worry_adaption.type = AdaptionType::Square;
        }
        else // operation new = old * x
        {
            worry_adaption.type = AdaptionType::Multiplication;
            worry_adaption.val = get_number_after_str<std::int64_t>(str, "old");
        }
    }
    else
    {
        if (str.find('+') != std::string::npos) // operation new = old + x
        {
            worry_adaption.type = AdaptionType::Addition;
            worry_adaption.val = get_number_after_str<std::int64_t>(str, "old");
        }
        else
        {
            throw std::invalid_argument("Invalid operation type in operation string found!\n" + str);
        }
    }
    return worry_adaption;
}


