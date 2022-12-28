#include <string>
#include <ostream>
#include <stack>
#include <set>

#include "../utility.h"

constexpr char B_WALL{ '#' }; // Char for valley wall
constexpr char B_LEFT{ '<' }; // Char for left blowing blizzard
constexpr char B_RIGHT{ '>' }; // Char for right blowing blizzard
constexpr char B_DOWN{ 'v' }; // Char for downward blowing blizzard
constexpr char B_UP{ '^' }; // Char for upward blowing blizzard
constexpr char B_FREE{ '.' }; // Free field
constexpr char B_TWO{ '2' }; // Two blizzards meet in the same location
constexpr char B_THREE{ '3' }; // Three blizzards meet in the same location
constexpr char B_FOUR{ '4' }; // Four blizzards meet in the same location


using BlizzField = std::vector<std::vector<char>>;


struct CombFields
{
    std::vector<BlizzField> hor_blizzards;
    std::vector<BlizzField> vert_blizzards;
};

struct EState
{
    Point<uint16_t> pos;
    uint16_t steps;
    bool operator<(const EState &s) const
    {
        return pos != s.pos ? pos < s.pos : steps < s.steps; 
    }
};

CombFields precompute_fields(const std::string &file_path);

std::vector<BlizzField> precompute_single_field(const BlizzField &init_field, size_t steps, bool is_vertical);
BlizzField do_blizzard_step(const BlizzField &field, bool is_vertical);
std::vector<BlizzField> combine_fields(const CombFields& comb_fields);
char combine_elem(const char c1, const char c2);
std::vector<Point<uint16_t>> get_possible_successors(const EState state, const BlizzField &field);
std::pair<EState, EState> get_start_end_pts(const BlizzField &init_field);
std::uint16_t get_shortest_path_dfs(const std::vector<BlizzField> &fields, std::uint16_t max_steps, EState start, const Point<uint16_t> end);
std::uint16_t get_shortest_path_bfs(const std::vector<BlizzField> &fields, EState start, const Point<uint16_t> end);
void print_blizzard(std::ostream &out, const BlizzField &field);

int32_t day_24_1(const std::string &file_path)
{
    CombFields precomp_fields = precompute_fields(file_path);
    std::vector<BlizzField> comb_fields = combine_fields(precomp_fields);
    std::pair<EState, EState> start_end_pts = get_start_end_pts(comb_fields.at(0));

    EState start = start_end_pts.first;
    EState end = start_end_pts.second;
    std::uint16_t min_steps = get_shortest_path_bfs(comb_fields, start, end.pos);
    
    // DFS does not work
    // std::uint16_t min_steps = get_shortest_path_dfs(comb_fields, 200, start, end.pos);

    // size_t i{ 11 };
    // std::ofstream out("24/hor_field.txt");
    // print_blizzard(out, precomp_fields.hor_blizzards[i%precomp_fields.hor_blizzards.size()]);
    // out.close();
    // std::ofstream out2("24/vert_field.txt");
    // print_blizzard(out2, precomp_fields.vert_blizzards[i%precomp_fields.vert_blizzards.size()]);
    // out2.close();
    std::ofstream out3("24/field.txt");
    for (size_t i=0; i<comb_fields.size(); ++i)
    {
        print_blizzard(out3, comb_fields[i%comb_fields.size()]);
        out3 << "\n \n";
    }
    out3.close();
    return min_steps;
}

int32_t day_24_2(const std::string &file_path)
{
    CombFields precomp_fields = precompute_fields(file_path);
    std::vector<BlizzField> comb_fields = combine_fields(precomp_fields);
    std::pair<EState, EState> start_end_pts = get_start_end_pts(comb_fields.at(0));

    EState start = start_end_pts.first;
    start.steps = 0u;
    EState end = start_end_pts.second;

    std::uint16_t min_steps = get_shortest_path_bfs(comb_fields, start, end.pos);
    end.steps = min_steps;
    min_steps = get_shortest_path_bfs(comb_fields, end, start.pos);
    start.steps = min_steps;
    min_steps = get_shortest_path_bfs(comb_fields, start, end.pos);

    // Intermediate step numbers: 269, 555, 825
    return min_steps;
}
std::uint16_t get_shortest_path_bfs(const std::vector<BlizzField> &fields, EState start, const Point<uint16_t> end)
{
    std::set<EState> cur_shortest_paths{ start };

    while (!cur_shortest_paths.empty())
    {
        std::set<EState> new_states;
        for (auto &cur_state : cur_shortest_paths)
        {
            // check end conditions
            if (cur_state.pos == end)
            {
                return cur_state.steps;
            }
            else // do another step in all possible directions (or wait)
            {
                std::vector<Point<uint16_t>> succ = get_possible_successors(cur_state, fields.at((cur_state.steps+1u)%fields.size()));
                for (const auto &elem : succ)
                {
                    EState new_state{ elem,cur_state.steps+1u };
                    new_states.insert(new_state);
                }
            }
        }
        cur_shortest_paths = new_states;
    }
    throw std::runtime_error("Unable to find end point through BFS!");
}

std::uint16_t get_shortest_path_dfs(const std::vector<BlizzField> &fields, std::uint16_t max_steps, EState start, const Point<uint16_t> end)
{
    std::stack<EState> reachable_states;
    reachable_states.push(start);

    while (!reachable_states.empty())
    {
        EState cur_state = reachable_states.top();
        reachable_states.pop();

        // check end conditions
        if (cur_state.pos == end || cur_state.steps == max_steps)
        {
            if (cur_state.steps < max_steps)
            {
                max_steps = cur_state.steps;
                std::cout << "New shortest path: " << max_steps << std::endl;
            }
        }
        else // do another step in all possible directions (or wait)
        {
            ++cur_state.steps;
            std::vector<Point<uint16_t>> succ = get_possible_successors(cur_state, fields.at(cur_state.steps%fields.size()));
            for (const auto &elem : succ)
            {
                EState new_state{ elem,cur_state.steps };
                reachable_states.push(new_state);
            }
        }
    }

    return max_steps;
}

std::vector<Point<uint16_t>> get_possible_successors(const EState state, const BlizzField &field)
{
    std::vector<Point<uint16_t>> successors;

    if (field.at(state.pos.x).at(state.pos.y) == B_FREE) successors.push_back({ state.pos.x,state.pos.y });
    if (state.pos.x < (field.size()-1) && field.at(state.pos.x+1u).at(state.pos.y) == B_FREE) successors.push_back({ state.pos.x+1u,state.pos.y });
    if (state.pos.x > 0 && field.at(state.pos.x-1u).at(state.pos.y) == B_FREE) successors.push_back({ state.pos.x-1u,state.pos.y });
    if (field.at(state.pos.x).at(state.pos.y+1u) == B_FREE) successors.push_back({ state.pos.x,state.pos.y+1u });
    if (field.at(state.pos.x).at(state.pos.y-1u) == B_FREE) successors.push_back({ state.pos.x,state.pos.y-1u });

    return successors;
}

// here we combine horizontal and vertical fields 
// the resulting fields only have information about whether a location 
// is blizzard free or not
std::vector<BlizzField> combine_fields(const CombFields& comb_fields)
{
    std::vector<BlizzField> fields;
    size_t num_vert_blizz{ comb_fields.vert_blizzards.size() };
    size_t num_hor_blizz{ comb_fields.hor_blizzards.size() };
    size_t rows{ comb_fields.hor_blizzards.at(0).size() };
    size_t cols{ comb_fields.hor_blizzards.at(0).at(0).size() };

    // calc how many fields we need to precompute until we reach cyclic behaviour
    size_t x{ num_hor_blizz };
    for (; x <=1000u; x+=num_hor_blizz)
    {
        if (x%num_vert_blizz == 0) break;
    }
    // std::cout << "Cyclic behaviour after " << x << " steps" << std::endl;

    // iterate through horizontal and vertical blizzards and create new combined one
    for (size_t i=0; i<x; ++i)
    {
        BlizzField new_field{ comb_fields.hor_blizzards.at(i%num_hor_blizz) };
        for (size_t row=0; row<rows; ++row)
        {
            for (size_t col=0; col<cols; ++col)
            {
                auto &cur_elem = new_field.at(row).at(col);
                cur_elem = combine_elem(cur_elem, comb_fields.vert_blizzards.at(i%num_vert_blizz).at(row).at(col));
            }
        }
    fields.push_back(new_field);
    }

    return fields;
}

// combines horizontal and vertical blizzard elements
char combine_elem(const char c1, const char c2)
{
    if (c1 == B_FREE) return c2;
    if (c2 == B_FREE) return c1;
    if (c1 == B_WALL || c2 == B_WALL) return B_WALL;
    if (c1 == B_LEFT || c1 == B_RIGHT || c1 == B_UP || c1 == B_DOWN)
    {
        if (c2 == B_LEFT || c2 == B_RIGHT || c2 == B_UP || c2 == B_DOWN)
        {
            return B_TWO;
        }
        if (c2 == B_TWO) return B_THREE;
    }
    else
    {
        if (c1 == B_TWO)
        {
            if (c2 == B_LEFT || c2 == B_RIGHT || c2 == B_UP || c2 == B_DOWN)
            {
                return B_THREE;
            }
            if (c2 == B_TWO) return B_FOUR;
        }
    }

    throw std::runtime_error("Should never happen!");
}

// If horizontal and vertical blowing fields are separated, we have a cyclic behaviour
// after horizontal or vertical valley lenght
CombFields precompute_fields(const std::string &file_path)
{
    CombFields comb_fields{};
    BlizzField init_field = read_2d_vec_from_file<char>(file_path);
    size_t rows{ init_field.size() };
    size_t cols{ init_field.at(0).size() };
    BlizzField vert_field(rows, std::vector<char>(cols,B_FREE));
    BlizzField horizontal_field(rows, std::vector<char>(cols,B_FREE));

    for (size_t row=0; row<rows; ++row)
    {
        for (size_t col=0; col<cols; ++col)
        {
            const auto &cur_f = init_field.at(row).at(col);
            
            switch (cur_f)
            {
            case B_LEFT:
                [[Fallthrough]]
            case B_RIGHT:
                horizontal_field.at(row).at(col) = cur_f;
                break;
            case B_UP:
                [[Fallthrough]]
            case B_DOWN:
                vert_field.at(row).at(col) = cur_f;
                break;
            case B_WALL:
                vert_field.at(row).at(col) = cur_f;
                horizontal_field.at(row).at(col) = cur_f;
                break;
            default:
                break;
            }
        }
    }

    comb_fields.hor_blizzards = precompute_single_field(horizontal_field, cols-2, false); // -2 due to walls to the left and right
    comb_fields.vert_blizzards = precompute_single_field(vert_field, rows-2, true); // -2 due to walls above and below
    return comb_fields;
}

std::vector<BlizzField> precompute_single_field(const BlizzField &init_field, size_t steps, bool is_vertical)
{
    std::vector<BlizzField> blizz_fields;
    blizz_fields.push_back(init_field);

    for (size_t i=1; i<steps; ++i)
    {
        blizz_fields.push_back(do_blizzard_step(blizz_fields[i-1], is_vertical));
    }
    return blizz_fields;
}

BlizzField do_blizzard_step(const BlizzField &field, bool is_vertical)
{
    size_t rows{ field.size() };
    size_t cols{ field.at(0).size() };
    BlizzField new_field(rows, std::vector<char>(cols,B_FREE));

    for (size_t row=0; row<rows; ++row)
    {
        for (size_t col=0; col<cols; ++col)
        {
            const auto &cur_f = field.at(row).at(col);
            if (cur_f == B_FREE)
            {
                continue;
            }
            if (cur_f == B_WALL)
            {
                new_field.at(row).at(col) = B_WALL;
                continue;
            }

            // consider special cases of a blizzard hitting a wall
            if (col == 1 && (cur_f == B_LEFT || (cur_f == B_TWO && !is_vertical))) 
            {
                new_field.at(row).at(cols-2) = B_LEFT;
                if (cur_f == B_TWO && !is_vertical) new_field.at(row).at(2) = B_RIGHT;
                continue;
            }
            if (col == cols-2 && (cur_f == B_RIGHT || (cur_f == B_TWO && !is_vertical))) 
            {
                if (new_field.at(row).at(1) == B_LEFT) new_field.at(row).at(1) = B_TWO;
                else
                {
                    if (new_field.at(row).at(1) == B_FREE) new_field.at(row).at(1) = B_RIGHT;
                    else throw std::runtime_error("Unknown field type at col=cols-2 found!");
                }

                if (cur_f == B_TWO && !is_vertical) 
                {
                    if (new_field.at(row).at(cols-3) == B_RIGHT) new_field.at(row).at(cols-3) = B_TWO;
                    else
                    {
                        if(new_field.at(row).at(cols-3) == B_FREE) new_field.at(row).at(cols-3) = B_LEFT;
                        else throw std::runtime_error("Unknown field type at col=cols-3 found!");
                    }
                }
                continue;
            }
            if (row == 1 && (cur_f == B_UP || (cur_f == B_TWO && is_vertical))) 
            {
                new_field.at(rows-2).at(col) = B_UP;
                if (cur_f == B_TWO && is_vertical) new_field.at(2).at(col) = B_DOWN;
                continue;
            }
            if (row == rows-2 && (cur_f == B_DOWN || (cur_f == B_TWO && is_vertical))) 
            {
                if (new_field.at(1).at(col) == B_UP) new_field.at(1).at(col) = B_TWO;
                else
                {
                    if (new_field.at(1).at(col) == B_FREE) new_field.at(1).at(col) = B_DOWN;
                    else throw std::runtime_error("Unknown field type at row 1 found!");
                }
                if (cur_f == B_TWO && is_vertical) 
                {
                    if (new_field.at(rows-3).at(col) == B_DOWN) new_field.at(rows-3).at(col) = B_TWO; 
                    else
                    {
                        if(new_field.at(rows-3).at(col) == B_FREE) new_field.at(rows-3).at(col) = B_UP;
                        else throw std::runtime_error("Unknown field type at row=rows-3 found!");
                    }
                }
                continue;
            }

            // 
            switch (cur_f)
            {
            case B_LEFT:
                if (new_field.at(row).at(col-1) == B_FREE) new_field.at(row).at(col-1) = cur_f;
                if (new_field.at(row).at(col-1) == B_RIGHT) new_field.at(row).at(col-1) = B_TWO;
                
                break;
            case B_RIGHT:
                if (new_field.at(row).at(col+1) == B_FREE) new_field.at(row).at(col+1) = cur_f;
                if (new_field.at(row).at(col+1) == B_LEFT) new_field.at(row).at(col+1) = B_TWO;
                break;
            case B_UP:
                if (new_field.at(row-1).at(col) == B_FREE) new_field.at(row-1).at(col) = cur_f;
                if (new_field.at(row-1).at(col) == B_DOWN) new_field.at(row-1).at(col) = B_TWO;
                break;
            case B_DOWN:
                if (new_field.at(row+1).at(col) == B_FREE) new_field.at(row+1).at(col) = cur_f;
                if (new_field.at(row+1).at(col) == B_UP) new_field.at(row+1).at(col) = B_TWO;
                break;
            case B_TWO: 
                if (is_vertical)
                {
                    if (new_field.at(row-1).at(col) == B_FREE) new_field.at(row-1).at(col) = B_UP;
                    if (new_field.at(row-1).at(col) == B_DOWN) new_field.at(row-1).at(col) = B_TWO;
                    if (new_field.at(row+1).at(col) == B_FREE) new_field.at(row+1).at(col) = B_DOWN;
                    if (new_field.at(row+1).at(col) == B_UP) new_field.at(row+1).at(col) = B_TWO;
                }
                else
                {
                    if (new_field.at(row).at(col-1) == B_FREE) new_field.at(row).at(col-1) = B_LEFT;
                    if (new_field.at(row).at(col-1) == B_RIGHT) new_field.at(row).at(col-1) = B_TWO;
                    if (new_field.at(row).at(col+1) == B_FREE) new_field.at(row).at(col+1) = B_RIGHT;
                    if (new_field.at(row).at(col+1) == B_LEFT) new_field.at(row).at(col+1) = B_TWO;
                }
            default:
                break;
            }
        }
    }

    return new_field;
}

std::pair<EState, EState> get_start_end_pts(const BlizzField &init_field)
{
    std::pair<EState, EState> start_end_states{};
    size_t col{ 0u };
    while (init_field.at(0).at(col) == B_WALL) 
    {
        ++col;
    }
    start_end_states.first = EState{ { 0,static_cast<uint16_t>(col) }};

    col = 0u;
    while (init_field.at(init_field.size()-1).at(col) == B_WALL) 
    {
        ++col;
    }
    start_end_states.second = EState{ { static_cast<uint16_t>(init_field.size()-1),static_cast<uint16_t>(col) }};

    return start_end_states;
}

void print_blizzard(std::ostream &out, const BlizzField &field)
{
    for (const auto &row : field)
    {
        for (const auto &elem : row)
        {
            out << elem;
        }
        out << "\n";
    }
}