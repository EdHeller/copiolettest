#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>
#include <unordered_map>
#include "storyNode.h"

class Commands {
public:
    static void talk(const std::vector<StoryNode>& story, int& currentNode, const std::string& entity, std::string& message);
    static void give(const std::vector<StoryNode>& story, int& currentNode, const std::string& item, const std::string& entity, std::vector<std::string>& inventory, std::string& message);
    static void go(const std::vector<StoryNode>& story, int& currentNode, const std::string& direction, std::string& message);
    static void look(const std::vector<StoryNode>& story, int currentNode, const std::string& object, std::string& message);
    static void take(const std::vector<StoryNode>& story, int currentNode, const std::string& object, std::vector<std::string>& inventory, std::string& message);
    static void use(const std::vector<StoryNode>& story, int& currentNode, const std::string& object, const std::vector<std::string>& inventory, std::string& message, bool& chestUnlocked);
    static void explain(const std::string& command, std::string& explanation);
    static void help(const std::string& command, std::string& explanation);
};

#endif // COMMANDS_H
