#include "gerp.h"
#include <istream>
#include <sstream>
#include <iostream>
#include <string>

using namespace std; 

gerp::gerp() { }

gerp::~gerp() { }

void gerp::run(string &input, string &outputFile){
    traverseDirectory(input); 
    commands(cin, outputFile);
    cout << "Goodbye! Thank you and have a nice day." << endl;    
}

void gerp::traverseDirectory(string &directory) {
    FSTree tree(directory);
    int file_index = 0; 

    if (!tree.getRoot()) { 
        cerr << "Error: Root of FSTree is null. Cannot traverse dir" << endl;
        return;
    }

    traverseHelper(tree.getRoot(), "", file_index);
}

void gerp::commands(istream &input, const string &outputFile) { 
    ofstream output;
    open_or_crash(output, outputFile);
    string command;

    while (input.good()) { 
        cout << "Query? ";
        input >> command;
        if (command.empty()) command = " ";
        else if (command == "@i" or command == "@insensitive") {
            input >> command;
            command = stripNonAlphaNum(command);
            insensitive(command, output); 
        }
        else if (command == "@f") { //New output filename
            input >> command;
            output.close();
            open_or_crash(output, command);
        }
        else if (command == "@q" or command == "@quit") return;
        else { 
            command = stripNonAlphaNum(command);
            newSensitive(command, output);
        } 
    }

    output.close();
}

void gerp::newProcessFile(istream &input, string &path, int &file_index){
    string line;
    int line_num = 1;
    vector<string> fileContent; 
    fileContent.push_back(path);

    vector<bool> boolVector;
    boolVector.push_back(false);
    
    while (getline(input, line)) { 
        newProcessLine(line_num, file_index, line);
        fileContent.push_back(line);
        boolVector.push_back(false);
        line_num++;
    }
    files.push_back(fileContent);
    booleans.push_back(boolVector);
    file_index++;
}

void gerp::newProcessLine(int &line_num, int &file_index, string &line){
    string word;
    stringstream lineStream(line);

    while (lineStream >> word) { 
        word = stripNonAlphaNum(word);
        if (not word.empty())
            newInsertWord(line_num, file_index, word);
    }
}

void gerp::newInsertWord(int &line_num, int &file_index, string &word) {
    string lower = toLower(word);
    auto *entry = hashtable.find(lower);
    if (entry == nullptr){ //The word is not in the hashtable
        info newInfo;
        newInfo.sensitive_cases.push_back(word);
        newInfo.fileLineMap.push_back({file_index, line_num});
        hashtable.insert(lower, {newInfo});
    }
    else { 
        auto &infoEntry = entry->value;
        info newInfo;
        newInfo.sensitive_cases.push_back(word);
        newInfo.fileLineMap.push_back({file_index, line_num});
        infoEntry.push_back(newInfo);
    }   
}

void gerp::newSensitive(string &wordInput, ostream &output) { 
    auto *entry = hashtable.find(toLower(wordInput));

    if (entry == nullptr) { 
        output << wordInput << " Not Found. Try with @insensitive or @i.\n";
        return;
    }
    
    for (int i = 0; i < entry->value.size(); i++) { 
        for (int j = 0; j < entry->value.at(i).sensitive_cases.size(); j++) { 
            if (entry->value.at(i).sensitive_cases.at(j) == wordInput) {
                newPrint(entry->value.at(i).fileLineMap.at(0).first, 
                entry->value.at(i).fileLineMap.at(0).second, output);
            }
        }
    }

    newResetAfterPrint();
}

void gerp::insensitive(string &wordInput, ostream &output){
    string lower = toLower(wordInput);
    auto *entry = hashtable.find(lower);

    if (entry == nullptr) { 
        output << wordInput << " Not Found.\n";
        return;
    }

    for (int i = 0; i < entry->value.size(); i++) { 
        for (int j = 0; j < entry->value.at(i).sensitive_cases.size(); j++) { 
            newPrint(entry->value.at(i).fileLineMap.at(0).first, 
            entry->value.at(i).fileLineMap.at(0).second, output);
        }
    }

}

void gerp::newPrint(int &file_num, int &line_num, ostream &output){
    if (!booleans[file_num][line_num]) {
        output << files[file_num][0] << ":" << line_num << ": " << 
        files[file_num][line_num] << endl;
        booleans[file_num][line_num] = true;
    }
}

void gerp::newResetAfterPrint() { 
    for (int file_num = 0; file_num < booleans.size(); ++file_num) {
        for (int line_num = 0; line_num < booleans[file_num].size(); ++line_num)
            booleans[file_num][line_num] = false;
    }
}

//HELPERS
void gerp::traverseHelper(DirNode *curr, string path, int &file_index){
    if (curr == nullptr)
        return;
    path = path + curr->getName() + "/";

    if (curr->numSubDirs() > 0) {
        for (int i = 0; i < curr->numSubDirs(); i++)
            traverseHelper(curr->getSubDir(i), path, file_index);
    }
    if (curr->numFiles() > 0){
        for (int i = 0; i < curr->numFiles(); i++){
            string updatedPath = path + curr->getFile(i);
            ifstream input;
            open_or_crash(input, updatedPath);
            newProcessFile(input, updatedPath, file_index);
            input.close();
        }
    }
}

string gerp::stripNonAlphaNum(string &input) {
    string output = "";
    char c;
    for (int i = 0; i < input.length(); i++) {
        c = input[i];
        if (isalnum(c))
           output += c;
        if (c == '-') {                 //EDGE CASE
            if (input[i + 1] != '-')
                output += c;
        }
    }

    return output;
}

string gerp::toLower(string &input) {
    string word = input;
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    return word;
}

/*
 * name:      open_or_crash
 * purpose:   make sure a file can be opened or print error message
 * arguments: any type of stream and a filename
 * returns:   none
 * effects:   Opens a filestream or prints an error message and crashes
 */
template<typename streamtype>
void gerp::open_or_crash(streamtype &stream, const string file_name){
    stream.open(file_name);
    if (not stream.is_open()) {
        throw runtime_error("Unable to open file " + file_name);
        return;
    }
}