#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h> // For Windows
#include <io.h>     // For Windows
#include <fstream>
#include <sstream>

using namespace std;

class MyShell {
    string currentDirectory;

    string getPrompt() {
        return currentDirectory + " $ ";
    }

    bool fileExists(const string& filename) {
        struct stat buffer;
        return (stat(filename.c_str(), &buffer) == 0);
    }

    void removeDirectory(const string& dirPath) {
        _finddata_t file;
        intptr_t hFile = _findfirst((dirPath + "/*").c_str(), &file);

        if (hFile == -1) {
            cerr << "Error opening directory" << endl;
            return;
        }

        do {
            if (strcmp(file.name, ".") != 0 && strcmp(file.name, "..") != 0) {
                string path = dirPath + "/" + file.name;
                if (file.attrib & _A_SUBDIR) {
                    removeDirectory(path);
                } else {
                    remove(path.c_str());
                }
            }
        } while (_findnext(hFile, &file) == 0);

        _findclose(hFile);
        _rmdir(dirPath.c_str());
    }

    void displayDirectoryContents(const string& dirPath) {
        _finddata_t file;
        intptr_t hFile = _findfirst((dirPath + "/*").c_str(), &file);

        if (hFile == -1) {
            cerr << "Error opening directory" << endl;
            return;
        }

        do {
            cout << file.name << endl;
        } while (_findnext(hFile, &file) == 0);

        _findclose(hFile);
    }

    void copyFile(const string& source, const string& destination) {
        ifstream in(source, ios::binary);
        if (!in) {
            cerr << "Error: Cannot open source file" << endl;
            return;
        }
        ofstream out(destination, ios::binary);
        if (!out) {
            cerr << "Error: Cannot create destination file" << endl;
            in.close();
            return;
        }
        out << in.rdbuf();
        in.close();
        out.close();
    }

    void moveFile(const string& source, const string& destination) {
        copyFile(source, destination);
        remove(source.c_str());
    }

public:
    MyShell() {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        currentDirectory = string(cwd);
    }

    void start() {
        string userInput;
        char cwd[PATH_MAX];
        while (true) {
            cout << getPrompt();
            getline(cin, userInput);
            istringstream iss(userInput);
            string command;
            iss >> command;

            if (command == "exit") {
                break;
            } else if (command == "cd") {
                string dirPath;
                iss >> dirPath;
                if (chdir(dirPath.c_str()) != 0) {
                    perror("cd");
                } else {
                    getcwd(cwd, sizeof(cwd));
                    currentDirectory = string(cwd);
                }
            } else if (command == "ls") {
                string dirPath = ".";
                iss >> dirPath;
                displayDirectoryContents(dirPath);
            } else if (command == "rm") {
                string option, target;
                iss >> option;
                if (option == "-r") {
                    iss >> target;
                    removeDirectory(target);
                } else {
                    target = option;
                    remove(target.c_str());
                }
            } else if (command == "mv") {
                string source, dest;
                iss >> source >> dest;
                moveFile(source, dest);
            } else if (command == "cp") {
                string source, dest;
                iss >> source >> dest;
                copyFile(source, dest);
            } else if (command == "pwd") {
                cout << currentDirectory << endl;
            } else {
                cout << "Command not found" << endl;
            }
        }
    }
};

int main() {
    MyShell customShell;
    customShell.start();
    return 0;
}
