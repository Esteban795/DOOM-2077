#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <map>

// Steam Deck button mappings - standard SDL2 gamepad
const std::map<SDL_GameControllerButton, std::string> buttonNames = {
    {SDL_CONTROLLER_BUTTON_A, "A Button"},
    {SDL_CONTROLLER_BUTTON_B, "B Button"},
    {SDL_CONTROLLER_BUTTON_X, "X Button"},
    {SDL_CONTROLLER_BUTTON_Y, "Y Button"},
    {SDL_CONTROLLER_BUTTON_BACK, "Select/... Button"},
    {SDL_CONTROLLER_BUTTON_GUIDE, "Steam Button"},
    {SDL_CONTROLLER_BUTTON_START, "Menu Button"},
    {SDL_CONTROLLER_BUTTON_LEFTSTICK, "Left Stick Click"},
    {SDL_CONTROLLER_BUTTON_RIGHTSTICK, "Right Stick Click"},
    {SDL_CONTROLLER_BUTTON_LEFTSHOULDER, "L1 Button"},
    {SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, "R1 Button"},
    {SDL_CONTROLLER_BUTTON_DPAD_UP, "D-Pad Up"},
    {SDL_CONTROLLER_BUTTON_DPAD_DOWN, "D-Pad Down"},
    {SDL_CONTROLLER_BUTTON_DPAD_LEFT, "D-Pad Left"},
    {SDL_CONTROLLER_BUTTON_DPAD_RIGHT, "D-Pad Right"},
};

// Steam Deck axis mappings
const std::map<SDL_GameControllerAxis, std::string> axisNames = {
    {SDL_CONTROLLER_AXIS_LEFTX, "Left Stick X"},
    {SDL_CONTROLLER_AXIS_LEFTY, "Left Stick Y"},
    {SDL_CONTROLLER_AXIS_RIGHTX, "Right Stick X"},
    {SDL_CONTROLLER_AXIS_RIGHTY, "Right Stick Y"},
    {SDL_CONTROLLER_AXIS_TRIGGERLEFT, "Left Trigger (L2)"},
    {SDL_CONTROLLER_AXIS_TRIGGERRIGHT, "Right Trigger (R2)"},
};

int main(int argc, char* argv[]) {
    // Initialize SDL with gamepad, joystick, and sensor support
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK | SDL_INIT_SENSOR) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a small window (required for event handling)
    SDL_Window* window = SDL_CreateWindow("Steam Deck Input Test", 
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        640, 480, 
                                        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window creation error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Print number of joysticks/controllers detected
    std::cout << "Detected " << SDL_NumJoysticks() << " joystick(s)" << std::endl;

    // Try to open the Steam Deck controller
    SDL_GameController* controller = nullptr;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                std::cout << "Found controller: " << SDL_GameControllerName(controller) << std::endl;
                std::cout << "Controller mapping: " << SDL_GameControllerMapping(controller) << std::endl;
                break;
            }
        }
    }

    if (!controller) {
        std::cerr << "No game controller found!" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Set a deadzone for joysticks to prevent drift
    const int JOYSTICK_DEADZONE = 3000;  // Adjust as needed

    // Main event loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_CONTROLLERBUTTONDOWN:
                    if (buttonNames.count(static_cast<SDL_GameControllerButton>(event.cbutton.button))) {
                        std::cout << "Button pressed: " << 
                            buttonNames.at(static_cast<SDL_GameControllerButton>(event.cbutton.button)) << 
                            " (ID: " << static_cast<int>(event.cbutton.button) << ")" << std::endl;
                    } else {
                        std::cout << "Unknown button pressed: " << 
                            static_cast<int>(event.cbutton.button) << std::endl;
                    }
                    break;

                case SDL_CONTROLLERBUTTONUP:
                    if (buttonNames.count(static_cast<SDL_GameControllerButton>(event.cbutton.button))) {
                        std::cout << "Button released: " << 
                            buttonNames.at(static_cast<SDL_GameControllerButton>(event.cbutton.button)) << std::endl;
                    }
                    break;

                case SDL_CONTROLLERAXISMOTION:
                    // Only report significant movements beyond deadzone
                    if (abs(event.caxis.value) > JOYSTICK_DEADZONE || 
                        event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT || 
                        event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
                        
                        if (axisNames.count(static_cast<SDL_GameControllerAxis>(event.caxis.axis))) {
                            std::cout << "Axis: " << 
                                axisNames.at(static_cast<SDL_GameControllerAxis>(event.caxis.axis)) << 
                                " Value: " << event.caxis.value << std::endl;
                        } else {
                            std::cout << "Unknown axis: " << 
                                static_cast<int>(event.caxis.axis) << 
                                " Value: " << event.caxis.value << std::endl;
                        }
                    }
                    break;

                // Handle touchpad events if detected as separate mouse events
                case SDL_MOUSEMOTION:
                    std::cout << "Mouse/Trackpad Motion: x=" << event.motion.x << 
                              ", y=" << event.motion.y << 
                              ", xrel=" << event.motion.xrel << 
                              ", yrel=" << event.motion.yrel << std::endl;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    std::cout << "Mouse/Trackpad Button: " << static_cast<int>(event.button.button) << 
                              " down at x=" << event.button.x << ", y=" << event.button.y << std::endl;
                    break;
            }
        }
        
        // Small delay to prevent CPU hogging
        SDL_Delay(10);
    }

    // Clean up
    SDL_GameControllerClose(controller);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}