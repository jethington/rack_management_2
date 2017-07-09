// Solution to r/dailyprogrammer challenge:
// https://www.reddit.com/r/dailyprogrammer/comments/5h40ml/20161207_challenge_294_intermediate_rack/

#include <string>
#include <iostream>
#include <cassert>
#include <tuple>

int value(std::string word);

// JCE: needs to be std::tuple(string, int)
int highest_value(std::string tiles); // TODO: std::vector<char> more accurate... does that make a difference?

const int tile_values[26] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10}; // values of A through Z

void run_tests(void) {
    // test 'value'
    assert(value("daily") == 31);
    
    // test 'highest_value'
    assert(highest_value)
}