#include <iostream>
#include <string>
#include "lib/parser.hpp"
using namespace std;

int main(){
    cout << "Enter the path of the main file (absolute path): ";
    string path;
    getline(cin, path);
    bfsOnIncludes(path);
}