// CPP program to implement hashing with chaining
#include<iostream>
#include "hash.hpp"


using namespace std;

HashNode* HashTable::createNode(string key, HashNode* next)
{
    HashNode* nw = new HashNode;
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
    return s.size() % tableSize;
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
bool HashTable::insertItem(string key, int commitNumber)
{
    int index = hashFunction(key);
    HashNode* currNode = searchItem(key);
    if(currNode != nullptr)
    {
        currNode->commitNums.push_back(commitNumber);
        return true;
    }
    else 
    {
        HashNode* newNode = createNode(key, table[index]);

        newNode->commitNums.push_back(commitNumber);
        table[index] = newNode;
                                       
        return true;
    }
    return false;
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
