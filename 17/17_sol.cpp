#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>

#include "stone_form.h"

constexpr std::uint8_t CHAMBER_WIDTH{ 7u };
constexpr std::uint64_t ROCK_NUMS{ 2022u };
constexpr std::uint64_t ROCK_NUMS_2{ 1'000'000'000'000u };
constexpr std::uint64_t START_HEIGHT{ 8000u }; // Init size of chmaber
constexpr THeight INIT_Y{ 3u }; // Y-height offset
constexpr THeight INIT_X{ 3u }; // X-offset from wall

std::string get_input(const std::string &file_path);
StoneForm new_rock(RockPts rock, const THeight offset);
std::vector<RockPts> create_rocks(const THeight l);
void set_chamber_floor(Chamber &chamber);
void print_chamber(std::ostream &out, const Chamber &chamber);


THeight day_17_1(const std::string &file_path)
{
    std::vector<RockPts> rocks = create_rocks(INIT_X);
    std::string jet_dir = get_input(file_path);

    std::vector<char> level{ '#','.','.','.','.','.','.','.','#' };
    Chamber chamber(START_HEIGHT,level);
    set_chamber_floor(chamber);
    std::uint64_t rock_ctr{ 0u };
    THeight max_height{ 0u };
    uint64_t jet_idx{ 0u };

    while (rock_ctr < ROCK_NUMS)
    {
        uint64_t rock_idx { rock_ctr%rocks.size() };
        StoneForm rock = new_rock(rocks[rock_idx], max_height+INIT_Y);

        rock.do_gas_jet(jet_dir[jet_idx%jet_dir.size()], chamber);
        ++jet_idx;
        while(rock.fall_down(chamber))
        {
            rock.do_gas_jet(jet_dir[jet_idx%jet_dir.size()], chamber);
            ++jet_idx;
        }
        max_height = set_stone(rock.get_rock_pts(), chamber, max_height);

        ++rock_ctr;        
    }
    std::ofstream out_stream_2("17/Rock_1.txt");
    print_chamber(out_stream_2, chamber);
    out_stream_2.close();

    return max_height;
}


/* 
Idea is to search for a cyclic behaviour in the stone fall process, by 
storing a snapshot of the chamber state after each stone is set. 
So the first stones are set more or less randomly, but at some point the stones repeatedly
set to the same places and a cycle has a length of jet_dir.size()
*/
THeight day_17_2(const std::string &file_path)
{
    std::vector<RockPts> rocks = create_rocks(INIT_X);
    std::string jet_dir = get_input(file_path);

    std::vector<char> level{ '#','.','.','.','.','.','.','.','#' };
    Chamber chamber(START_HEIGHT,level);
    set_chamber_floor(chamber);
    std::uint64_t rock_ctr{ 0u };
    THeight max_height{ 0u };
    uint64_t jet_idx{ 0u };
    uint64_t rock_limit{ ROCK_NUMS_2 };
    uint64_t cycle_height{ 0u };
    uint64_t cycle_num{ 0u }; 

    std::vector<Snapshot> snapshots(jet_dir.size());  // Cycle length in jet blows should be exactly jet_dir.size()

    while (rock_ctr < rock_limit)
    {
        uint64_t rock_idx { rock_ctr%rocks.size() };
        StoneForm rock = new_rock(rocks[rock_idx], max_height+INIT_Y);

        rock.do_gas_jet(jet_dir[jet_idx%jet_dir.size()], chamber);
        ++jet_idx;
        while(rock.fall_down(chamber))
        {
            rock.do_gas_jet(jet_dir[jet_idx%jet_dir.size()], chamber);
            ++jet_idx;
        }
        auto pts = rock.get_rock_pts();
        max_height = set_stone(pts, chamber, max_height);

        // Generate new snapshot of current chamber state and rock + jet index
        Snapshot snap{ rock_idx, jet_idx%jet_dir.size(), rock_ctr, jet_idx, max_height, 
                        { chamber.at(max_height), chamber.at(max_height-1) } };

        // If no cycle has been found so far, compare new snapshot to the one jet_dir.size() before
        const auto &comp = snapshots.at(jet_idx % jet_dir.size());
        if (snap == comp && cycle_num == 0u)
        {
            // Calculate height of one cycle and how many complete cycles fit into the number of remaining rock falls
            uint64_t cycle_len_in_rocks = rock_ctr - comp.rock_ctr;
            cycle_height = max_height - comp.height;
            cycle_num = static_cast<uint64_t>((rock_limit-rock_ctr) / cycle_len_in_rocks); // number of whole cycles still fitting into remaining rock falls
            rock_limit = rock_limit - cycle_num * cycle_len_in_rocks;
        }
        else 
        {
            // Store snapshot for future comparison
            snapshots[jet_idx % jet_dir.size()] = snap;
        }

        ++rock_ctr;
    }
    max_height = max_height + cycle_num * cycle_height;
    // std::ofstream out_stream_2("17/Rock_2.txt");
    // print_chamber(out_stream_2, chamber);
    // out_stream_2.close();
    return max_height;
}

StoneForm new_rock(RockPts rock, const THeight offset)
{
    for (auto &elem : rock)
    {
        elem.y += offset;
    }
    return StoneForm(rock);
}

// l can be used to set the leftmost point of each rock
std::vector<RockPts> create_rocks(const THeight l)
{
    std::vector<RockPts> rocks;
    rocks.push_back({ {l,1},{l+1,1},{l+2,1},{l+3,1} }); 
    rocks.push_back({ {l+1,1},{l,2},{l+1,2},{l+2,2},{l+1,3} }); 
    rocks.push_back({ {l,1},{l+1,1},{l+2,1},{l+2,2},{l+2,3} }); 
    rocks.push_back({ {l,1},{l,2},{l,3},{l,4} }); 
    rocks.push_back({ {l,1},{l+1,1},{l,2},{l+1,2} }); 

    return rocks;
}

void set_chamber_floor(Chamber &chamber)
{
    for (size_t i=0; i<chamber.at(0).size(); ++i)
    {
        chamber.at(0).at(i) = '#';
    }
}

std::string get_input(const std::string &file_path)
{
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    std::string input_line;
    if (input_file.is_open())
    {
        getline(input_file, input_line);  
        input_file.close();  
        return input_line;
    }
    throw std::runtime_error("Failed to read input file: " + file_path);
}

void print_chamber(std::ostream &out, const Chamber &chamber)
{
    for (int64_t row=chamber.size()-1; row>0; --row)
    {
        for (uint64_t col=0; col<chamber.at(0).size(); ++col)
        {
            out << chamber.at(row).at(col);
        }
        out << "\n";
    }
}