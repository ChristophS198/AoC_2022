#include <string>
#include <map>
#include <vector>

#include "../utility.h"

using Score = std::uint32_t;

constexpr Score WIN_SCORE = 6u;
constexpr Score DRAW_SCORE = 3u;

enum class Shape : std::uint8_t
{
    Rock,
    Paper,
    Scissor 
};

enum class Result : std::uint8_t
{
    Loss, 
    Draw, 
    Win
};

const std::map<char, Shape> shape_map = {   {'A',Shape::Rock}, {'B',Shape::Paper}, {'C',Shape::Scissor},
                                            {'X',Shape::Rock}, {'Y',Shape::Paper}, {'Z',Shape::Scissor} };
const std::map<Shape, Score> score_map = { {Shape::Rock, 1}, {Shape::Paper, 2}, {Shape::Scissor, 3} };
const std::map<char, Result> result_map = { {'X',Result::Loss}, {'Y',Result::Draw}, {'Z',Result::Win} };

std::vector<std::pair<Shape, Shape>> get_predictions(const std::string &file_path);
std::vector<std::pair<Shape, Result>> get_predictions_2(const std::string &file_path);
Score calc_prediction_score(const std::vector<std::pair<Shape, Shape>> &pred_vec);
std::vector<std::pair<Shape, Shape>> calc_shapes(const std::vector<std::pair<Shape, Result>> &pred);
Shape interpret_pred(const Shape shape, const Result res);
Score play_game(Shape a, Shape b);

Score day_2_1(const std::string &file_path)
{
    std::vector<std::pair<Shape, Shape>> pred = get_predictions(file_path);
    return calc_prediction_score(pred);
}

Score day_2_2(const std::string &file_path)
{
    std::vector<std::pair<Shape, Result>> pred = get_predictions_2(file_path);
    std::vector<std::pair<Shape, Shape>> pred_shapes = calc_shapes(pred);

    return calc_prediction_score(pred_shapes);
}

Shape interpret_pred(const Shape shape, const Result res)
{
    if (Result::Draw == res)
    {
        return shape;
    }
    if (Shape::Rock == shape)
    {
        if (Result::Win == res)
        {
            return Shape::Paper;
        }
        else
        {
            return Shape::Scissor;
        }
    }
    if (Shape::Paper == shape)
    {
        if (Result::Win == res)
        {
            return Shape::Scissor;
        }
        else
        {
            return Shape::Rock;
        }
    }
    if (Shape::Scissor == shape)
    {
        if (Result::Win == res)
        {
            return Shape::Rock;
        }
        else
        {
            return Shape::Paper;
        }
    }
    throw std::invalid_argument("Outcome undefined!");
}


std::vector<std::pair<Shape, Shape>> calc_shapes(const std::vector<std::pair<Shape, Result>> &pred_vec)
{
    std::vector<std::pair<Shape, Shape>> pred_shapes{};
    for (const auto &pred : pred_vec)
    {
        Shape my_shape = interpret_pred(pred.first, pred.second);
        pred_shapes.push_back(std::make_pair(pred.first, my_shape));
    }
    return pred_shapes;
}

Score calc_prediction_score(const std::vector<std::pair<Shape, Shape>> &pred_vec)
{
    Score sum_score{ 0 };
    for (const auto &pred_pair : pred_vec)
    {
        sum_score += score_map.at(pred_pair.second);
        sum_score += play_game(pred_pair.second, pred_pair.first);
    }
    return sum_score;
}

Score play_game(Shape a, Shape b)
{
    if (a == b)
    {
        return DRAW_SCORE;
    }
    if (a == Shape::Rock    && b == Shape::Scissor || 
        a == Shape::Paper   && b == Shape::Rock ||
        a == Shape::Scissor && b == Shape::Paper)
    {
        return WIN_SCORE;
    }
    return 0u;
}

std::vector<std::pair<Shape, Result>> get_predictions_2(const std::string &file_path)
{
    std::vector<std::pair<Shape, Result>> prediction_pair_vec;
    std::vector<std::string> pred_vec = read_string_vec_from_file(file_path);

    for (const auto &pred_pair : pred_vec)
    {
        if (pred_pair.length() == 3u)
        {
            prediction_pair_vec.push_back(std::make_pair(shape_map.at(pred_pair[0]), result_map.at(pred_pair[2])));
        }
    }
    return prediction_pair_vec;
}

std::vector<std::pair<Shape, Shape>> get_predictions(const std::string &file_path)
{
    std::vector<std::pair<Shape, Shape>> prediction_pair_vec;
    std::vector<std::string> pred_vec = read_string_vec_from_file(file_path);

    for (const auto &pred_pair : pred_vec)
    {
        if (pred_pair.length() == 3u)
        {
            prediction_pair_vec.push_back(std::make_pair(shape_map.at(pred_pair[0]), shape_map.at(pred_pair[2])));
        }
    }
    return prediction_pair_vec;
}