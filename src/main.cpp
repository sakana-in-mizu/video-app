#include "config/config_manager.h"
#include "log/log_system.h"

#include "render/context/gl_context.h"
#include "render/context/window_manager.h"

int main(int argc, char **argv) {
    ConfigManager::get()->parse();
    LogSystem::get()->initialize();

    auto gl = GLContext::createWithWindow({800, 600, "video-app"});

    auto wm = gl->createWindowManager();
    wm->registerOnKeyFunc([](WindowManager *wm, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            wm->setShouldClose(true);
        }
    });
    gl->makeCurrentContext();

    while (!wm->shouldClose()) {
        wm->pollEvents();

        glCall(gl, ClearColor, 0.2, 0.3, 0.3, 1.0);
        glCall(gl, Clear, GL_COLOR_BUFFER_BIT);

        gl->swapBuffers();
    }

    wm.reset();
    gl.reset();

    LogSystem::get()->shutdown();
    return 0;
}
