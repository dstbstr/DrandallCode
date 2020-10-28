#include "CommandParser/CommandSplitter.h"
#include "CommandParser/Option.h"
#include "CommandParser/OptionCollection.h"
#include "Singers/IBirthdaySinger.h"
#include "Singers/NaiveSinger.h"
#include "Utilities/StringUtilities.h"

#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // construct an argument parser with argc and argv
    // pass the root directory and the recurse option to a directory navigator
    // fan out the files to find include directives in each file
    // construct including and included maps
    CommandParser::BoolOption recurseFlag("r", "recurse", false, "Whether or not to search the provided directory/directories recursively");
    CommandParser::BoolOption helpFlag("h", "help", false, "Prints the usage");
    CommandParser::VecStringOption filePathOption(false, "File paths and directories");
    CommandParser::OptionCollection options("Produces a report about the number of includes");
    options.Add(recurseFlag).Add(helpFlag).Add(filePathOption);

    CommandParser::CommandSplitter splitter(argc, argv);
    options.Apply(splitter.GetAll());
    if(helpFlag.IsPopulated()) {
        options.PrintUsage(std::cout);
        return 0;
    }

    bool recurse = recurseFlag.IsPopulated();

    std::vector<std::string> targets;
    if(filePathOption.IsPopulated()) {
        targets = filePathOption.GetValue();
    } else {
        targets.push_back(std::filesystem::current_path().string());
    }

    std::cout << "Recurse: " << std::boolalpha << recurse << std::endl;
    std::cout << "Paths: " << StrUtil::JoinVec(", ", targets) << std::endl;

    std::cin.ignore(1, '\n');
    return 0;
}