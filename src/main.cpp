#include "config/config_manager.h"
#include "log/log_system.h"

#include "render/window/window_manager.h"

int main(int argc, char **argv) {
    ConfigManager::get()->parse();
    LogSystem::get()->initialize();

    auto ctx = GLContext::createWithWindow({800, 600, "video-app"});
    auto gl  = ctx->getGL();

    ctx->makeCurrentContext();
    auto wm = WindowManager::get();
    wm->init(GLContext::getCurrentContext());

    while (!wm->shouldClose()) {
        wm->pollEvents();

        gl.ClearColor(0.2, 0.3, 0.3, 1.0);
        gl.Clear(GL_COLOR_BUFFER_BIT);

        ctx->swapBuffers();
    }

    ctx.reset();
    LogSystem::get()->shutdown();
    return 0;
}
