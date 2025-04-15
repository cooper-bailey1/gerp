#ifndef GERP
#define GERP

#include "HashTable.h"
#include "DirNode.h"
#include "FSTree.h"
#include <vector>
#include <utility>
#include <functional>
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <iostream>

using namespace std;

class gerp {
public: 
    gerp();
    ~gerp();
    void run(string &inputDirectory, string&outputFile);

private:
    struct info { 
        vector<string> sensitive_cases;
        vector<pair<int, int>> fileLineMap; //File#, Line#, bool "seen"        
        info(){} 
    };

    void traverseDirectory(string &directory);
    void commands(istream &input, const string &outputFile);

    void newProcessFile(istream &input, string &path, int &file_index);
    void newProcessLine(int &line_num, int &file_index, string &line); 

    void newSensitive(string &wordInput, ostream &output);
    void insensitive(string &wordInput, ostream &output);
    void newPrint(int &file_num, int &line_num, ostream &output);
    void newResetAfterPrint();
    // void resetAfterPrint(wordInfo &info);
    
    //Helper Functions
    void traverseHelper(DirNode *curr, string path, int &file_index);
    void newInsertWord(int &line_num, int &file_index, string &word); 
    string stripNonAlphaNum(string &input);
    string toLower(string &input);
    template<typename streamtype>
    void open_or_crash(streamtype &stream, const string file_name);
    
    //Key: Insenstive (lowercase) Value: vector of info struct
    HashTable<string, vector<info>> hashtable;
    //Outer vector represents a whole file inner vector represents each line
    vector<vector<string>> files;
    vector<vector<bool>> booleans; //Parallel array to files

    //Functions used to Test
    //void printInfo();
    //void printLineVector();
};

#endif