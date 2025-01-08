#include "commands.h"
#include "commandParser.h"

void Commands::talk(const std::vector<StoryNode>& story, int& currentNode, const std::string& entity, std::string& message) {
    // Implement logic to talk with NPC
    message = "You talk to " + entity + ". They seem friendly.";
}

void Commands::give(const std::vector<StoryNode>& story, int& currentNode, const std::string& item, const std::string& entity, std::vector<std::string>& inventory, std::string& message) {
    // Implement logic to give an item to NPC
    auto it = std::find(inventory.begin(), inventory.end(), item);
    if (it != inventory.end()) {
        inventory.erase(it);
        message = "You give the " + item + " to " + entity + ".";
    } else {
        message = "You don't have that item.";
    }
}

void Commands::go(const std::vector<StoryNode>& story, int& currentNode, const std::string& direction, std::string& message) {
    auto it = std::find(story[currentNode].getOptions().begin(), story[currentNode].getOptions().end(), direction);
    if (it != story[currentNode].getOptions().end()) {
        int index = std::distance(story[currentNode].getOptions().begin(), it);
        currentNode = story[currentNode].getNextNodes()[index];
        message = "You go " + direction + ".";
    } else {
        message = "Invalid direction.";
    }
}

void Commands::look(const std::vector<StoryNode>& story, int currentNode, const std::string& object, std::string& message) {
    auto it = story[currentNode].getDescriptions().find(object);
    if (it != story[currentNode].getDescriptions().end()) {
        message = it->second;
    } else {
        message = "You don't see that here.";
    }
}

void Commands::take(const std::vector<StoryNode>& story, int currentNode, const std::string& object, std::vector<std::string>& inventory, std::string& message) {
    auto it = story[currentNode].getDescriptions().find(object);
    if (it != story[currentNode].getDescriptions().end()) {
        inventory.push_back(object);
        message = "You have taken the " + object + ".";
    } else {
        message = "You don't see that here.";
    }
}

void Commands::use(const std::vector<StoryNode>& story, int& currentNode, const std::string& object, const std::vector<std::string>& inventory, std::string& message, bool& chestUnlocked) {
    if (object == "key" && currentNode == 1 && std::find(inventory.begin(), inventory.end(), "key") != inventory.end()) {
        chestUnlocked = true;
        message = "You have unlocked the door with the key!";
    } else if (object == "door" && chestUnlocked) {
        currentNode = 2;  // Move to the node where the player enters the locked room
        message = "You open the door and enter the room.";
    } else {
        message = "You can't use that here.";
    }
}

void Commands::explain(const std::string& command, std::string& explanation) {
    if (command == "go") {
        explanation = "go <direction>: Move to a different location\nExample: go north";
    } else if (command == "look") {
        explanation = "look <object>: Look at an object or area\nExample: look at bed";
    } else if (command == "take") {
        explanation = "take <object>: Take an object\nExample: take key";
    } else if (command == "use") {
        explanation = "use <object> [on <object>]: Use an object on another object or by itself\nExample: use key on door";
    } else if (command == "talk") {
        explanation = "talk <entity>: Talk to an NPC\nExample: talk to guard";
    } else if (command == "give") {
        explanation = "give <item> to <entity>: Give an item to an NPC\nExample: give key to guard";
    } else if (command == "inventory") {
        explanation = "inventory: View your inventory";
    } else if (command == "commands" || command == "help") {
        explanation = "commands/help: List all commands or explain a specific command\nExample: help go";
    } else if (command == "verbose") {
        explanation = "verbose: Toggle verbose mode";
    } else if (command == "quit") {
        explanation = "quit: Quit the game";
    } else {
        explanation = "Unknown command.";
    }
}

void Commands::help(const std::string& command, std::string& explanation) {
    explain(command, explanation);
}
