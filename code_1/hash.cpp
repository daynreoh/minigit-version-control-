// CPP program to implement hashing with chaining
#include<iostream>
#include "hash.hpp"


using namespace std;

HashNode* HashTable::createNode(string key, HashNode* next)
{
    HashNode* nw = NULL;
    nw->key = key;
    nw->next = next;
    return nw;
}

HashTable::HashTable(int bsize)
{
    tableSize = bsize;
    table = new HashNode*[tableSize];
    for(int i = 0; i < bsize; i++)
    {
        table[i] = nullptr;
    }
}

//function to calculate hash function
unsigned int HashTable::hashFunction(string s)
{
    int sum = 0;
    int index = 0;
    for(string::size_type i = 0; i < s.length(); i++)
    {
        sum += s[i];
    }
    
    index = sum % tableSize;
    return index;
}

// TODO Complete this function
//function to search
HashNode* HashTable::searchItem(string key)
{
    int index = hashFunction(key);
    HashNode* temp = table[index];
    
    while(temp)
    {
        if(temp->key == key)
        {
            return temp;
        }
        temp = temp->next;
    }

    return nullptr;
    
}

//TODO Complete this function
//function to insert
// int cNum = chainNumber? 
void HashTable::insertItem(string key, int commitNumber)
{
    int index;
    HashNode* currNode = searchItem(key);
    if(currNode != nullptr)
    {
        currNode->commitNums.push_back(commitNumber);
    }
    else 
    {
        HashNode* newNode = new HashNode;
        newNode->key = key;
        newNode->commitNums.push_back(commitNumber);
        
        index = hashFunction(key);
        table[index] = newNode;
    }
}


// function to display hash table //
/* assume the table size is 5. For each bucket it will show the 
** the string key and the commit number (separated by comma) within parenthesis
** e.g. the key is science and commit numbers are 1 and 4. The key science
** is hashed to position 0. So the print format will be-

0|| science(1,4,)
1|| 
2|| 
3|| 
4|| difficult(3,)-->fun(2,)-->computer(0,)

*/
void HashTable::printTable()
{
    for(int i = 0; i < 5; i++)
    {
        cout << table[i] << ",";
    }
    cout << endl;
}
