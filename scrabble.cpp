// Solution to r/dailyprogrammer challenge:
// https://www.reddit.com/r/dailyprogrammer/comments/5h40ml/20161207_challenge_294_intermediate_rack/

#include <string>
#include <iostream>
#include <cassert>
#include <tuple>
#include <vector>
#include <algorithm>
#include <fstream>

int value(std::string word);
int char_to_index(char c);
std::tuple<std::string, int> highest_value(std::string tiles);

struct Node {
	Node* next[26];
	bool done;

	Node() {
		done = false;
		for (int i = 0; i < 26; i++) {
			next[i] = nullptr;
		}
	}
	~Node() {
		// note: not bothering with cleanup because Nodes are never deleted during the life of the program
		//for (int i = 0; i < 26; i++) {
		//	delete next[i];
		//}
	}
	void add_string(std::string to_add);
	void words(std::string tiles, std::string so_far, std::vector<std::string>& results);

	// TODO: optimizations, add later?
	//int score_so_far;
	//int highest_score_this_branch;
};

const int tile_values[26] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10}; // values of A through Z

Node tree;

//std::tuple(std::string, int) Node::best_score(std::string tiles_used, std::string tiles_left) {
//
//}

// note: single head node does not represent a letter/tile
//       so the word 'a' will have 2 nodes
void Node::add_string(std::string to_add) {
	if (to_add.length() == 0) {
		done = true;
		return;
	}
	else {
		char c = to_add[0];
		to_add.erase(0, 1);
		int index = char_to_index(c);
		if (next[index] == nullptr) {
			next[index] = new Node();
		}
		next[index]->add_string(to_add);
	}
}

void Node::words(std::string tiles, std::string so_far, std::vector<std::string>& results) {
	
	//std::cout << "so_far: " << so_far << std::endl;
	//std::cout << "tiles: " << tiles << std::endl;

	if (done) {
		results.push_back(so_far);
	}

	if (tiles.length() == 0) {
		return; // this branch is done
	}
	else {
		for (int i = 0; i < tiles.length(); i++) {
			char c = tiles[i];
			int index = char_to_index(c);
			if (next[index] != nullptr) {
				std::string tiles_copy = tiles;
				std::string so_far_copy = so_far;
				tiles_copy.erase(i, 1);
				so_far_copy.push_back(c);
				next[index]->words(tiles_copy, so_far_copy, results);
			}
		}
	}
}

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
    // use tree instead
    std::vector<std::string> results;
    tree.words(tiles, "", results);
    
    int highest = 0;
    std::string best;
    for (const std::string& s: results) {
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

    // test 'Node::add_string'
    Node t;
    t.add_string("foo");
    t.add_string("bar");
    t.add_string("baz");

    assert(t.next[0] == nullptr); // a
    assert(t.next[1] != nullptr); // b
    assert(t.next[2] == nullptr); // c
    assert(t.next[5] != nullptr); // f

    Node* f = t.next[5];
    assert(f->next[14] != nullptr); // f - o
    Node* b = t.next[1];
    assert(b->next[0] != nullptr); // b - a

    // test 'Node::words'
    std::vector<std::string> results;
    t.words("barwxyz", "", results);
    assert(results.size() == 2);
    assert(results[0] == "bar");
    assert(results[1] == "baz");
	
    // test 'highest_value'
    assert(highest_value("iogsvooely") == std::make_tuple("oology", 44));
    assert(highest_value("seevurtfci") == std::make_tuple("service", 52));
    assert(highest_value("vepredequi") == std::make_tuple("reequip", 78));
}

int main(void) {
   
    // read word list into a tree   
    std::ifstream in_file("enable1.txt"); 
    std::string word;
    while (in_file >> word) {
        tree.add_string(word);
    }

    run_tests();

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
