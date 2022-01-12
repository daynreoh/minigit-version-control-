#pragma once

#include<iostream>
#include<vector>
#include "hash.hpp"

using namespace std;


enum GITOP {init, add, rm, commit, checkout, status};

struct FileNode {
    string name;
    int version;
    FileNode* next;
};

struct BranchNode {
    int commitID;
    string commitMessage;
    BranchNode* next;
    BranchNode* previous;
    FileNode* fileHead;   
};

//*** You can add helper functions if you desire ***//
class MiniGit {
private:    
    BranchNode* commitHead;
    int commits;
    HashTable* ht;
    
   

public:
    MiniGit();
    ~MiniGit();
    
    bool noChanges(FileNode* SLLhead);
    void copyFromRevert(string miniGitName, string currentName);
    void copyDirectoryFiles(BranchNode* revertDLL);
    bool searchCommitID(string commitID);
    void printCommitNumbers(string searchKey);
    void addToDirectory(FileNode* file);
    bool parseCommitMessage(string commitMessage, int commitID);
    FileNode* fileInDirectory(FileNode* file);
    bool filesAreSame(string fileName, string directoryFileName);
    bool hasSameCommitMessage(string commitMessage);
    bool isGoodMessage(string commitMessage);
    bool traverseSLL(string fileName);
    bool searchDirectory(string fileName);
    void init(int hashtablesize);
    void add(string fileName);
    void rm(string fileName);
    int commit(string msg);
    void checkout(string commitID);
    void printHashTable();
    FileNode* findFile(string fileName);
    void diff(FileNode* inputFile);
    
    
};