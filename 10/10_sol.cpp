#include <string>
#include <map>
#include <vector>
#include <numeric>
#include <fstream>

#include "../utility.h"

using Cycles = std::uint16_t;
using Signal = std::string;
using Bitmap = std::vector<std::vector<bool>>;

constexpr std::int32_t START_X = 1; // Init value x for register
constexpr std::size_t WIDTH = 40u; // CRT screen width in pixel
constexpr std::size_t HEIGHT = 6u; // CRT screen height in pixel

const std::map<Signal, Cycles>  sig_cycle_map{ {"noop", 1}, {"addx", 2} };

struct Instruction
{
    Signal sig; ///< signal to be executed
    std::int32_t x; ///< for add operations this value is added during execution
};

std::vector<Instruction> get_instructions(const std::string &file_path);
std::vector<std::int32_t> get_signal_strenghts_at(const std::vector<Instruction> &instructions, const std::vector<Cycles> &cycles, const std::map<Signal, Cycles>  sig_cycle_map);
void execute_instruction(std::int32_t &x, const Instruction &ins);
void get_CRT_output(Bitmap &bitmap, const std::vector<Instruction> &instructions, const std::map<Signal, Cycles>  sig_cycle_map);
void print_CRT_bitmap(std::ostream &out, const Bitmap &crt_out);

std::uint32_t day_10_1(const std::string &file_path)
{
    std::vector<Instruction> instructions = get_instructions(file_path);
    const std::vector<Cycles> cycles_vec{ 20, 60, 100, 140, 180, 220 };
    std::vector<std::int32_t> sig_strengths = get_signal_strenghts_at(instructions, cycles_vec, sig_cycle_map);

    return std::accumulate(sig_strengths.begin(), sig_strengths.end(), decltype(sig_strengths)::value_type(0));
}

void day_10_2(const std::string &file_path, const std::string &out_file)
{
    std::vector<Instruction> instructions = get_instructions(file_path);
    Bitmap bit_map(HEIGHT, std::vector<bool>(WIDTH, false));
    get_CRT_output(bit_map, instructions, sig_cycle_map);
    std::ofstream out_stream(out_file);
    print_CRT_bitmap(out_stream, bit_map);
    out_stream.close();
}

void get_CRT_output(Bitmap &bitmap, const std::vector<Instruction> &instructions, const std::map<Signal, Cycles>  sig_cycle_map)
{
    std::int32_t sprite_pos{ START_X };
    Cycles cycle_ctr{ 1u };
    int32_t pixel_pos{ 0u };
    size_t ins_ctr{ 0u };
    Cycles sub_cycles{ 0u };
    Instruction cur_ins;
    size_t crt_pos_x{ 0u };
    size_t crt_pos_y{ 0u };

    while (ins_ctr < instructions.size() || sub_cycles > 0)
    {
        pixel_pos = static_cast<int32_t>(cycle_ctr) - 1;
        // if pixel position is within 1 pixel distance of sprite position draw light pixel
        if (std::abs((pixel_pos%40) - sprite_pos) < 2)
        {
            crt_pos_x = pixel_pos % WIDTH;
            crt_pos_y = pixel_pos / WIDTH;
            bitmap.at(crt_pos_y).at(crt_pos_x) = true;
        }

        // if current instruction is finished -> get next instruction 
        if (sub_cycles == 0 && ins_ctr < instructions.size())
        {
            cur_ins = instructions.at(ins_ctr++);
            sub_cycles = sig_cycle_map.at(cur_ins.sig);
        }
        if (sub_cycles == 1)
        {
            execute_instruction(sprite_pos, cur_ins);
        }
        --sub_cycles;
        ++cycle_ctr;
    }
}

void print_CRT_bitmap(std::ostream &out, const Bitmap &crt_out)
{
    out << "\n";
    for (const auto &row : crt_out)
    {
        for (const auto &elem : row)
        {
            if (elem)
            {
                out << "#";
            }
            else
            {
                out << ".";
            }
        }
        out << "\n";
    }
    out << "\n";
}

std::vector<std::int32_t> get_signal_strenghts_at(const std::vector<Instruction> &instructions, const std::vector<Cycles> &stren_cycles, const std::map<Signal, Cycles>  sig_cycle_map)
{
    std::int32_t x{ START_X };
    Cycles cycle_ctr{ 1u };
    size_t ins_ctr{ 0u };
    Cycles sub_cycles{ 0u };
    Cycles cur_c = stren_cycles.at(0);
    size_t stren_cycles_ctr{ 0u };
    Instruction cur_ins;
    std::vector<std::int32_t> signal_strengths;

    while (cycle_ctr <= stren_cycles.back())
    {
        if (cycle_ctr == cur_c)
        {
            signal_strengths.push_back(x * cycle_ctr);
            if (stren_cycles_ctr < stren_cycles.size()-1)
            {
                cur_c = stren_cycles.at(++stren_cycles_ctr);
            }
        }
        // if current instruction is finished -> get next instruction 
        if (sub_cycles == 0 && ins_ctr < instructions.size())
        {
            cur_ins = instructions.at(ins_ctr++);
            sub_cycles = sig_cycle_map.at(cur_ins.sig);
        }
        if (sub_cycles == 1)
        {
            execute_instruction(x, cur_ins);
        }
        --sub_cycles;
        ++cycle_ctr;
    }
    return signal_strengths;
}

void execute_instruction(std::int32_t &x, const Instruction &ins)
{
    if (ins.sig == "addx")
    {
        x += ins.x;
    }
}

std::vector<Instruction> get_instructions(const std::string &file_path)
{
    std::vector<Instruction> instructions;
    std::vector<std::string> data_in = read_string_vec_from_file(file_path);

    for (const auto &line : data_in)
    {
        Instruction new_ins;
        new_ins.sig = split_string(line, " ").at(0);
        new_ins.x = new_ins.sig == "noop" ? 0 : parse_string_to_number<std::int32_t>(line);
        instructions.push_back(new_ins);
    }
    return instructions;
}
