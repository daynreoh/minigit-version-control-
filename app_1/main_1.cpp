#include <iostream>
#include "../code_1/miniGit.hpp"
#include <filesystem>
using namespace std;

bool hasLetters(string input);

/*
 * Purpose; displays a menu with options
 */
void displayMenu()
{
    cout << "Select a numerical option:" << endl;
    cout << "+=====Main Menu=========+" << endl;
    cout << " 1. init " << endl;
    cout << " 2. add " << endl;
    cout << " 3. rm " << endl;
    cout << " 4. commit " << endl;
    cout << " 5. checkout" << endl;
    cout << " 6. search" << endl;
    cout << " 7. quit " << endl;
    cout << "+-----------------------+" << endl;
    cout << "#> ";
}
int main(int argc, char* argv[]) {
    
    MiniGit* repository = new MiniGit;
    bool isSeven = false;
    int numOperation;
    string inputNums;
    
    while(!isSeven)
    {
        displayMenu();
        cin >> inputNums;
        bool inputLetter = hasLetters(inputNums);
        
        if(!inputLetter)
        {
            numOperation = stoi(inputNums);

            if(numOperation == 1)
            {
                // call initializer functions
                repository->init(5);
            }
            else if(numOperation == 2)
            {
                // add file
                string fileName;
                cout << "please enter a file name: ";
                cin >> fileName;
                cout << endl;
                
                while(!repository->searchDirectory(fileName))
                {
                    cout << "file does not exist, enter a different name" << endl;
                    
                    cout << "please enter a file name: ";
                    cin >> fileName;
                    cout << endl;
                }
                
                if(!repository->traverseSLL(fileName))
                {
                    repository->add(fileName);
                }
                else 
                {
                    cout << "a file with that name already exists in the repository" << endl;
                }
                
            }
            else if(numOperation == 3)
            {
                // remove file
                string fileName;
                cout << "please enter a file name: ";
                cin >> fileName;
                cout << endl;
                
                if(repository->traverseSLL(fileName))
                {
                    repository->rm(fileName);
                }
                else 
                {
                    cout << "file does not exist." << endl;
                }
                
            }
            else if(numOperation == 4)
            {
                // commit changes
                string commitMessage;
                cout << "please enter a commit message (at most 3 words): ";
                cin.ignore();
                getline(cin, commitMessage);

                while(!repository->isGoodMessage(commitMessage))
                {
                    cout << "message was more than 3 words, try again." << endl;

                    cout << "please enter a commit message (at most 3 words): ";
                    cin.ignore();
                    getline(cin, commitMessage);
                }
                
                while(repository->hasSameCommitMessage(commitMessage))
                {
                    // true, then duplicate
                    cout << "commit message already exists, try a different one." << endl;
                    
                    cout << "please enter a commit message (at most 3 words): ";
                    cin.ignore();
                    getline(cin, commitMessage);
                }
                      
                repository->commit(commitMessage);
               
               
                
                
                
            }
            else if(numOperation == 5)
            {
                // checkout 
            }
            else if(numOperation == 6)
            {
                // search hash table
            }
            else if(numOperation == 7)
            {
                isSeven = true;
            }
            else if(numOperation == 8)
            {
                string fileName = "f1.txt";
                string fileName2 = "f2.txt";
                if(repository->filesAreSame(fileName, fileName2))
                {
                    cout << "files are the same!" << endl;
                }
                else 
                {
                    cout << "files are not the same..." << endl;
                }
            }
            else
            {
                cout << endl;
                cout << "*** invalid numerical operation, please choose again ***" << endl;
                cout << endl;
            }
        }
        else 
        {
            cout << endl;
            cout << "*** please input a numerical expression, you inputted a letter ***" << endl;
            cout << endl;
        }
        
    }
   
    return 0;
}

bool hasLetters(string input)
{
    bool areLetters = false;
    for(unsigned int i = 0; i < input.length(); i++)
    {   
        if(isdigit(input.at(i)) == 0)
        {
            areLetters = true;
        }
    }
    return areLetters;    
}