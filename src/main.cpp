#include "config/config_manager.h"
#include "log/log_system.h"

#include "render/context/gl_context.h"

int main(int argc, char **argv) {
    ConfigManager::get()->parse();
    LogSystem::get()->initialize();

    auto ctx = GLContext::createWithWwindow({800, 600, "video-app"});
    auto gl  = ctx->getGL();

    auto window = ctx->getWindow();
    ctx->makeCurrentContext();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        gl.ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl.Clear(GL_COLOR_BUFFER_BIT);

        ctx->swapBuffers();
    }

    ctx.reset();
    LogSystem::get()->shutdown();
    return 0;
}
