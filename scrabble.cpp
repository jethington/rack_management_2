// Solution to r/dailyprogrammer challenge:
// https://www.reddit.com/r/dailyprogrammer/comments/5h40ml/20161207_challenge_294_intermediate_rack/

#include <string>
#include <iostream>
#include <cassert>
#include <tuple>

int value(std::string word);
std::tuple<std::string, int> highest_value(std::string tiles); // TODO: std::vector<char> more accurate... does that make a difference?

const int tile_values[26] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10}; // values of A through Z

int char_to_index(char c) {
    // assuming characters will be lower-case a..z    
    assert((c >= 'a') && (c <= 'z'));
    return(c - 'a');
}

int value(std::string word) {
    int multiplier = 1; // first letter is worth 1x, second letter is worth 2x, etc.
    int total_value = 0;

    for (const char& c: word) {
        int base_value = tile_values[char_to_index(c)];
        total_value += base_value * multiplier;
        multiplier++;
    }
    
    return total_value;
}

std::tuple<std::string, int> highest_value(std::string tiles) {
    return std::make_tuple("test", 0);
}

void run_tests(void) {
    // test 'char_to_index'
    assert(char_to_index('a') == 0);
    assert(char_to_index('z') == 25);    

    // test 'value'
    assert(value("daily") == 31);
    
    // test 'highest_value'
    //assert(highest_value)
}

int main(void) {
    run_tests();    

    return 0;
}
