#pragma once
#include <iostream>
#include <string>
using KeyType = std::string;
using ValueType = double;

class Map
{
  private:

    struct Node{
        KeyType key;
        ValueType val;
        Node* right;
        Node* left;
    };
    Node* root;

        //recursive functions for tree:
    
    //remove for assignment
    void deleter(Node* &nodeptr) {
        if (nodeptr == nullptr)
            return;
        deleter(nodeptr->left);
        deleter(nodeptr->right);
        delete nodeptr;
        nodeptr = nullptr;
    }

    //copy constructor
    void copy(Node* ptr, Node* &nodeptr) {
        if (ptr == nullptr) {
            nodeptr = nullptr;
            return;
        }
        nodeptr = new Node;
        nodeptr->key = ptr->key;
        nodeptr->val = ptr->val;
        copy(ptr->left, nodeptr->left);
        copy(ptr->right, nodeptr->right);
    }

    //insert
    bool add (const KeyType& key, const ValueType& value, Node* &nodeptr) {
        if (nodeptr == nullptr){
            nodeptr = new Node;
            nodeptr->key = key;
            nodeptr->val = value;
            return true;
        }
        if (nodeptr->key == key) {
            return false;
        }
        if (key < nodeptr->key)
            return add(key, value, nodeptr->left);
        if (key > nodeptr->key)
            return add(key, value, nodeptr->right);
        
        return false;
    }

    //update
    bool updated(const KeyType& key, const ValueType& value, Node* &nodeptr) {
        if (nodeptr == nullptr)
            return false;
        if (key == nodeptr->key) {
            nodeptr->val = value;
            return true;
        }
        if (key < nodeptr->key) 
            return updated(key, value, nodeptr->left);
        if (key > nodeptr->key) 
            return updated(key, value, nodeptr->right);
        
        return false;
    }

    //size
    int length(Node* nodeptr) const{
        if (nodeptr == nullptr)
            return 0;
        int size;
        size = 1 + length(nodeptr->right) + length(nodeptr->left);
        return size;
    }

    //contain
    bool contain(const KeyType& key, Node* nodeptr) const{
        if (nodeptr == nullptr) 
            return false;
        if (nodeptr->key == key) 
            return true;
        if (key < nodeptr->key)
            return contain(key, nodeptr->left);
        return contain(key, nodeptr->right);
    }

    //insertOrUpdate
    bool ins_update(const KeyType& key, const ValueType& value, Node* &nodeptr) {
        if (nodeptr == nullptr){
            nodeptr = new Node;
            nodeptr->key = key;
            nodeptr->val = value;
            return true;
        }
        if (nodeptr->key == key) {
            nodeptr->val = value;
            return true;
        }
        if (key < nodeptr->key)
            return ins_update(key, value, nodeptr->left);
        if (key > nodeptr->key)
            return ins_update(key, value, nodeptr->right);
        
        return true;
    }

    //helper for remove
    Node* max(Node* nodeptr) { // call with nodeptr->left to get max of left side
        if (nodeptr == nullptr)
            return nullptr;
        if (nodeptr->right == nullptr)
            return nodeptr;
        return max(nodeptr->right);
    }

    //erase
    bool remove (const KeyType& key, Node* &nodeptr) {
        if (nodeptr == nullptr){
            return false;
        }
        if (key == nodeptr->key) {
            //2 children
            if (nodeptr->left != nullptr && nodeptr->right != nullptr) {
                Node* temp = max(nodeptr->left);
                nodeptr->key = temp->key;
                nodeptr->val = temp->val;
                remove(temp->key, nodeptr->left);
                temp = nullptr;
            }
            //no children
            if (nodeptr->left == nullptr && nodeptr->right == nullptr) {
                delete nodeptr;
                nodeptr = nullptr;
                return true;
            }
            //1 child right
            if (nodeptr->left == nullptr) {
                Node* p = nodeptr;
                nodeptr = nodeptr->right;
                delete p;
                p = nullptr;
            }
            //1 child left
            if (nodeptr->right == nullptr) {
                Node* p = nodeptr;
                nodeptr = nodeptr->left;
                delete p;
                p = nullptr;
            }
            return true;
        }
        if (nodeptr->key < key){
            return remove(key, nodeptr->right);
        }
        if (nodeptr->key > key){
            return remove(key, nodeptr->left);
        }
        return false;
    }

    //print
    void print(Node* nodeptr) {
        if (nodeptr == nullptr) {
            return;
        }
        print(nodeptr->left);
        std::cout << "key: " << nodeptr->key << " value: " << nodeptr->val << std::endl;
        print(nodeptr->right);
    }

    //get
    bool got(const KeyType& key, ValueType& value, Node* nodeptr) const{
        if (nodeptr == nullptr)
            return false;
        if (nodeptr->key == key){
            value = nodeptr->val;
            return true;
        }
        if (key < nodeptr->key)
            return got(key, value, nodeptr->left);
        if (key > nodeptr->key)
            return got(key, value, nodeptr->right);
        
        return false;
    }

    //get 3 parameters
    bool got3(int i, KeyType& key, ValueType& value, Node* nodeptr) const{
        if (nodeptr == nullptr)
            return false;
        if (i == 0){
            value = nodeptr->val;
            key = nodeptr->key;
            return true;
        }
        if (i <= length(nodeptr->left))
            return got3(i - 1, key, value, nodeptr->left);
        return got3(i - length(nodeptr->left) - 1, key, value, nodeptr->right);
    }


  public:
    Map();              // Create an empty map (i.e., one with no key/value pairs)

    Map(const Map& rhs);                    // Copy Constructor

    const Map& operator=(const Map& rhs);   //assignment operator

    ~Map();             // Destructor

    bool empty() const; // Return true if the map is empty, otherwise false.

    int size() const;   // Return the number of key/value pairs in the map.

    bool insert(const KeyType& key, const ValueType& value);
      // If key is not equal to any key currently in the map, and if the
      // key/value pair can be added to the map, then do so and return true.
      // Otherwise, make no change to the map and return false (indicating
      // that either the key is already in the map).

    bool update(const KeyType& key, const ValueType& value);
      // If key is equal to a key currently in the map, then make that key no
      // longer map to the value it currently maps to, but instead map to
      // the value of the second parameter; return true in this case.
      // Otherwise, make no change to the map and return false.

    bool insertOrUpdate(const KeyType& key, const ValueType& value);
      // If key is equal to a key currently in the map, then make that key no
      // longer map to the value it currently maps to, but instead map to
      // the value of the second parameter; return true in this case.
      // If key is not equal to any key currently in the map then add it and 
      // return true. In fact this function always returns true.

    bool erase(const KeyType& key);
      // If key is equal to a key currently in the map, remove the key/value
      // pair with that key from the map and return true.  Otherwise, make
      // no change to the map and return false.

    bool contains(const KeyType& key) const;
      // Return true if key is equal to a key currently in the map, otherwise
      // false.

    bool get(const KeyType& key, ValueType& value) const;
      // If key is equal to a key currently in the map, set value to the
      // value in the map that that key maps to, and return true.  Otherwise,
      // make no change to the value parameter of this function and return
      // false.

    bool get(int i, KeyType& key, ValueType& value) const;
      // If 0 <= i < size(), copy into the key and value parameters the
      // key and value of one of the key/value pairs in the map and return
      // true.  Otherwise, leave the key and value parameters unchanged and
      // return false.  (See below for details about this function.)

    void swap(Map& other);
      // Exchange the contents of this map with the other one.
    
    void dump();
};

bool combine(const Map& m1, const Map& m2, Map& result);

void subtract(const Map& m1, const Map& m2, Map& result);