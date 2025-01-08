#include "storyNode.h"

StoryNode::StoryNode(const std::string& title, const std::string& text, const std::vector<std::string>& options, const std::vector<int>& nextNodes, const std::unordered_map<std::string, std::string>& descriptions, const std::string& entity)
    : title(title), text(text), options(options), nextNodes(nextNodes), descriptions(descriptions), entity(entity), visited(false) {}
