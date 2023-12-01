#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <ctime>
#include <filesystem>
#include <thread>

using namespace std;
namespace fs = std::filesystem;

class MyShell {
private:
    string currentDirectory;

    string getPrompt() {
        return currentDirectory + " $ ";
    }

    bool fileExists(const string& filename) {
        struct stat buffer;
        return (stat(filename.c_str(), &buffer) == 0);
    }

    void copyFile(const std::string& sourcePath, const std::string& destinationDir) {
        std::string fileName = fs::path(sourcePath).filename().string();
        std::string destinationPath = destinationDir + "/" + fileName;

        if (!fs::exists(destinationDir)) {
            try {
                fs::create_directories(destinationDir);
            } catch (const std::exception& e) {
                std::cerr << "Error creating destination directory: " << e.what() << std::endl;
                return;
            }
        }

        std::ifstream sourceFile(sourcePath, std::ios::binary);
        std::ofstream destinationFile(destinationPath, std::ios::binary);

        if (!sourceFile.is_open() || !destinationFile.is_open()) {
            std::cerr << "Error opening files for copying." << std::endl;
            return;
        }

        destinationFile << sourceFile.rdbuf();

        std::cout << "File copied successfully from " << sourcePath << " to " << destinationPath << std::endl;
    }

    void copyDirectory(const std::string& sourceDir, const std::string& destinationDir) {
        std::vector<std::thread> threads;

        if (!fs::exists(sourceDir) || !fs::is_directory(sourceDir)) {
            std::cerr << "Error: Source directory does not exist." << std::endl;
            return;
        }

        if (!fs::exists(destinationDir)) {
            try {
                fs::create_directories(destinationDir);
            } catch (const std::exception& e) {
                std::cerr << "Error creating destination directory: " << e.what() << std::endl;
                return;
            }
        }

        // Iterate over the contents of the source directory
        for (const auto& entry : fs::directory_iterator(sourceDir)) {
            // If it's a regular file, copy it
            if (fs::is_regular_file(entry.path())) {
                threads.emplace_back([this, sourcePath = entry.path(), destDir = destinationDir]() {
                    copyFile(sourcePath, destDir);
                });
            }
            // If it's a directory, recursively copy it
            else if (fs::is_directory(entry.path())) {
                std::string subDir = destinationDir + "/" + entry.path().filename().string();
                threads.emplace_back([this, sourcePath = entry.path(), destDir = subDir]() {
                    copyDirectory(sourcePath, destDir);
                });
            }
        }

        // Wait for all threads to finish
        for (auto& thread : threads) {
            thread.join();
        }

        std::cout << "Directory copied successfully from " << sourceDir << " to " << destinationDir << std::endl;
    }


    bool promptForConfirmation(const string& filePath) {
        string response;
        cout << "Do you really want to remove '" << filePath << "'? (y/n): ";
        getline(cin, response);
        return (response == "y" || response == "Y");
    }
    
    bool myRemove(const char* path) {
    if (std::remove(path) != 0) {
        perror("Error removing file");
        return false;
    } else {
        std::cout << "File removed successfully: " << path << "\n";
        return true;
    }
}

       void removeFile(const std::string& filePath, bool recursive = false) {
        if (promptForConfirmation(filePath)) {
            // Check if it's a directory
            if (fs::is_directory(filePath)) {
                if (recursive) {
                    try {
                        fs::remove_all(filePath); // Recursive removal
                        std::cout << "Directory '" << filePath << "' removed successfully." << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "Error removing directory: " << e.what() << std::endl;
                    }
                } else {
                    std::cerr << "Error: Directory '" << filePath << "' cannot be removed without the -r option." << std::endl;
                }
            } else {
                // It's a regular file
                try {
                    fs::remove(filePath);
                    std::cout << "File '" << filePath << "' removed successfully." << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error removing file: " << e.what() << std::endl;
                }
            }
        } else {
            std::cout << "Removal canceled." << std::endl;
        }
    }

    void displayDirectoryContents(const string& dirPath, bool sortBySize, bool humanReadable, bool sortByTime, bool detailedInfo, bool recursive, int depth = 0) {
        DIR* dir = opendir(dirPath.c_str());
        if (dir == nullptr) {
            cerr << "Error opening directory" << endl;
            return;
        }

        struct dirent* entry;
        vector<pair<string, struct stat>> files;

        while ((entry = readdir(dir)) != nullptr) {
            struct stat fileStat;
            string filePath = dirPath + "/" + entry->d_name;

            if (stat(filePath.c_str(), &fileStat) == 0) {
                files.push_back({entry->d_name, fileStat});
            } else {
                cerr << "Error: Unable to retrieve file information" << endl;
            }
        }

        closedir(dir);

        if (sortBySize) {
            sort(files.begin(), files.end(), [](const auto& a, const auto& b) {
                return a.second.st_size > b.second.st_size;
            });
        } else if (sortByTime) {
            sort(files.begin(), files.end(), [](const auto& a, const auto& b) {
                return difftime(a.second.st_mtime, b.second.st_mtime) > 0;
            });
        }

        cout << dirPath << ":" << endl;

        for (const auto& fileData : files) {
            const auto& fileName = fileData.first;
            const auto& fileInfo = fileData.second;

            for (int i = 0; i < depth; ++i) {
                cout << "    ";  // Adjust the indentation based on the depth
            }

            string filePath = dirPath + "/" + fileName;

            cout << (S_ISDIR(fileInfo.st_mode) ? "d" : "-");
            cout << ((fileInfo.st_mode & S_IRUSR) ? "r" : "-");
            cout << ((fileInfo.st_mode & S_IWUSR) ? "w" : "-");
            cout << ((fileInfo.st_mode & S_IXUSR) ? "x" : "-");
            cout << " " << fileInfo.st_size;
            char timebuf[26];
            ctime_r(&fileInfo.st_mtime, timebuf);
            timebuf[strlen(timebuf) - 1] = '\0';  // Remove the newline character from ctime result
            cout << " " << timebuf;
            cout << " " << fileName << endl;

            if (recursive && S_ISDIR(fileInfo.st_mode) && strcmp(fileName.c_str(), ".") != 0 && strcmp(fileName.c_str(), "..") != 0) {
                displayDirectoryContents(dirPath + "/" + fileName, sortBySize, humanReadable, sortByTime, detailedInfo, recursive, depth + 1);
            }
        }
        cout << endl;
    }

    void moveFile(const string& source, const string& destination) {
        copyFile(source, destination);
        removeFile(source);
    }

public:
    MyShell() {
        char buffer[PATH_MAX];
        if (getcwd(buffer, sizeof(buffer)) == nullptr) {
            perror("getcwd");
            exit(EXIT_FAILURE);
        }
        currentDirectory = string(buffer);
    }

    void start() {
        string userInput;
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

                if (dirPath == "~") {
                    // Change to home directory
                    if (chdir(getenv("HOME")) != 0) {
                        perror("cd");
                    } else {
                        char buffer[PATH_MAX];
                        if (getcwd(buffer, sizeof(buffer)) == nullptr) {
                            perror("getcwd");
                            exit(EXIT_FAILURE);
                        }
                        currentDirectory = string(buffer);
                    }
                } else if (dirPath == "..") {
                    // Move up one directory (parent directory)
                    if (chdir("..") != 0) {
                        perror("cd");
                    } else {
                        char buffer[PATH_MAX];
                        if (getcwd(buffer, sizeof(buffer)) == nullptr) {
                            perror("getcwd");
                            exit(EXIT_FAILURE);
                        }
                        currentDirectory = string(buffer);
                    }
                }
                else if (dirPath == "help") {
                    cout<<"The  help function - the following functionality is -: "<<endl;
                    cout<<"cd .."<<"- move to the parent directory "<<endl;
                    cout<<"cd - "<<" - move to the previous directory "<<endl;
                    cout<<"cd help "<<"- to check the cd functionality "<<endl;
                    cout<<"cd ~ "<<"to go to the root directory "<<endl;
                    cout<<"cd foldername "<<"- to the foldername present in the present directory "<<endl;
                    } else if (dirPath == "-") {
                    // Move to the previous directory
                    if (chdir("-") != 0) {
                        perror("cd");
                    } else {
                        char buffer[PATH_MAX];
                        if (getcwd(buffer, sizeof(buffer)) == nullptr) {
                            perror("getcwd");
                            exit(EXIT_FAILURE);
                        }
                        currentDirectory = string(buffer);
                    }
                } else {
                    // Change to the specified directory and list its contents
                    if (chdir(dirPath.c_str()) != 0) {
                        perror("cd");
                    } else {
                        char buffer[PATH_MAX];
                        if (getcwd(buffer, sizeof(buffer)) == nullptr) {
                            perror("getcwd");
                            exit(EXIT_FAILURE);
                        }
                        currentDirectory = string(buffer);
                        displayDirectoryContents(currentDirectory, false, false, false, true, false);
                    }
                }
            } else if (command == "ls") {
                string dirPath = ".";
                bool sortBySize = false;
                bool humanReadable = false;
                bool sortByTime = false;
                bool detailedInfo = false;
                bool recursive = false;

                string option;
                while (iss >> option) {
                    if (option == "-S") {
                        sortBySize = true;
                    } else if (option == "-h") {
                        humanReadable = true;
                    } else if (option == "-t") {
                        sortByTime = true;
                    } else if (option == "-l") {
                        detailedInfo = true;
                    } else if (option == "-R") {
                        recursive = true;
                    }else if (option == "help"){
                    cout<<"The  help function - the following functionality is -: "<<endl;
                    cout<<"ls -S "<<"- to sort the file according to the name in the directory"<<endl;
                    cout<<"ls -t "<<" - to sort the file according to the creation time "<<endl;
                    cout<<"ls -l "<<"- to see the detail of the file "<<endl;
                    cout<<"ls help "<<"to check the functionality "<<endl;
                    cout<<"ls foldername "<<"- to the foldername present in the present directory "<<endl;
                    } 
                    else {
                        dirPath = option;
                    }
                }

                displayDirectoryContents(dirPath, sortBySize, humanReadable, sortByTime, detailedInfo, recursive);
            } else if (command == "rm") {
               string option, target;
iss >> option;

if (option == "-i") {
    // Interactive removal
    iss >> target;
    removeFile(target);
}else if (option == "help"){
                    cout<<"The  help function - the following functionality is -: "<<endl;
                    cout<<"rm -i "<<"- for the interactive removal"<<endl;
                    cout<<"rm -r "<<" - to remove file recursively "<<endl;
                    cout<<"rm .txt "<<"- to delete file of .txt extension "<<endl;
                    cout<<"rm  "<<"to remove particular file "<<endl;
                    } 
 else if (option == "-r") {
    // Recursive removal
    iss >> target;
    removeFile(target, true);
} else if (option == "*.txt") {
    // Pattern-based removal
    std::string pattern = "*.txt";
    for (const auto& entry : fs::directory_iterator(currentDirectory)) {
        if (entry.is_regular_file() && entry.path().filename().string().find(pattern) != std::string::npos) {
            std::string filePath = entry.path();
            removeFile(filePath);
        }
    }
} else {
    // Remove a specific file
    if (fileExists(option)|| fs::is_directory(option)) {
        removeFile(option);
    } else {
        cerr << "Error: File '" << option << "' not found." << endl;
    }
}
} else if (command == "mv") {
                 string option;
                 iss>> option;
                 if (option == "help"){
                    cout<<"The  help function - the following functionality is -: "<<endl;
                    cout<<"mv help "<<"- to check the move function "<<endl;
                    cout<<"mv "<<" - to move file from one directory to another directory"<<endl;

                    }
                    else
                    {
                string source, dest;
                iss >> source >> dest;
                moveFile(source, dest);
                }
            } else if (command == "cp") {
                string option, source, dest;
                iss >> option >> source >> dest;
                if (option == "-r") {
                fs::path sourcePath(source);
                fs::path destinationPath(dest);
                      copyDirectory(source, dest);
                }else if (option == "help"){
                    cout<<"The  help function - the following functionality is -: "<<endl;
                    cout<<"cp  "<<"- used to copy file "<<endl;
                    cout<<"cp -r "<<" - to copy folder from one to another recursively "<<endl;
                    cout<<"cp help "<<"- to tell the functionality "<<endl;
                    } 
                else {
                
                cout<<option<<" "<<source;
                 
    if (!fs::exists(option)) {
    
        std::cerr << "Error: Source file or directory does not exist." << std::endl;
        
    }

    // Check if the destination directory exists or create it
    try {
        if (!fs::exists(source)) {
            fs::create_directories(source);
            std::cout << "Destination directory created: " << source << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error creating destination directory: " << e.what() << std::endl;
        
    }

    if (fs::is_directory(option)) {
        // If the source is a directory, iterate over its contents and copy each file to the destination directory
        for (const auto& entry : fs::directory_iterator(option)) {
            if (fs::is_regular_file(entry.path())) {
                copyFile(entry.path(), source);
            }
        }
    } else if (fs::is_regular_file(option)) {
        // If the source is a file, copy it to the destination directory
        copyFile(option, source);
    } else {
        std::cerr << "Error: Invalid source path." << std::endl;
        
    }
    }
    }
    else if (command == "pwd") {
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

