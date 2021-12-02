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
    commitHead->commitID = 00;
    
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

void MiniGit::printCommitNumbers(string searchKey)
{
    HashNode* hashNode = ht->searchItem(searchKey);
    
    if(hashNode != nullptr)
    {
        cout << "commit numbers for " << searchKey << ": ";

        for(unsigned int i = 0; i < hashNode->commitNums.size() - 1; i++)
        {
            cout << hashNode->commitNums.at(i) << ", ";
        }
        cout << hashNode->commitNums.at(hashNode->commitNums.size() - 1) << endl;
    }
    else 
    {
        cout << "key does not exist in the hash table, please try again." << endl;
    }    
}


void MiniGit::search(string key)
{
}



string MiniGit::commit(string msg) {
    
    bool useless = parseCommitMessage(msg);
    BranchNode* parse = commitHead;
    
    while(parse->next != nullptr)
    {
        parse = parse->next;
    }

    // this will crawl through the SLL
    FileNode* crawler = parse->fileHead;
    
    // this will represent the copied file from fileInDirectory
    FileNode* currFile; 
    
    // creates a copy node for DLL commit
    BranchNode* DLLcopy = new BranchNode;
    DLLcopy->commitID = parse->commitID++;
    
    while(crawler != nullptr)
    {
        currFile = fileInDirectory(crawler);
        if(currFile == crawler)
        {
           // do nothing 
        }
        // means that the crawler file got copied to the .minigit directory
        // somehow need to assign a next pointer
        else
        {
            
            if(DLLcopy->fileHead == nullptr)
            {
                DLLcopy->fileHead = currFile;
                currFile->next = nullptr;
            }
            else 
            {
                // f1.txt --> f2.txt --> f3.txt --> f4.txt --> nullptr
                FileNode* DLLcrawler = DLLcopy->fileHead;
                
                while(DLLcrawler->next != nullptr)
                {
                    DLLcrawler = DLLcrawler->next;
                }
                
                DLLcrawler->next = currFile;
                currFile->next = nullptr;
            }
        }
        crawler = crawler->next;
    }
    
    commitHead->next = DLLcopy;
    DLLcopy->previous = commitHead;
    
    //should return the commitID of the commited DLL node    
    return "" + parse->commitID; 
}

bool MiniGit::parseCommitMessage(string commitMessage)
{   
    commitMessage += " ";
    int commitNumber = commitHead->commitID;
    string word = "";
    int space = 0;
    bool isTrue = true;
    
    for(unsigned int i = 0; i < commitMessage.length(); i++)
    {
        if(commitMessage.at(i) == ' ')
        {
            space++;
            isTrue = ht->insertItem(word, commitNumber);
            word = "";
        }
        else 
        {
            word += commitMessage.at(i);
        }
        
    }
    if(space == 0)
    {
        isTrue = ht->insertItem(word, commitNumber);
    }
    
    return isTrue;
}

// write this traversal assuming that it will only handle one file at a time and it won't traverse the SLL
FileNode* MiniGit::fileInDirectory(FileNode* file)
{   
    
    string versionFile = "./minigit/" + file->name + "__" + to_string(file->version);
    // should look like: ./minigit/f1.txt__00
    
    // creates the filename for .minigit directory
    string fileName = file->name;
    string fileName2 = "./minigit/" + fileName; 
 

    // tests to see if the file exists in .minigit
    if(fs::exists(versionFile))
    {
        // check whether the file has changed in version
        // do this by opening the file, and reading line by line to make sure every line matches up
        if(!filesAreSame(fileName, versionFile))
        {
            // COPY: the file from the current directory to the .minigit directory
            FileNode* copy = new FileNode;
            copy->name = fileName + "__" + to_string(file->version++);
            copy->version = file->version++;
            copy->next = nullptr;
            
            addToDirectory(file->name, versionFile);
            
            return copy;
        }
        
        // do nothing
        return file;
    }
    else
    {
        FileNode* copyCat = new FileNode;
        copyCat->name = file->name + "__" + to_string(file->version++);
        copyCat->version = file->version++;
        copyCat->next = nullptr;
        
        addToDirectory(file->name, versionFile);
        
        return copyCat;
    }
}

void MiniGit::addToDirectory(string fileName, string directoryFile)
{
    fstream currentFile (fileName);
    ofstream newFile;
    newFile.open(directoryFile);
    
    string currentLine;
    
    if(currentFile.is_open() && newFile.is_open())
    {
        while(getline(currentFile, currentLine))
        {
            newFile << currentLine;
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
    
    currentFile.close();
    directoryFile.close();
    
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
