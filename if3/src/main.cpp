#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "json.hpp"
#include "storyNode.h"
#include "commands.h"
#include "commandParser.h"

using json = nlohmann::json;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* FONT_PATH = "assets/Ubuntu-Regular.ttf";
const int FONT_SIZE = 24;
const char* BACKGROUND_PATH = "assets/background.png";

enum DisplayMode {
    STORY,
    HELP,
    COMMANDS,
    INVENTORY,
    MESSAGE  // New display mode for various messages
};

bool verboseMode = false;  // Global variable to control verbose mode
int turnCounter = 0;  // Global variable to track the number of turns

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    SDL_Color White = {255, 255, 255};
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(font, text.c_str(), White, SCREEN_WIDTH - 40);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect messageRect;
    messageRect.x = x;
    messageRect.y = y;
    messageRect.w = surfaceMessage->w;
    messageRect.h = surfaceMessage->h;

    SDL_RenderCopy(renderer, message, nullptr, &messageRect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(message);
}

void renderBackground(SDL_Renderer* renderer, SDL_Texture* backgroundTexture) {
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    int imgW, imgH;
    SDL_QueryTexture(backgroundTexture, NULL, NULL, &imgW, &imgH);

    float aspectRatio = (float)imgW / (float)imgH;
    int renderW, renderH;

    if ((float)w / (float)h > aspectRatio) {
        renderH = h;
        renderW = (int)(h * aspectRatio);
    } else {
        renderW = w;
        renderH = (int)(w / aspectRatio);
    }

    SDL_Rect destRect = { (w - renderW) / 2, (h - renderH) / 2, renderW, renderH };
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &destRect);
}

std::vector<StoryNode> loadStory(const std::string& filePath) {
    std::ifstream file(filePath);
    json storyJson;
    file >> storyJson;

    std::vector<StoryNode> story;
    for (const auto& node : storyJson) {
        story.emplace_back(node["title"], node["text"], node["options"], node["nextNodes"], node["descriptions"]);
    }
    return story;
}

void displayHelp(SDL_Renderer* renderer, TTF_Font* font, const std::string& command = "") {
    std::string helpText;

    if (command.empty()) {
        helpText = "Commands:\n"
                   "go <direction>: Move to a different location\n"
                   "look <object>: Look at an object or area\n"
                   "take <object>: Take an object\n"
                   "use <object> [on <object>]: Use an object on another object or by itself\n"
                   "talk <entity>: Talk to an NPC\n"
                   "give <item> to <entity>: Give an item to an NPC\n"
                   "inventory: View your inventory\n"
                   "commands/help: List all commands or explain a specific command\n"
                   "verbose: Toggle verbose mode\n"
                   "quit: Quit the game";
    } else if (command == "go") {
        helpText = "go <direction>: Move to a different location\n"
                   "Example: go north";
    } else if (command == "look") {
        helpText = "look <object>: Look at an object or area\n"
                   "Example: look at bed";
    } else if (command == "take") {
        helpText = "take <object>: Take an object\n"
                   "Example: take key";
    } else if (command == "use") {
        helpText = "use <object> [on <object>]: Use an object on another object or by itself\n"
                   "Example: use key on door";
    } else if (command == "talk") {
        helpText = "talk <entity>: Talk to an NPC\n"
                   "Example: talk to guard";
    } else if (command == "give") {
        helpText = "give <item> to <entity>: Give an item to an NPC\n"
                   "Example: give key to guard";
    } else if (command == "inventory") {
        helpText = "inventory: View your inventory";
    } else if (command == "commands" || command == "help") {
        helpText = "commands/help: List all commands or explain a specific command\n"
                   "Example: help go";
    } else if (command == "verbose") {
        helpText = "verbose: Toggle verbose mode";
    } else if (command == "quit") {
        helpText = "quit: Quit the game";
    } else {
        helpText = "Unknown command.";
    }

    renderText(renderer, font, helpText, 20, 100);  // Adjusted Y position
}

void displayInventory(SDL_Renderer* renderer, TTF_Font* font, const std::vector<std::string>& inventory) {
    std::string inventoryText = "Inventory:\n";
    for (const auto& item : inventory) {
        inventoryText += "- " + item + "\n";
    }
    renderText(renderer, font, inventoryText, 20, 100);  // Adjusted Y position
}

void displayMessage(SDL_Renderer* renderer, TTF_Font* font, const std::string& message) {
    renderText(renderer, font, message, 20, 100);  // Adjusted Y position
}



void displayStory(SDL_Renderer* renderer, TTF_Font* font, const StoryNode& currentNode, DisplayMode mode, const std::vector<std::string>& inventory, const std::string& message = "") {
    // Display turn counter in the upper right corner
    renderText(renderer, font, "Turns: " + std::to_string(turnCounter), SCREEN_WIDTH - 150, 20);

    switch (mode) {
        case STORY:
            if (verboseMode) {
                renderText(renderer, font, currentNode.getTitle(), 20, 60);  // Adjusted Y position
                renderText(renderer, font, currentNode.getText(), 20, 100);  // Adjusted Y position
            } else {
                renderText(renderer, font, currentNode.getText(), 20, 100);  // Adjusted Y position
            }
            for (size_t i = 0; i < currentNode.getOptions().size(); ++i) {
                renderText(renderer, font, std::to_string(i + 1) + ". " + currentNode.getOptions()[i], 40, 180 + i * 50);  // Adjusted Y position
            }
            break;
        case HELP:
            displayHelp(renderer, font);
            break;
        case COMMANDS:
            displayHelp(renderer, font);
            break;
        case INVENTORY:
            displayInventory(renderer, font, inventory);
            break;
        case MESSAGE:
            displayMessage(renderer, font, message);
            break;
    }
}

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF could not initialize! TTF Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init could not initialize! IMG Error: " << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Interactive Fiction", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    TTF_Font* font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!font) {
        std::cerr << "Failed to load font! TTF Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Surface* backgroundSurface = IMG_Load(BACKGROUND_PATH);
    if (!backgroundSurface) {
        std::cerr << "Failed to load background image! IMG Error: " << IMG_GetError() << std::endl;
      //  return 1;
    }

    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    std::vector<StoryNode> story = loadStory("assets/story.json");

    int currentNode = 0;
    bool quit = false;
    SDL_Event e;
    std::string userInput;
    std::vector<std::string> inventory;
    DisplayMode mode = STORY;
    std::string message;
    bool chestUnlocked = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_RETURN) {
                    if (mode != STORY) {
                        mode = STORY;
                    } else {
						if(!userInput.empty())
						{
							std::string command = CommandParser::toLower(userInput);
							std::vector<std::string> parts = CommandParser::split(command, ' ');

							if (command == "quit") {
								quit = true;
							} else if (command == "help" || command == "commands") {
								if (parts.size() > 1) {
									std::string explanation;
									Commands::help(parts[1], explanation);
									message = explanation;
									mode = MESSAGE;
								} else {
									mode = HELP;
								}
							} else if (command == "inventory") {
								mode = INVENTORY;
							} else if (command == "verbose") {
								verboseMode = !verboseMode;
								mode = MESSAGE;
								message = "Verbose mode " + std::string(verboseMode ? "enabled" : "disabled") + ".";
							} else if (parts[0] == "look") {
								if (parts.size() == 1) {
									mode = MESSAGE;
									message = story[currentNode].getText() + "\nYou look around and see:\n";
									for (const auto& description : story[currentNode].getDescriptions()) {
										message += "- " + description.first + "\n";
									}
								} else {
									std::string object = parts[1];
									for (size_t i = 2; i < parts.size(); ++i) {
										object += " " + parts[i];
									}
									Commands::look(story, currentNode, object, message);
									mode = MESSAGE;
								}
							} else if (parts[0] == "go" && parts.size() > 1) {
								Commands::go(story, currentNode, parts[1], message);
								turnCounter++;
								mode = STORY;
							} else if (parts[0] == "take" && parts.size() > 1) {
								std::string object = parts[1];
								for (size_t i = 2; i < parts.size(); ++i) {
									object += " " + parts[i];
								}
								Commands::take(story, currentNode, object, inventory, message);
								mode = MESSAGE;
							} else if (parts[0] == "use" && parts.size() > 1) {
								std::string object = parts[1];
								for (size_t i = 2; i < parts.size(); ++i) {
									object += " " + parts[i];
								}
								Commands::use(story, currentNode, object, inventory, message, chestUnlocked);
								mode = MESSAGE;
							} else if (parts[0] == "talk" && parts.size() > 1) {
								std::string entity = parts[1];
								for (size_t i = 2; i < parts.size(); ++i) {
									entity += " " + parts[i];
								}
								Commands::talk(story, currentNode, entity, message);
								mode = MESSAGE;
							} else if (parts[0] == "give" && parts.size() > 3 && parts[2] == "to") {
								std::string item = parts[1];
								std::string entity = parts[3];
								for (size_t i = 4; i < parts.size(); ++i) {
									entity += " " + parts[i];
								}
								Commands::give(story, currentNode, item, entity, inventory, message);
								mode = MESSAGE;
						}
                        }
                        else {
                            mode = MESSAGE;
                            message = "Unknown command.";
                        }

                        userInput.clear();
                    }
                } else if (e.key.keysym.sym == SDLK_BACKSPACE && !userInput.empty()) {
                    userInput.pop_back();
                }
            } else if (e.type == SDL_TEXTINPUT) {
                userInput += e.text.text;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (backgroundTexture) {
            renderBackground(renderer, backgroundTexture);
        }
        displayStory(renderer, font, story[currentNode], mode, inventory, message);

        renderText(renderer, font, "> " + userInput, 20, 500);

        SDL_RenderPresent(renderer);
    }

    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }
    TTF_CloseFont(font);
    font = nullptr;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;
    renderer = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
