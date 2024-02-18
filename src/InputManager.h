//
// Created by Sebastian on 17.02.2024.
//

#ifndef OPENGL_INPUTMANAGER_H
#define OPENGL_INPUTMANAGER_H
#include "GLFW/glfw3.h"


class InputManager {
private:
    bool keysHeldDown[GLFW_KEY_LAST]{false};
    bool keysPressed[GLFW_KEY_LAST]{false};

public:
    void setKeyState(int keyCode, bool down) {
        if (keysHeldDown[keyCode] != down) {
            keysPressed[keyCode] = true;
        }
        keysHeldDown[keyCode] = down;
    }

    bool isKeyDown(int keyCode) {
        return keysHeldDown[keyCode];
    }

    bool wasKeyPressed(int keyCode) {
        return !keysHeldDown[keyCode] && keysPressed[keyCode];
    }

    void resetKeyPresses() {
        for (bool& changedKey : keysPressed) {
            changedKey = false;
        }
    }
};


#endif //OPENGL_INPUTMANAGER_H
