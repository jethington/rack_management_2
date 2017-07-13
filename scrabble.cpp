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
	int score_so_far;

	Node(int score_so_far) : score_so_far(score_so_far), done(false) {
		for (int i = 0; i < 26; i++) {
			next[i] = nullptr;
		}
	}
	Node() : Node(0) {
	}
	~Node() {
		// note: not bothering with cleanup because Nodes are never deleted during the life of the program
	}
	void add_string(std::string& to_add, int letter_index);
	void words(std::string tiles, std::string so_far, std::vector<std::string>& results);
};

const int tile_values[26] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10}; // values of A through Z

Node tree;

//std::tuple(std::string, int) Node::best_score(std::string tiles_used, std::string tiles_left) {
//
//}

// note: single head node does not represent a letter/tile
//       so the word 'a' will have 2 nodes
void Node::add_string(std::string& to_add, int letter_index) {
	if (to_add.length() == letter_index) {
		done = true;
		return;
	}
	else {
		char c = to_add[letter_index];
		letter_index++;
		int next_node_index = char_to_index(c);
		if (next[next_node_index] == nullptr) {
			int new_score = score_so_far + tile_values[next_node_index] * letter_index; // TODO: these names are kind of confusing
			next[next_node_index] = new Node(new_score);
		}
		next[next_node_index]->add_string(to_add, letter_index);
	}
}

void Node::words(std::string tiles, std::string so_far, std::vector<std::string>& results) {
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
				std::string tiles_copy;
				tiles_copy.append(tiles, 0, i);
				tiles_copy.append(tiles, i, tiles.length());
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
    t.add_string(std::string("foo"), 0);
    t.add_string(std::string("bar"), 0);
    t.add_string(std::string("baz"), 0);

    assert(t.next[0] == nullptr); // a
    assert(t.next[1] != nullptr); // b
    assert(t.next[2] == nullptr); // c
    assert(t.next[5] != nullptr); // f

	assert(t.score_so_far == 0); // head Node should have a score of 0, no letters so far

    Node* f = t.next[5];
    assert(f->next[14] != nullptr); // f - o
	assert(f->score_so_far == tile_values[5]);

	Node* b = t.next[1];
    assert(b->next[0] != nullptr); // b - a
	assert(b->score_so_far == 3); // b

	Node* a = b->next[0];
	assert(a->score_so_far == (3*1 + 1*2)); // ba
	Node* r = a->next[17];
	assert(r->score_so_far == (3*1 + 1*2 + 1*3)); // bar

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
        tree.add_string(word, 0);
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
