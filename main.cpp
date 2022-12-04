#include <iostream>
#include <vector>
#include <string>

#include "1/1_sol.cpp"
#include "2/2_sol.cpp"
#include "3/3_sol.cpp"
#include "4/4_sol.cpp"
// #include "5/5_sol.cpp"
// #include "6/6_sol.cpp"
// #include "7/7_sol.cpp"
// #include "8/8_sol.cpp"
// #include "9/9_sol.cpp"
// #include "10/10_sol.cpp"
// #include "11/11_sol.cpp"
// #include "12/12_sol.cpp"
// #include "13/13_sol.cpp"
// #include "14/14_sol.cpp"
// #include "15/15_sol.cpp"
// #include "16/16_sol.cpp"
// #include "17/17_sol.cpp"
// #include "19/19_sol.cpp"
// #include "20/20_sol.cpp"
// #include "21/21_sol.cpp"

int main(){
    // Day 1
    std::cout << "Max calories of elf: " << day_1_1("1/calories.txt") << std::endl;
    std::cout << "Sum of 3 max calories: " << day_1_2("1/calories.txt") << std::endl;

    // Day 2
    std::cout << "Score with predicted shapes: " << day_2_1("2/strategy_guide.txt") << std::endl;
    std::cout << "Score with correct predicted shapes: " << day_2_2("2/strategy_guide.txt") << std::endl;
    
    //Day 3
    std::cout << "Summed priorities: " << day_3_1("3/item_list.txt") << std::endl;
    std::cout << "Summed badge priorities: " << day_3_2("3/item_list.txt") << std::endl;

    //Day 4
    std::cout << "Number of fully contained sectinos: " << day_4_1("4/section_list.txt") << std::endl;
    std::cout << "Number of overlapping sections: " << day_4_2("4/section_list.txt") << std::endl;

    return 0;
}