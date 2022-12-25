#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <type_traits>

template<typename T>
struct Point3D
{
    using type = T;
    T x;
    T y;
    T z;
    Point3D(const T& x1, const T& y1, const T& z1) : x{x1}, y {y1}, z{z1} {};
    Point3D(const std::vector<T>& v);
    Point3D() : x{}, y{}, z{} {};
    bool operator<(const Point3D& other) const;
    bool operator==(const Point3D& other) const;
    T dist_squ() const;
};

template<typename T>
Point3D<T>::Point3D(const std::vector<T>& v)
{
    assert(v.size() == 3u);
    x = v[0];
    y = v[1];
    z = v[2];
}


template<typename T>
T Point3D<T>::dist_squ() const
{
    return x*x + y*y + z*z;
}

template<typename T>
bool Point3D<T>::operator<(const Point3D& other) const
{
    if (this->x < other.x)
    {
        return true;
    }
    if (this->x == other.x && this->y < other.y) 
    {
        return true;
    } 
    if (this->x == other.x && this->y == other.y && this->z < other.z) 
    {
        return true;
    } 
    return false;
}

template<typename T>
bool Point3D<T>::operator==(const Point3D& other) const
{
    return this->x == other.x && this->y == other.y && this->z == other.z;
}

template<typename T>
Point3D<T> operator-(const Point3D<T> &p1, const Point3D<T> &p2)
{
    return Point3D<T>{p1.x-p2.x, p1.y-p2.y, p1.z-p2.z};
}

template<typename T>
std::ostream& operator<<(std::ostream &out, const Point3D<T> &p)
{
    return out << p.x << ", " << p.y << ", " << p.z;
}

template<typename T>
T scalar_product(const Point3D<T> &v1, const Point3D<T> &v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template<typename T>
T dist_squ(const Point3D<T> &v1)
{
    return v1.x*v1.x + v1.y*v1.y + v1.z*v1.z;
}

template<typename T>
struct Point 
{
    T x;
    T y;
    Point(const T& x1, const T& y1) : x{x1}, y {y1} {};
    Point() : x{}, y{} {};
    bool operator<(const Point& other) const;
    bool operator==(const Point& other) const;
};

template<typename T>
bool Point<T>::operator<(const Point& other) const
{
    if (this->x < other.x)
    {
        return true;
    }
    if (this->x == other.x && this->y < other.y) 
    {
        return true;
    } 
    return false;
}

template<typename T>
bool Point<T>::operator==(const Point& other) const
{
    return this->x == other.x && this->y == other.y;
}

template<typename T>
Point<T> operator+(const Point<T>& p1, const Point<T>& p2) 
{
    return Point<T>{p1.x+p2.x, p1.y+p2.y};
}

template<typename T>
Point<T> operator-(const Point<T>& p1, const Point<T>& p2) 
{
    return Point<T>{p1.x-p2.x, p1.y-p2.y};
}


/**
 * @brief converts a vector of chars to a number
 * 
 * @param c_vec vector of chars
 * @return T number contained in char vector
 */
template<typename T>
T convert_to_num(const std::vector<char> c_vec)
{
    T num{};
    for (const auto& c : c_vec)
    {
        if (!std::isdigit(static_cast<unsigned char>(c)))
        {
            throw std::invalid_argument("Trying to convert non-digit char to digit!");
        }
        num = num*10 + c - '0';
    }
    return num;
}

/**
 * @brief Converts a string containig multiple numbers to a vector of numbers 
 * 
 * @param in_str string containing multiple numbers
 * @return std::vector<unsigned int> vector where all numbers contained in in_str are inserted
 */
template<typename T>
std::vector<T> parse_string_to_number_vec(const std::string& in_str)
{
    std::vector<T> number_vec{};
    std::vector<char> c_vec{};
    int sign{1}; // is set to -1 if a negative sign '-' is read at the first position
    for (const auto& c : in_str)
    {
        if (c_vec.empty() && c == '-')
        {
            sign = -1;
            continue;
        }
        if (c_vec.empty() && c == '+')
        {
            sign = 1;
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            c_vec.push_back(c);
        }
        else
        {
            if (c_vec.size() > 0u)
            {
                T val = convert_to_num<T>(c_vec);
                number_vec.push_back(sign*val);
            }
            sign = 1;
            c_vec.clear();
        }
    }
    // Convert and insert last number
    if(c_vec.size() > 0)
    {
        number_vec.push_back(sign*convert_to_num<T>(c_vec));
        c_vec.clear();
    }
    return number_vec;
}


/**
 * @brief Converts a string containig at least one number to a number (the first one found)
 * 
 * @param in_str string containing at least one number
 * @return T
 */
template<typename T>
T parse_string_to_number(const std::string& in_str)
{
    std::vector<char> c_vec{};
    int sign{ 1 }; // is set to -1 if a negative sign '-' is read at the first position
    for (const auto& c : in_str)
    {
        if (c_vec.empty() && c == '-')
        {
            sign = -1;
            continue;
        }
        if (c_vec.empty() && c == '+')
        {
            sign = 1;
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            c_vec.push_back(c);
        }
        else
        {
            if (c_vec.size() > 0u)
            {
                T val = convert_to_num<T>(c_vec);
                return sign*val;
            }
            sign = 1;
            c_vec.clear();
        }
    }
    if (c_vec.size() > 0u)
    {
        T val = convert_to_num<T>(c_vec);
        return sign*val;
    }
    else 
    {
        throw std::invalid_argument("No number in given string found!\n"+in_str);
    }
}


/**
 * @brief Searches input string for the next number after position start 
 * and returns the position and length of the next number found (or npos if not found)
 * Signs ('+','-') are only considered if a signed data type T is used!
 * 
 * @tparam T 
 * @param in_str 
 * @param start starting position for the search of next number
 * @param num returned number value
 * @return std::pair<size_t, uint16_t> position of first char of number + number of chars relevant for this number (signs included)  
 */
template<typename T>
std::pair<size_t, uint16_t> parse_next_number_in_str(const std::string& in_str, size_t start, T &num)
{
    std::vector<char> c_vec{};
    int sign{ 1 }; // is set to -1 if a negative sign '-' is read at the first position
    uint16_t num_len{ 0u };
    size_t start_pos{ start };
    for (size_t pos=start; pos<in_str.length(); ++pos)
    {
        char c = in_str.at(pos);
        if (c_vec.empty() && c == '-' && std::is_signed<T>::value)
        {
            sign = -1;
            ++num_len;
            start_pos = pos;
            continue;
        }
        if (c_vec.empty() && c == '+' && std::is_signed<T>::value)
        {
            sign = 1;
            start_pos = pos;
            ++num_len;
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            ++num_len;
            if (c_vec.empty())
            {
                start_pos = pos;                
            }
            c_vec.push_back(c);
        }
        else
        {
            if (c_vec.size() > 0u)
            {
                num = sign * convert_to_num<T>(c_vec);
                return { start_pos, num_len };
            }
            sign = 1;
            c_vec.clear();
        }
    }
    if (c_vec.size() > 0u)
    {
        num = sign * convert_to_num<T>(c_vec);
        return { start_pos, num_len };
    }
    else 
    {
        return { std::string::npos, 0u };
    }
}

/**
 * @brief Reads numbers from a file and interprets each non-consecutive digit as a separate number
 * Each number is appended to the output vector (one-dimensional)
 * 
 * @tparam T 
 * @param file_path 
 * @return std::vector<T> 
 */
template<typename T>
std::vector<T> read_numbers_from_file(const std::string& file_path)
{
    std::vector<T> number_vec{};
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open())
    {
        std::string input_line;
        while(getline(input_file, input_line))
        {  
            std::vector<T> num_in_line = parse_string_to_number_vec<T>(input_line);
            number_vec.insert(number_vec.end(),num_in_line.begin(), num_in_line.end());
        }
        input_file.close();   //close the file object.
    }
    return number_vec;
}

/**
 * @brief Parses a string into a vector of strings. The delimiter is given as second paramter
 * 
 * @param str original string to be parsed
 * @param delimiter delimiter used to extract substrings from str
 * @return std::vector<std::string> vector of substrings
 */
std::vector<std::string> split_string(const std::string& str, const std::string delimiter=" ")
{
    std::vector<std::string> substrings;
    size_t start{0};
    size_t end{0};
    while(end != std::string::npos)
    {
        end = str.find(delimiter, start);
        substrings.push_back(str.substr(start, end-start));
        start = end + delimiter.length();
    }
    return substrings;
}

template<typename T>
std::vector<T> parse_to_single_digits(const std::string& str)
{
    std::vector<T> numbers;
    for (const auto& c : str)
    {
        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            numbers.push_back(static_cast<T>(c-'0'));
        }
    }
    return numbers;
}

/**
 * @brief Reads in a 2D map,containig only numbers without delimiters and parses the map to a 2D vector
 * 
 * @param file_path 
 * @return std::vector<std::vector<T>> 
 */
template<typename T>
std::vector<std::vector<T>> read_2d_vec_from_file(const std::string& file_path)
{
    std::vector<std::vector<T>> number_vec_2d{};
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open()){
        std::string input_line;
        while(getline(input_file, input_line)){  //read data from file object and put it into string.
            number_vec_2d.push_back(parse_to_single_digits<T>(input_line));
        }
        input_file.close();   //close the file object.
    }
    return number_vec_2d;
}

template<>
std::vector<std::vector<char>> read_2d_vec_from_file(const std::string& file_path)
{
    std::vector<std::vector<char>> char_vec_2d{};
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open())
    {
        std::string input_line;
        while(getline(input_file, input_line))
        {  
            char_vec_2d.push_back(std::vector<char>(input_line.begin(), input_line.end()));
        }
        input_file.close();   //close the file object.
    }
    return char_vec_2d;
}


/**
 * @brief Reads in a file and copies content line by line to a vector of strings
 * 
 * @param file_path 
 * @return std::vector<std::string> 
 */
std::vector<std::string> read_string_vec_from_file(const std::string& file_path)
{
    std::vector<std::string> string_vec{};
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    if (input_file.is_open())
    {
        std::string input_line;
        while(getline(input_file, input_line))
        {  
            string_vec.push_back(input_line);
        }
        input_file.close();  
    }
    return string_vec;
}

/**
 * @brief Return the first line of given file
 * 
 * @param file_path 
 * @return std::string first line 
 */
std::string read_string_from_file(const std::string& file_path)
{
    std::fstream input_file;
    input_file.open(file_path,std::ios::in);
    std::string input_line{ "" };
    if (input_file.is_open())
    {
        getline(input_file, input_line);
        input_file.close();   //close the file object.
    }
    return input_line;
}

/**
 * @brief Get the next number written in a string after a substring
 * 
 * @tparam T type of number that is returned
 * @param str input string
 * @param delimiter substring after which the next number is read and returned
 * @return T 
 */
template<typename T>
T get_number_after_str(const std::string& str, const std::string& delimiter)
{
    T num{0};
    int sign{1}; // is set to -1 if a negative sign '-' is read at the first position
    bool num_found{false};
    size_t start = str.find(delimiter, 0u);
    if (start != std::string::npos)
    {
        start += delimiter.length(); 
        if (str[start] == '-')
        {
            sign = -1;
            ++start;
        }
        if (str[start] == '+')
        {
            ++start;
        }
        for (size_t i=start; i < str.length(); ++i)
        {
            if (std::isdigit(static_cast<unsigned char>(str[i])))
            {
                num = num*10 + str[i] - '0';
                num_found = true;
            }
            else
            {
                if (num_found)
                {
                    return sign*num;
                }
            }
        }
    }
    if (!num_found)
    {
        std::string err_str = std::string("No number after substring '" + delimiter + "' found in string:\n") + str;
        throw std::invalid_argument(err_str);
    }
    return sign*num;
}

std::uint64_t bin_str_to_uint(const std::string &bin_str)
{
    std::uint64_t res{0u};
    for (const auto c : bin_str)
    {
        res <<= 1;
        res |= (c == '1');
    }
    return res;
}

std::uint64_t bool_vec_to_uint(const std::vector<bool> &bool_vec)
{
    std::uint64_t res{0u};
    for (const auto c : bool_vec)
    {
        res <<= 1;
        res |= c;
    }
    return res;
}

std::ostream& bold_on(std::ostream& os)
{
    return os << "\e[1m";
}

std::ostream& bold_off(std::ostream& os)
{
    return os << "\e[0m";
}
