#include "window_manager.h"

#include "log/log_system.h"
#include "gl_context.h"

WindowManager::~WindowManager() { DEBUG("release WindowManager: {}", (void *)this); }

void WindowManager::pollEvents() const { glfwPollEvents(); }

bool WindowManager::shouldClose() const { return glfwWindowShouldClose(m_ctx->m_window); }

void WindowManager::setShouldClose(bool value) const { glfwSetWindowShouldClose(m_ctx->m_window, value); }

WindowManager::WindowManager(std::shared_ptr<GLContext> ctx) : m_ctx(ctx) {
    if (!ctx) {
        FATAL("invalid context!");
    }

    glfwSetWindowUserPointer(m_ctx->m_window, this);
    glfwSetKeyCallback(m_ctx->m_window, keyCallback);
}

void WindowManager::onKey(int key, int scancode, int action, int mods) {
    for (auto func : m_on_key_funcs) {
        func(this, key, scancode, action, mods);
    }
}

void WindowManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto wm = reinterpret_cast<WindowManager *>(glfwGetWindowUserPointer(window));
    wm->onKey(key, scancode, action, mods);
}
