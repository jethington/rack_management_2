// Solution to r/dailyprogrammer challenge:
// https://www.reddit.com/r/dailyprogrammer/comments/5h40ml/20161207_challenge_294_intermediate_rack/

#include <string>
#include <iostream>
#include <cassert>
#include <tuple>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_set>

int value(std::string word);
std::tuple<std::string, int> highest_value(std::string tiles);
std::vector<std::string> generate_permutations(std::string tiles);
void generate_permutations_helper(std::string tiles_left, std::string permutation, std::vector<std::string>& results);

const int tile_values[26] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10}; // values of A through Z

std::unordered_set<std::string> words_set;

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

// TODO: rename these functions
std::vector<std::string> generate_permutations(std::string tiles) {
    std::vector<std::string> results;
    generate_permutations_helper(tiles, "", results);
    return results;
}

void generate_permutations_helper(std::string tiles_left, std::string permutation, std::vector<std::string>& results) {   
    for (int i = 0; i < tiles_left.length(); i++) {
        char c = tiles_left[i];
        std::string tiles_left_copy = tiles_left;
        std::string permutation_copy = permutation;
  
        std::swap(tiles_left_copy[i], tiles_left_copy.back());
        tiles_left_copy.pop_back();
        permutation_copy.push_back(c);

        if (words_set.count(permutation_copy) != 0) {
            results.push_back(permutation_copy); // this permutation is in the list of english words, so add it to the results vector
        }

        if (tiles_left_copy.length() == 0) {            
            // done, just return
        }
        else {
            generate_permutations_helper(tiles_left_copy, permutation_copy, results);
        }
    }
}

std::tuple<std::string, int> highest_value(std::string tiles) {
    // brute force approach

    std::vector<std::string> permutations = generate_permutations(tiles);

    int highest = 0;
    std::string best;
    for (const std::string& s: permutations) {
        if (value(s) > highest) {
            best = s;
            highest = value(s);
        }
    }

    return std::make_tuple(best, highest);
}

void run_tests(void) {
    // test 'char_to_index'
    assert(char_to_index('a') == 0);
    assert(char_to_index('z') == 25);    

    // test 'value'
    assert(value("daily") == 31);
    
    // test 'generate_permutations'
    // TODO: need to modify testing strategy, this currently fails due to word list
    /*std::vector<std::string> permutations = generate_permutations("abc");
    assert(permutations.size() == 6);
    assert(permutations[0] == "acb");
    assert(permutations[1] == "abc");
    assert(permutations[2] == "bac");
    assert(permutations[3] == "bca");
    assert(permutations[4] == "cab");
    assert(permutations[5] == "cba");*/

    // test 'highest_value'
    // Note: this is pretty slow, about 6-10 seconds with -O2
    assert(highest_value("iogsvooely") == std::make_tuple("oology", 44));
    assert(highest_value("seevurtfci") == std::make_tuple("service", 52));
    assert(highest_value("vepredequi") == std::make_tuple("reequip", 78));
}

int main(void) {
    // read word list into a hash set
        
    std::ifstream in_file("enable1.txt"); 
    std::string word;
    while (in_file >> word) {
        words_set.insert(word);
    }

    //run_tests();

    std::string s;
    int i;
    std::tie(s, i) = highest_value("umnyeoumcp");
    std::cout << s << " " << i << std::endl;
    std::tie(s, i) = highest_value("orhvtudmcz");
    std::cout << s << " " << i << std::endl;
    std::tie(s, i) = highest_value("fyilnprtia");
    std::cout << s << " " << i << std::endl;

    // prints:

    // eponym 52
    // vouch 41
    // nitrify 67

    return 0;
}
