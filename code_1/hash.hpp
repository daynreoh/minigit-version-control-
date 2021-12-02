#ifndef HASH_HPP
#define HASH_HPP

#include <string>
#include <vector>


using namespace std;

struct  HashNode //hash node datastructure
{
    string key;
    vector<int> commitNums;
    struct HashNode* next;
};

class HashTable
{
    int tableSize;  // No. of buckets (linked lists)

    // Pointer to an array containing buckets
     HashNode* *table;

     HashNode* createNode(string key, HashNode* next);
public:
    HashTable(int bsize);  // Constructor

    // inserts a key into hash table
    void insertItem(string key, int commitNumber);

    // hash function to map values to key
    unsigned int hashFunction(string key);

    void printTable();

    HashNode* searchItem(string key);
};

#endif
