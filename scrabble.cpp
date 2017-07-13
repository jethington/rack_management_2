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
std::tuple<std::string, int> highest_value_2(std::string tiles);

struct Node {
	Node* next[26];
	bool done;
	int score_so_far;
	int highest_this_branch;

	Node(int score_so_far) : score_so_far(score_so_far), done(false), highest_this_branch(0) {
		for (int i = 0; i < 26; i++) {
			next[i] = nullptr;
		}
	}
	Node() : Node(0) {
	}
	~Node() {
		// note: not bothering with cleanup because Nodes are never deleted during the life of the program
	}
	int add_string(std::string& to_add, int letter_index);
	void words(std::string tiles, std::string so_far, std::vector<std::string>& results);
	std::tuple<std::string, int> highest_value_helper(std::string tiles, std::string so_far, int highest_found);
};

const int tile_values[26] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10}; // values of A through Z

Node tree;

//std::tuple(std::string, int) Node::best_score(std::string tiles_used, std::string tiles_left) {
//
//}

// note: single head node does not represent a letter/tile
//       so the word 'a' will have 2 nodes
int Node::add_string(std::string& to_add, int letter_index) {
	if (to_add.length() == letter_index) {
		done = true;
		highest_this_branch = score_so_far;
		return highest_this_branch;
	}
	else {
		char c = to_add[letter_index];
		letter_index++;
		int next_node_index = char_to_index(c);
		if (next[next_node_index] == nullptr) {
			int new_score = score_so_far + tile_values[next_node_index] * letter_index; // TODO: these names are kind of confusing
			next[next_node_index] = new Node(new_score);
		}
		int highest_in_new_branch = next[next_node_index]->add_string(to_add, letter_index);
		if (highest_in_new_branch > highest_this_branch) {
			highest_this_branch = highest_in_new_branch;
		}
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
	std::vector<std::string> results;
	tree.words(tiles, "", results);

	int highest = 0;
	std::string best;
	for (const std::string& s : results) {
		if (value(s) > highest) {
			best = s;
			highest = value(s);
		}
	}

	return std::make_tuple(best, highest);
}

std::tuple<std::string, int> highest_value_2(std::string tiles) {
	return tree.highest_value_helper(tiles, "", 0);
}

std::tuple<std::string, int> Node::highest_value_helper(std::string tiles, std::string so_far, int highest_found) {
	int highest = 0;
	std::string best;

	if (done) {
		best = so_far;
		highest = score_so_far;
	}

	if (tiles.length() == 0) {
		// do nothing, this branch is done
	}
	else {
		for (int i = 0; i < tiles.length(); i++) {
			char c = tiles[i];
			int index = char_to_index(c);
			if (next[index] != nullptr) {
				if (next[index]->highest_this_branch < highest) {
					continue; // skip this branch, nothing good enough
					// TODO: pass this down as well
				}
				
				std::string tiles_copy;
				tiles_copy.append(tiles, 0, i);
				tiles_copy.append(tiles, i, tiles.length());
				std::string so_far_copy = so_far;
				tiles_copy.erase(i, 1);
				so_far_copy.push_back(c);
				
				std::string s;
				int score;
				std::tie(s, score) = next[index]->highest_value_helper(tiles_copy, so_far_copy, highest);
				if (score > highest) {
					highest = score;
					best = s;
				}
			}
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
    t.add_string(std::string("foo"), 0); // value: 9
    t.add_string(std::string("bar"), 0); // value: 8
    t.add_string(std::string("baz"), 0); // value: 35

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

	assert(t.highest_this_branch == 35);
	assert(f->highest_this_branch == 9);

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

	// test 'highest_value_2'
	assert(highest_value_2("iogsvooely") == std::make_tuple("oology", 44));
	assert(highest_value_2("seevurtfci") == std::make_tuple("service", 52));
	assert(highest_value_2("vepredequi") == std::make_tuple("reequip", 78));
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
