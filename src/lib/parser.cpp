#include "parser.hpp"
#include <queue>
#include <set>
#include <filesystem>
#include <vector>
#include <regex>
#include <fstream>

namespace fs = std::filesystem;

vector<string> getAllIncludes(const string& filePath){
    vector<string> includes;
    regex includeRegex("#include\\s*\"(.*)\"");
    ifstream file(filePath);
    if (file.is_open()){
        string line;
        while (getline(file, line)){
            smatch match;
            if (regex_search(line, match, includeRegex)){
                includes.push_back(match[1]);
            }
        }
        file.close();
    } else {
        cout << "Unable to open file: " << filePath << endl;
    }
    return includes;
}

string getAbsoluteFilePath(const string& relativePath, const string& currentDir) {
    fs::path absolutePath = fs::path(currentDir) / relativePath;
    return absolutePath.string();
}

void bfsOnIncludes(const string& filename) {
    queue<pair<string, string>> includeQueue;  // (file path, current dir)
    set<string> visitedIncludes;

    fs::path filePath = fs::path(filename).lexically_normal();
    includeQueue.push({ filePath.string(), filePath.parent_path().string() });

    while (!includeQueue.empty()) {
        string currentFile = includeQueue.front().first;
        string currentDir = includeQueue.front().second;
        includeQueue.pop();

        if (visitedIncludes.count(currentFile) > 0)
            continue;

        visitedIncludes.insert(currentFile);
        cout << "Processing " << currentFile << endl;

        vector<string> includes = getAllIncludes(currentFile);
        for (const string& include : includes) {
            cout << "Include: " << include << endl;

            string absolutePath = getAbsoluteFilePath(include, currentDir);
            if (!fs::exists(absolutePath)) {
                cout << "Include file not found: " << absolutePath << endl;
                continue;
            }

            includeQueue.push({ absolutePath, fs::path(absolutePath).parent_path().string() });
        }
    }

    cout << "g++ -o main ";

    for (const string& include : visitedIncludes) {
        // if include ends with .hpp change to .cpp
        string includeCpp = include;
        if (includeCpp.size() > 4 && includeCpp.substr(includeCpp.size() - 4) == ".hpp") {
            includeCpp = includeCpp.substr(0, includeCpp.size() - 4) + ".cpp";
        }
        cout << "\"" << includeCpp << "\" ";
    }
    cout << endl;
}