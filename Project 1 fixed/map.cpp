#include <iostream>
#include <string>
#include "Map.h"
#include <cassert>
using namespace std;


Map::Map() {
    root = nullptr;
}    // Create an empty map (i.e., one with no key/value pairs)

Map::Map (const Map& rhs){ //copy constructor
    copy(rhs.root, this->root);
}

const Map& Map::operator=(const Map& rhs){ //assignment operator
    if (this == &rhs)
        return *this;
    deleter(root);
    copy(rhs.root, this->root);
    return *this;
}

Map::~Map() { //destructor
    deleter(root);
}


bool Map::empty() const {
    if (root == nullptr) 
        return true;
    return false;
}     // Return true if the map is empty, otherwise false.

int Map::size() const {
    return length(root);
}       // Return the number of key/value pairs in the map.

bool Map::insert(const KeyType& key, const ValueType& value) {
    return add(key, value, root);
}   
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map).

bool Map::update(const KeyType& key, const ValueType& value) {
    return updated(key, value, root);
}   
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    return ins_update(key, value, root);
}   
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // If key is not equal to any key currently in the map then add it and 
    // return true. In fact this function always returns true.

bool Map::erase(const KeyType& key) {
    return remove (key, root);
}
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.

bool Map::contains(const KeyType& key) const {
    return contain(key, root);
}   
    // Return true if key is equal to a key currently in the map, otherwise
    // false.

bool Map::get(const KeyType& key, ValueType& value) const {
    return got(key, value, root);
}   
    // If key is equal to a key currently in the map, set value to the
    // value in the map that that key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.

bool Map::get(int i, KeyType& key, ValueType& value) const { //TODO
    if (i > this->size())
        return false;
    return got3(i, key, value, this->root);
}
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of one of the key/value pairs in the map and return
    // true.  Otherwise, leave the key and value parameters unchanged and
    // return false.  (See below for details about this function.)

void Map::swap(Map& other) {
    Node* temp = this->root;
    this->root = other.root;
    other.root = temp;
    temp = nullptr;
}

void Map::dump() {
    print(this->root);
}

bool combine (const Map& m1, const Map& m2, Map& result) {
    bool res = true;
    KeyType m1_key;
    KeyType m2_key;
    ValueType m1_val;
    ValueType m2_val;
    Map temp = result;

    //empties out result
    int size = temp.size();
    while(size > 0) {
        temp.get(size - 1, m1_key, m1_val);
        temp.erase(m1_key);
        size--;
    }
    
    if (m1.empty() && m2.empty()) {
        return res;
    }

    //adds all of m1 values
    size = m1.size();
    for (int i = 0; i < size; i++) {
        m1.get(i, m1_key, m1_val);
        temp.insert(m1_key, m1_val);
    }

    //checks for dupes then adds m2 values
    size = m2.size();
    for (int i = 0; i < size; i++) {
        m2.get(i, m2_key, m2_val);
        if (!temp.insert(m2_key, m2_val)) {
            m1.get(m2_key, m1_val);
            if (m1_val != m2_val){
                temp.erase(m2_key);
                res = false;
            }
        }
        else {
            temp.insert(m2_key, m2_val);
        }
    }
    result = temp;
    return res;
}

void subtract(const Map& m1, const Map& m2, Map& result) {
    KeyType m1_key;
    KeyType m2_key;
    ValueType m1_val;
    ValueType m2_val;
    Map temp = result;

    //empties out result
    int size = temp.size();
    while(size > 0) {
        temp.get(size - 1, m1_key, m1_val);
        temp.erase(m1_key);
        size--;
    }

    if (m1.empty()) {
        return;
    }

    //adds all of m1 values
    size = m1.size();
    for (int i = 0; i < size; i++) {
        m1.get(i, m1_key, m1_val);
        temp.insert(m1_key, m1_val);
    }

    size = m2.size();
    while (size > 0) {
        m2.get(size - 1, m2_key, m2_val);
        if (!temp.insert(m2_key, m2_val)){
            temp.erase(m2_key);
        }
        size--;
    }
    result = temp;
}