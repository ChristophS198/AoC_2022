#include <string>
#include <vector>
#include <memory>

#include "../utility.h"


using FSize = std::uint64_t;

constexpr FSize TOTAL_DISK_SPACE = 70'000'000u;
constexpr FSize UPDATE_SPACE = 30'000'000u;

struct File
{
    std::string name;
    FSize size{ 0u };
};

struct Folder
{
    std::vector<std::shared_ptr<Folder>> subfolder;
    std::vector<File> files;
    std::shared_ptr<Folder> parent;
    std::string name;
    FSize size{ 0u };
};

enum class LineType : uint8_t
{
    CD_UP,
    CD_DOWN,
    LS,
    FILE,
    DIR
};

std::shared_ptr<Folder> build_filesystem(const std::vector<std::string> &input_data, FSize &sum);
std::shared_ptr<Folder> build_filesystem_rec(const std::vector<std::string> &input_data, const std::shared_ptr<Folder> &parent, size_t &cur_line, FSize &summed_size);
std::string get_folder_name(const std::string &line);
LineType get_line_type(const std::string &line);
void get_smallest_suitable_folder(std::shared_ptr<Folder> &root, const FSize space_val, std::shared_ptr<Folder> &best_folder);

FSize day_7_1(const std::string &file_path)
{
    std::vector<std::string> input_data = read_string_vec_from_file(file_path);
    FSize summed_size{ 0u };
    std::shared_ptr<Folder> root = build_filesystem(input_data, summed_size);

    return summed_size;
}

FSize day_7_2(const std::string &file_path)
{
    std::vector<std::string> input_data = read_string_vec_from_file(file_path);
    FSize summed_size{ 0u };
    std::shared_ptr<Folder> root = build_filesystem(input_data, summed_size);
    FSize free_space = TOTAL_DISK_SPACE - root->size;
    FSize req_space = UPDATE_SPACE - free_space;
    std::shared_ptr<Folder> best_folder = root;
    get_smallest_suitable_folder(root, req_space, best_folder);
    return best_folder->size;
}

void get_smallest_suitable_folder(std::shared_ptr<Folder> &root, const FSize space_val, std::shared_ptr<Folder> &best_folder)
{
    if (root->size < best_folder->size && root->size >= space_val)
    {
        best_folder = root;
    }
    for (auto &elem : root->subfolder)
    {
        if (elem->size >= space_val) 
        {
            get_smallest_suitable_folder(elem, space_val, best_folder);
        }
    }
}


std::shared_ptr<Folder> build_filesystem(const std::vector<std::string> &input_data, FSize &sum)
{
    size_t line_num{ 0u };
    sum = 0u;
    std::shared_ptr<Folder> root = build_filesystem_rec(input_data, nullptr, line_num, sum);
    return root;
}

std::shared_ptr<Folder> build_filesystem_rec(const std::vector<std::string> &input_data, const std::shared_ptr<Folder> &parent, size_t &cur_line, FSize &sum)
{
    std::shared_ptr<Folder> folder = std::make_shared<Folder>();
    folder->name = get_folder_name(input_data.at(cur_line++));
    folder->parent = parent;
    std::uint8_t num_ls{ 0u };

    while (cur_line < input_data.size() && LineType::CD_UP != get_line_type(input_data.at(cur_line)))
    {
        LineType line_type = get_line_type(input_data.at(cur_line));
        std::vector<std::string> str_vec = split_string(input_data.at(cur_line), " ");
        std::shared_ptr<Folder> sub_folder = nullptr;
        switch (line_type)
        {
        case LineType::LS:
            if (++num_ls > 1) throw std::runtime_error("More than one read per level!");
            break;
        case LineType::CD_DOWN:
            sub_folder = build_filesystem_rec(input_data, folder, cur_line, sum);
            folder->size += sub_folder->size;
            folder->subfolder.push_back(sub_folder);
            break;
        case LineType::FILE:
            folder->size += parse_string_to_number<FSize>(str_vec.at(0));
            folder->files.push_back(File{ str_vec.at(1), parse_string_to_number<FSize>(str_vec.at(0)) });
            break;
        case LineType::DIR:
            /*Do nothing*/
            break;
        
        default:
            throw std::runtime_error("Unknown LineType found! Line Content:\n" + input_data.at(cur_line));
            break;
        }

        ++cur_line;
    }
    if (folder->size <= 100'000u)
    {
        sum += folder->size;
    }
    return folder;
}

std::string get_folder_name(const std::string &line)
{
    return split_string(line, " ").back();
}

LineType get_line_type(const std::string &line)
{
    std::vector<std::string> str_vec = split_string(line, " ");
    LineType line_type;
    if ("$" == str_vec.at(0))
    {
        if ("cd" == str_vec.at(1))
        {
            if (".." == str_vec.at(2))
            {
                line_type = LineType::CD_UP;
            }
            else
            {
                line_type = LineType::CD_DOWN;
            }
        }
        else
        {
            line_type = LineType::LS;
        }
    }
    else
    {
        if ("dir" == str_vec.at(0))
        {
            line_type = LineType::DIR;
        }
        else
        {
            line_type = LineType::FILE;
        }
    }
    return line_type;
}