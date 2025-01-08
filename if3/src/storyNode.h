#ifndef STORYNODE_H
#define STORYNODE_H

#include <string>
#include <vector>
#include <unordered_map>

class StoryNode {
public:
    StoryNode(const std::string& title, const std::string& text, const std::vector<std::string>& options, const std::vector<int>& nextNodes, const std::unordered_map<std::string, std::string>& descriptions, const std::string& entity = "");

    const std::string& getTitle() const { return title; }
    const std::string& getText() const { return text; }
    const std::vector<std::string>& getOptions() const { return options; }
    const std::vector<int>& getNextNodes() const { return nextNodes; }
    const std::unordered_map<std::string, std::string>& getDescriptions() const { return descriptions; }
    const std::string& getEntity() const { return entity; }
    void setEntity(const std::string& entityName) { entity = entityName; }
    bool isVisited() const { return visited; }
    void visit() { visited = true; }

private:
    std::string title;
    std::string text;
    std::vector<std::string> options;
    std::vector<int> nextNodes;
    std::unordered_map<std::string, std::string> descriptions;
    std::string entity;
    bool visited;
};

#endif // STORYNODE_H
