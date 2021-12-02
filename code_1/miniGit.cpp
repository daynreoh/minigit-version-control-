#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "miniGit.hpp"
#include <vector>

MiniGit::MiniGit() {
    fs::remove_all(".minigit");
    fs::create_directory(".minigit");
}

MiniGit::~MiniGit() {   
    // Any postprocessing that may be required
    // this includes deleting the hash table and Linked Lists

}

void MiniGit::init(int hashtablesize) {
    
    BranchNode* repoHead = new BranchNode;
    commitHead = repoHead;
    
    commitHead->fileHead = nullptr;
    commitHead->next = nullptr;
    commitHead->previous = nullptr;
    commitHead->commitID = 0;
    
    HashTable* repoTable = new HashTable(5);
    ht = repoTable;
    
}

void MiniGit::add(string fileName) {
    
    FileNode* newFile = new FileNode;
    newFile->name = fileName;
    newFile->next = nullptr;
    newFile->version = 00;
    
    FileNode* crawler = commitHead->fileHead;
    
    if(commitHead->fileHead != nullptr)
    {
        while(crawler->next != nullptr)
        {
            crawler = crawler->next;
        }

        crawler->next = newFile;
    }
    else 
    {
        commitHead->fileHead = newFile;
    }
}

bool MiniGit::searchDirectory(string fileName)
{
    // how do I search a a directory?
    string path = "/home/jovyan/final-project/dummy/" + fileName;
    return fs::exists(path);
}


bool MiniGit::traverseSLL(string fileName)
{
    FileNode* crawler = commitHead->fileHead;
    bool inSLL = false;
    
    while(crawler != nullptr)
    {
        if(crawler->name == fileName)
        {
            inSLL = true;
        }
        crawler = crawler->next;
    }
    
    return inSLL;
}

void MiniGit::rm(string fileName) {
    
    // ACTUAL: f1.txt --> nullptr
    // crawler = f1.txt
    // previous = nullptr
    
    
    // fileName = f3.txt
    // f1.txt --> f2.txt --> f3.txt --> f4.txt --> nullptr
    // previous = f2.txt
    // crawler = f3.txt
    FileNode* crawler = commitHead->fileHead;
    FileNode* previous = nullptr;
    bool isDeleted = false;
    
    while(crawler != nullptr && !isDeleted)
    {
        // be careful, this might not work because the filenode name will have numbers in it 
        if(crawler->name == fileName)
        {
            if(crawler == commitHead->fileHead)
            {
                delete crawler;
            }
            else 
            {
                previous->next = crawler->next;
                crawler->next = nullptr;
                delete crawler;
            }
            isDeleted = true;
        }
        else 
        {
            previous = crawler;
            crawler = crawler->next;
        }
    }
}



void MiniGit::printSearchTable()
{
     ht->printTable();
}


void MiniGit::search(string key)
{
}



string MiniGit::commit(string msg) {
    
    
    
    
    
    
    return " "; //should return the commitID of the commited DLL node
}

bool MiniGit::fileInDirectory(string fileName)
{
    //traverse SLL 
    FileNode* crawler = commitHead->fileHead;
    string versionFile;
    string fileName2;
    
    // should look like: ./minigit/f1.txt__00
    
    // this traverses through the current directory
    while(crawler != nullptr)
    {
        fileName2 = "./minigit/" + crawler->name;    

        // this should just test the input file name not the version file.. so like f1.txt
        // tests to see if the file exists in .minigit
        if(fs::exists(fileName2))
        {
            // check whether the file has changed in version
            // do this by opening the file, and reading line by line to make sure every line matches up
            // versionFile = “./minigit/“ + crawler->name + ”__” + to_string(crawler->version);

            if(filesAreSame(fileName, fileName2))
            {
                // do nothing because the file version is updated
            }
            else 
            {
                // copy file from current directory to .minigit directory
            }
        }
        else
        {
            // copy the file from current directory to minigit
        }
    }
}

 

bool MiniGit::filesAreSame(string fileName, string directoryFileName)
{
    ifstream currentFile (fileName);
    ifstream directoryFile (directoryFileName);
    bool isSame = false;
    
    if(currentFile.is_open() && directoryFile.is_open())
    {
        isSame = true;
        string currentLine;
        string directoryLine;
        
        while(getline(currentFile, currentLine) && getline(directoryFile, directoryLine))
        {            
            if(currentLine != directoryLine)
            {
                isSame = false;
            }
        }
    }
    
    return isSame;
    
}

bool MiniGit::isGoodMessage(string commitMessage)
{
    // Ex: "computer fun science j"
    // length: 12
    
    int whiteSpace = 0;
    bool previousWhite = true;
    
    for(unsigned int i = 0; i < commitMessage.length(); i++)
    {
        if(commitMessage.at(i) == ' ' && !previousWhite)
        {
            whiteSpace++;
            previousWhite = true;
        }
        else if(commitMessage.at(i) != ' ')
        {
            previousWhite = false;
        }
    }
    
    if(whiteSpace > 2)
    {
        return false;
    }
    else 
    {
        return true;
    }
    //return (whiteSpace > 2);
}

bool MiniGit::hasSameCommitMessage(string commitMessage)
{
    // traverse through the DLL and check to see if the commit message
    // of any previous commit is the exact same
    
    bool isDuplicate = false;
    BranchNode* crawler = commitHead;
    
    while(crawler != nullptr)
    {
        if(crawler->commitMessage == commitMessage)
        {
            isDuplicate = true;
        }
        crawler = crawler->next;
    }
    return isDuplicate;
}

void MiniGit::checkout(string commitID) {
   

}
