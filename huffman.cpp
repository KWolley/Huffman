// huffman.cpp
//

#include "huffman.h"
#include <iostream>
using namespace std;

// create_leaf initializes a freq_info from heap memory and returns a
// pointer to it. the returned object's symbol and count fields are
// set to those provided; left and right child pointers are NULL, and
// it is flagges as a leaf node.
freq_info* create_leaf(char symbol, int count) {
  // returns pointer to 
  // assign input values into {} object init 
  freq_info* new_f = new freq_info();
  new_f -> symbol = symbol;
  new_f -> count = count;
  new_f -> left = nullptr;
  new_f -> right = nullptr;
  new_f -> is_leaf = true;
  return new_f;
}

// combine_nodes initializes a freq_info from the heap and returns a
// pointer to it. The new node has left and right pointers set to the
// values given. The symbol data for this node is undefined (since it
// is an internal node), and is_leaf is false. DECISION NODE
freq_info* combine_nodes(freq_info* left, freq_info* right) {
  freq_info* new_n = new freq_info();
  new_n -> symbol = '$';
  new_n -> count = left->count + right->count;
  new_n -> left = left;
  new_n -> right = right;
  new_n -> is_leaf = false;
  return new_n;
}

// increment lookup will add one to the value associated with the
// symbol, or if the symbol is not yet a key in the map, it will
// create a mapping lookup[symbol] = 1.
// take in a map (may be empty) - find place where char should go
void increment_lookup(map<char, int>& lookup, char symbol) {
  // if key does not exist, map will create default value - 0
  lookup[symbol]++;
  return;
}

// load_queue turns each key/value pair in the lookup table into a
// freq_info pointer, and pushes it into the provided priority queue
// (use q.push(node)).
void load_queue(const map<char, int>& lookup, tree_queue& q) {
  // const - value will not change   
  // auto& - let compiler determine type q/o 
  // & kvp not used everytime
  for ( const auto& kvp : lookup){
    q.push(create_leaf(kvp.first, kvp.second));
  }
}

freq_info* build_tree(tree_queue& q) {
  while(q.size() > 1){
    // take two top 
    const auto left = q.top();
    q.pop();
    const auto right = q.top(); // type is freq_info* left
    q.pop();
    // combined and put back into node
    q.push(combine_nodes(left, right));
    // priority queue will do sorting for us on entry back into qs
  }
  return q.top();
}

// recursive fuction to traverse through map until leaf 
void build_encoding_help(const freq_info* node, map<char, string>* table, const string& path){
  // helper recursive for encoding build function
  // if node is a leaf
  if (node->is_leaf){
    table->emplace(node ->symbol, path);  // sets key node symnbol with path
  } else{ // recurse through until leaf is found
  build_encoding_help(node->left, table, path + LEFT_CHAR);
  build_encoding_help(node ->right,table,path + RIGHT_CHAR);
  }
  return;
}

map<char, string> build_encoding_table(freq_info* root) {
  map<char,string> table;
  build_encoding_help(root,&table,"");
  return table;
}

string encode(map<char, string> enc_table, string input) {
  string output;
  for (const auto& character : input){
    string character_encoding = enc_table[character];
    output += character_encoding;
  }
  return output;
}

string decode(freq_info* root, string input) {
  string output;
  freq_info* cursor = root;

  for (const auto& decision : input){

    if(decision == '.'){
    cursor = cursor -> left;
  } else {
    cursor = cursor -> right;
  }
  if (cursor -> is_leaf){
    output += cursor -> symbol;
    cursor = root;
  }
    //curosr = decision == "." ? curosr -> left : cursor -> right;
  }
  return output;
}
