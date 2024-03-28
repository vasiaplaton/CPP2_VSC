#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <map>

enum VersionState {
    FIXED,
    EDITING
};

class Version {
private:
    int number;
    VersionState state;
    std::string date;
    std::string fileName;
    std::string content;

public:
    Version(int num, VersionState st, std::string dt, std::string fName, std::string cont) :
            number(num), state(st), date(std::move(dt)), fileName(std::move(fName)), content(std::move(cont)) {}

    int getNumber() const { return number; }
    VersionState getState() const { return state; }
    std::string getDate() const { return date; }
    std::string getFileName() const { return fileName; }
    std::string getContent() const { return content; }
};

class File {
private:
    std::string name;
    std::vector<Version> versions;

public:
    File(std::string n) : name(std::move(n)) {}

    void addVersion(int num, VersionState state, std::string date, std::string fileName, std::string content) {
        Version newVersion(num, state, std::move(date), std::move(fileName), std::move(content));
        versions.push_back(newVersion);
    }

    std::vector<Version> getVersions() const { return versions; }
    std::string getName() const { return name; }
};

class ApplicationManager {
private:
    std::map<std::string, File> files;

public:
    void addFile(const std::string& name) {
        File newFile(name);
        files.insert({name, newFile});
    }

    void addVersionToLastFile(int num, VersionState state, std::string date, std::string fileName, std::string content) {
        if (!files.empty()) {
            auto it = files.end();
            --it;
            it->second.addVersion(num, state, std::move(date), std::move(fileName), std::move(content));
        }
    }

    void addVersionByFileName(const std::string& fileName, int num, VersionState state, std::string date, std::string content) {
        auto it = files.find(fileName);
        if (it != files.end()) {
            it->second.addVersion(num, state, std::move(date), fileName, std::move(content));
        } else {
            std::cout << "File with name " << fileName << " not found." << std::endl;
        }
    }

    std::vector<Version> buildConfigurationByDate(const std::string& targetDate) const {
        std::vector<Version> result;
        for (const auto& file : files) {
            for (const auto& version : file.second.getVersions()) {
                if (version.getDate() <= targetDate) {
                    result.push_back(version);
                }
            }
        }
        return result;
    }

    std::vector<Version> buildConfigurationByVersion(int targetVersion) const {
        std::vector<Version> result;
        for (const auto& file : files) {
            for (const auto& version : file.second.getVersions()) {
                if (version.getNumber() == targetVersion) {
                    result.push_back(version);
                }
            }
        }
        return result;
    }

    std::vector<Version> buildConfigurationByState(VersionState targetState) const {
        std::vector<Version> result;
        for (const auto& file : files) {
            for (const auto& version : file.second.getVersions()) {
                if (version.getState() == targetState) {
                    result.push_back(version);
                }
            }
        }
        return result;
    }
};

void printVersions(const std::vector<Version>& versions) {
    for (const auto& version : versions) {
        std::cout << "File: " << version.getFileName() << ", Content: " << version.getContent() << std::endl;
        std::cout << "    Version " << version.getNumber() << ", State: " << (version.getState() == FIXED ? "Fixed" : "Editing") << ", Date: " << version.getDate() << std::endl;
    }
}

int main() {
    ApplicationManager appManager;

    // Add files
    appManager.addFile("File1");
    appManager.addFile("File2");

    // Add versions to files
    appManager.addVersionToLastFile(1, FIXED, "2024-03-25", "File1", "Content of version 1 of File1");
    appManager.addVersionToLastFile(2, EDITING, "2024-03-26", "File1", "Content of version 2 of File1");
    appManager.addVersionToLastFile(3, FIXED, "2024-03-27", "File1", "Content of version 3 of File1");
    appManager.addVersionToLastFile(4, FIXED, "2024-03-28", "File1", "Content of version 4 of File1");

    // Add version by file name
    appManager.addVersionByFileName("File2", 1, EDITING, "2024-03-29", "Content of version 1 of File1");
    appManager.addVersionByFileName("File2", 2, FIXED, "2024-03-29", "Content of version 2 of File2");
    appManager.addVersionByFileName("File2", 3, EDITING, "2024-03-29", "Content of version 3 of File3");

    // Build configurations
    std::cout << "Configuration for date 2024-03-26:" << std::endl;
    printVersions(appManager.buildConfigurationByDate("2024-03-26"));

    std::cout << "\nConfiguration for version 2:" << std::endl;
    printVersions(appManager.buildConfigurationByVersion(2));

    std::cout << "\nConfiguration for state Editing:" << std::endl;
    printVersions(appManager.buildConfigurationByState(EDITING));

    return 0;
}
