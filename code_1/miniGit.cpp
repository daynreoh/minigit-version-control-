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
    
    // delete the repository which means going through the DLL and then within a DLL node deleting
    // the SLL then deleting the DLL. After that delete the hashtable. 
    BranchNode* crawler = commitHead->next;
    BranchNode* behindCrawler = commitHead;
    
    while(crawler != nullptr)
    {
        FileNode* SLLcrawler = crawler->fileHead->next;
        FileNode* SLLprevious = crawler->fileHead;
        
        while(SLLcrawler != nullptr)
        {
            delete SLLprevious;
            SLLprevious = SLLcrawler;
            SLLcrawler = SLLcrawler->next;
        }
        delete SLLprevious;
        delete behindCrawler;
        behindCrawler = crawler;
        crawler = crawler->next;
    }
    delete behindCrawler;
    
    delete ht;
    fs::remove_all(".minigit");
}

void MiniGit::init(int hashtablesize) {
    BranchNode* repoHead = new BranchNode;
    commitHead = repoHead;
    
    commitHead->fileHead = nullptr;
    commitHead->next = nullptr;
    commitHead->previous = nullptr;
    commitHead->commitID = -1;
    
    HashTable* repoTable = new HashTable(5);
    ht = repoTable;
}

void MiniGit::add(string fileName) {
    
    FileNode* newFile = new FileNode;
    newFile->name = fileName;
    newFile->next = nullptr;
    newFile->version = 0;
    
    BranchNode* DLLcrawler = commitHead;
    FileNode* crawler;
        
    while(DLLcrawler->next != nullptr)
    {
        DLLcrawler = DLLcrawler->next;
    }
    crawler = DLLcrawler->fileHead;
    
    if(crawler != nullptr)
    {
        while(crawler->next != nullptr)
        {
            crawler = crawler->next;
        }
        crawler->next = newFile;
    }
    else 
    {
        DLLcrawler->fileHead = newFile;
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
    BranchNode* DLLcrawler = commitHead;
    
    while(DLLcrawler->next != nullptr)
    {
        DLLcrawler = DLLcrawler->next;
    }
    
    FileNode* crawler = DLLcrawler->fileHead;
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
    
    BranchNode* DLLcrawler = commitHead;
    
    while(DLLcrawler->next != nullptr)
    {
        DLLcrawler = DLLcrawler->next;
    }
    
    FileNode* crawler = DLLcrawler->fileHead;
    FileNode* previous = nullptr;
    bool isDeleted = false;
    
    while(crawler != nullptr && !isDeleted)
    {
        // be careful, this might not work because the filenode name will have numbers in it 
        if(crawler->name == fileName)
        {   
            if(crawler == DLLcrawler->fileHead)
            {
                DLLcrawler->fileHead = crawler->next;
                crawler->next = nullptr;
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

void MiniGit::printHashTable()
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

int MiniGit::commit(string msg) 
{    
    BranchNode* parse = commitHead;
    
    while(parse->next != nullptr)
    {
        parse = parse->next;
    }
    
    // this will crawl through the SLL of the most recent DLL
    FileNode* crawler = parse->fileHead;
    FileNode* currFile;
    BranchNode* DLLcopy = new BranchNode;
    
    parse->commitMessage = msg;
    
    // if the new commit is the same as the most previous commit, return -1 and say that you have to try again
    if(noChanges(crawler))
    {
        delete DLLcopy;
        return -1;
    }
    else 
    {
        while(crawler != nullptr)
        {
            // currFile needs to be assigned to either the previous crawler value
            // or the new copy of the file which includes the updated file name
            currFile = fileInDirectory(crawler);
            
            // no SLL yet. No files have been added to the repo
            if(DLLcopy->fileHead == nullptr)
            {
                DLLcopy->fileHead = currFile;
                currFile->next = nullptr; 
            }
            else 
            {
                // f1.txt --> f2.txt --> f3.txt --> f4.txt --> nullptr
                FileNode* newCommitCrawler = DLLcopy->fileHead;
                
                while(newCommitCrawler->next != nullptr)
                {
                    newCommitCrawler = newCommitCrawler->next;
                }
                newCommitCrawler->next = currFile;
                currFile->next = nullptr;
            }
            crawler = crawler->next;
        }
    }
    parse->next = DLLcopy;
    DLLcopy->previous = parse;

    parse->commitMessage = msg;
    parse->commitID = parse->commitID + 1;
    DLLcopy->commitID = parse->commitID;
    bool useless = parseCommitMessage(msg, parse->commitID);
    
    //should return the commitID of the commited DLL node
    return parse->commitID;
}

bool MiniGit::noChanges(FileNode* SLLhead)
{
    int total = 0;
    int unchanged = 0;
    while(SLLhead != nullptr)
    {
        string versionFile = "./.minigit/" + SLLhead->name + "__" + to_string(SLLhead->version);
        if(fs::exists(versionFile))
        {
            if(filesAreSame(SLLhead->name, versionFile))
            {
                unchanged++;
            }
        }
        else 
        {
            return false;
        }
        SLLhead = SLLhead->next;
        total++;
    }
    return (total == unchanged);
}

bool MiniGit::parseCommitMessage(string commitMessage, int commitID)
{   
    commitMessage += " ";
    string word = "";
    int space = 0;
    bool isTrue = true;
    
    for(unsigned int i = 0; i < commitMessage.length(); i++)
    {
        if(commitMessage.at(i) == ' ')
        {
            space++;
            isTrue = ht->insertItem(word, commitID);
            word = "";
        }
        else 
        {
            word += commitMessage.at(i);
        }
    }
    if(space == 0)
    {
        isTrue = ht->insertItem(word, commitID);
    }
    return isTrue;
}

// write this traversal assuming that it will only handle one file at a time and it won't traverse the SLL
FileNode* MiniGit::fileInDirectory(FileNode* file)
{   
    string versionFile = "./.minigit/" + file->name + "__" + to_string(file->version);
    string fileName = file->name; 
    
    // tests to see if the file exists in .minigit
    if(fs::exists(versionFile))
    {   
        // check whether the file has changed in version
        // do this by opening the file, and reading line by line to make sure every line matches up
        FileNode* copy = new FileNode;
        copy->name = fileName;
        copy->version = file->version;
        copy->next = nullptr;
        
        // if files are NOT the same..
        if(filesAreSame(fileName, versionFile) == false)
        {
            cout << "Files: " << fileName << " and " << versionFile << " are not the same" << endl;
            
            copy->version = copy->version + 1;
            addToDirectory(copy);
        }
        
        return copy;
    }
    else
    {
        
        FileNode* copy = new FileNode;
        copy->name = fileName;
        copy->version = file->version;
        copy->next = nullptr;

        addToDirectory(copy);
        return copy;
    }
}

void MiniGit::addToDirectory(FileNode* file)
{
    string directoryFile = "./.minigit/" + file->name + "__" + to_string(file->version);
    fstream currentFile (file->name);
    ofstream newFile;
    newFile.open(directoryFile);
    
    string currentLine;
    
    if(currentFile.is_open() && newFile.is_open())
    {
        getline(currentFile, currentLine);
        newFile << currentLine;
        while(getline(currentFile, currentLine))
        {
            newFile << endl << currentLine;
        }
    }
    currentFile.close();
    newFile.close();
}

bool MiniGit::filesAreSame(string fileName, string directoryFileName)
{   
    ifstream currentFile (fileName);
    ifstream directoryFile (directoryFileName);
    bool isSame = false;
    
    if(currentFile.is_open() && directoryFile.is_open())
    {
        isSame = true;
        string currentLine = "";
        string directoryLine = "";
        
        while(currentFile.eof() == 0 && directoryFile.eof() == 0)
        {      
            getline(currentFile, currentLine);
            getline(directoryFile, directoryLine);
            if(currentLine != directoryLine)
            {
                cout << "The lines of files " << fileName << " and " << directoryFileName << " don't match up" << endl;
                cout << "Here are the lines: " << currentLine << " VS " << directoryLine << endl;
                isSame = false;
                break;
            }
        }
        
        if((currentFile.eof() != 0 && directoryFile.eof() == 0) || (currentFile.eof() == 0 && directoryFile.eof() != 0))
        {
            cout << "One or more file is not at the end of file while the other one is" << endl;
            cout << "Here are the file names: " << fileName << " and " << directoryFileName << endl;
            isSame = false;
        }
    }
    else 
    {
        cout << "one or more files can't open." << endl;
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
    int commitNum = stoi(commitID);
    BranchNode* crawler = commitHead;
    while(crawler != nullptr)
    {
        if(crawler->commitID == commitNum)
        {
            copyDirectoryFiles(crawler);
            break;
        }
        crawler = crawler->next;
    }

}

void MiniGit::copyFromRevert(string miniGitName, string currentName)
{
    ofstream currentFile (currentName);
    ifstream miniGitFile (miniGitName);
    
    string currentLine;
    
    if(currentFile.is_open() && miniGitFile.is_open())
    {
        getline(miniGitFile, currentLine);
        currentFile << currentLine;
        while(getline(miniGitFile, currentLine))
        {
            currentFile << endl << currentLine;
        }
    }
    currentFile.close();
    miniGitFile.close();  
}

void MiniGit::copyDirectoryFiles(BranchNode* revertDLL)
{   
    // the head of the commit that we are looking to copy to current directory
    FileNode* revertCrawler = revertDLL->fileHead;
    
    // this goes to the most recent commit, not the current version
    // how do I access the current version?
    while(revertCrawler != nullptr)
    {
        if(fs::exists(revertCrawler->name))
        {   
            //copy file contents from revert to current file
            string miniGitPath = "./.minigit/" + revertCrawler->name + "__" + to_string(revertCrawler->version);
            copyFromRevert(miniGitPath, revertCrawler->name);
        }
        else 
        {
            cout << "file " << revertCrawler->name << " does not exist in current directory" << endl;
        }
        revertCrawler = revertCrawler->next;
    }
    
}

bool MiniGit::searchCommitID(string commitID)
{
    int commitNum = stoi(commitID);
    BranchNode* crawler = commitHead;
    bool isReal = false;
    
    while(crawler != nullptr)
    {
        if(crawler->commitID == commitNum)
        {
            isReal = true;
        }
        crawler = crawler->next;
    }
    return isReal;
}

      /*/   EXTRA CREDIT PORTION   /*/

void MiniGit::diff(FileNode* inputFile)
{       
    // need to compare the current file with respect to the directory
    // to the most recent commit with the same file name within the .minigit directory
    string versionFile = "./.minigit/ " + inputFile->name + to_string(inputFile->version);
    
    fstream directoryVersion (inputFile->name);
    fstream minigitVersion (versionFile);
    
    string directoryLine;
    string minigitLine;
    
    if(directoryVersion.is_open() && minigitVersion.is_open())
    {
        while(directoryVersion.eof() == 0 && minigitVersion.eof() == 0)
        {
            getline(directoryVersion, directoryLine);
            getline(minigitVersion, minigitLine);
            
            if(directoryLine != minigitLine)
            {
                cout << "Difference: " << endl;
                cout << "   " << directoryLine << endl;
                cout << "   " << minigitLine << endl;
                break;
            }
        }
        cout << "no difference in files." << endl;
    }
    else 
    {
        cout << "one or more files are not open." << endl;
    }
    
    directoryVersion.close();
    minigitVersion.close();
}


FileNode* MiniGit::findFile(string fileName)
{
    BranchNode* DLLcrawler = commitHead;
    
    while(DLLcrawler->next != nullptr)
    {
        DLLcrawler = DLLcrawler->next;
    }
    
    FileNode* crawler = DLLcrawler->fileHead;
    
    while(crawler->next != nullptr)
    {
        if(crawler->name == fileName)
        {
            return crawler;
        }
        crawler = crawler->next;
    }
    return nullptr;
}

