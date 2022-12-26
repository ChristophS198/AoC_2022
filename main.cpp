#include <iostream>
#include <vector>
#include <string>

// #include "1/1_sol.cpp"
// #include "2/2_sol.cpp"
// #include "3/3_sol.cpp"
// #include "4/4_sol.cpp"
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
// #include "18/18_sol.cpp"
// #include "19/19_sol.cpp"
// #include "20/20_sol.cpp"
// #include "21/21_sol.cpp"
// #include "22/22_sol.cpp"
#include "23/23_sol.cpp"
// #include "24/24_sol.cpp"
// #include "25/25_sol.cpp"

int main(){
/*
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

    //Day 5
    std::cout << "Top crates using CrateMover 9000: " << day_5_1("5/crate_list.txt") << std::endl;
    std::cout << "Top crates using CrateMover 9001: " << day_5_2("5/crate_list.txt") << std::endl;

    //Day 6
    std::cout << "Marker position: " << day_6_1("6/datastream.txt") << std::endl;
    std::cout << "Message position: " << day_6_2("6/datastream.txt") << std::endl;

    //Day 7
    std::cout << "Total sum of small folder sizes: " << day_7_1("7/terminal_output.txt") << std::endl;
    std::cout << "Size of best file to delete: " << day_7_2("7/terminal_output.txt") << std::endl;

    //Day 8
    std::cout << "Visible trees from outside: " << day_8_1("8/tree.txt") << std::endl;
    std::cout << "Size of best file to delete: " << day_8_2("8/tree.txt") << std::endl;

    //Day 9
    std::cout << "Day 9_1: " << day_9_1("9/motion_moves.txt") << std::endl;
    std::cout << "Day 9_2: " << day_9_2("9/motion_moves.txt") << std::endl;

    //Day 10
    std::cout << "Day 10_1: " << day_10_1("10/input.txt") << std::endl;
    day_10_2("10/input.txt", "10/crt_bitmap_out.txt"); 

    //Day 11
    std::cout << "Day 11_1: " << day_11_1("11/input.txt") << std::endl;
    std::cout << "Day 11_2: " << day_11_2("11/input.txt") << std::endl;

    //Day 12
    std::cout << "Day 12_1: " << day_12_1("12/input.txt") << std::endl;
    std::cout << "Day 12_2: " << day_12_2("12/input.txt") << std::endl;

    //Day 13
    std::cout << "Day 13_1: " << day_13_1("13/input.txt") << std::endl;
    std::cout << "Day 13_2: " << day_13_2("13/input.txt") << std::endl;

    //Day 14
    std::cout << "Day 14_1: " << day_14_1("14/input.txt") << std::endl;
    std::cout << "Day 14_2: " << day_14_2("14/input.txt") << std::endl;

    //Day 15
    std::cout << "Day 15_1: " << day_15_1("15/input.txt") << std::endl;
    std::cout << "Day 15_2: " << day_15_2("15/input.txt") << std::endl;

    //Day 16
    std::cout << "Day 16_1: " << day_16_1("16/input.txt") << std::endl;
    std::cout << "Day 16_2: " << day_16_2("16/input.txt") << std::endl;

    //Day 17
    std::cout << "Day 17_1: " << day_17_1("17/input.txt") << std::endl;
    std::cout << "Day 17_2: " << day_17_2("17/input.txt") << std::endl;

    //Day 18
    std::cout << "Day 18_1: " << day_18_1("18/input.txt") << std::endl;
    std::cout << "Day 18_2: " << day_18_2("18/input.txt") << std::endl;

    //Day 19
    std::cout << "Day 19_1: " << day_19_1("19/input.txt") << std::endl;
    std::cout << "Day 19_2: " << day_19_2("19/input.txt") << std::endl; // should finish in about 20s

    // Day 20
    std::cout << "Day 20_1: " << day_20_1("20/input.txt") << std::endl;
    std::cout << "Day 20_2: " << day_20_2("20/input.txt") << std::endl;

    // Day 21
    std::cout << "Day 21_1: " << day_21_1("21/input.txt") << std::endl;
    std::cout << "Day 21_2: " << day_21_2("21/input.txt") << std::endl;

    // Day 22
    std::cout << "Day 22_1: " << day_22_1("22/input.txt") << std::endl;
    std::cout << "Day 22_2: " << day_22_2("22/input.txt") << std::endl;
*/
    // Day 23
    std::cout << "Day 23_1: " << day_23_1("23/input.txt") << std::endl;
    std::cout << "Day 23_2: " << day_23_2("23/input.txt") << std::endl;
/*
    // Day 24
    std::cout << "Day 24_1: " << day_24_1("24/input.txt") << std::endl;
    std::cout << "Day 24_2: " << day_24_2("24/input.txt") << std::endl;

    // Day 25
    std::cout << "Day 25_1: " << day_25_1("25/input.txt") << std::endl;
    std::cout << "Day 25_2: " << day_25_2("25/input.txt") << std::endl;

    */
    return 0;
}