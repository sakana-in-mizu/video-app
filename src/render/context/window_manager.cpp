#include "window_manager.h"

#include "log/log_system.h"
#include "gl_context.h"

WindowManager::~WindowManager() { DEBUG("release WindowManager: {}", (void *)this); }

void WindowManager::pollEvents() const { glfwPollEvents(); }

bool WindowManager::shouldClose() const { return glfwWindowShouldClose(m_ctx->m_window); }

void WindowManager::setShouldClose(bool value) const { glfwSetWindowShouldClose(m_ctx->m_window, value); }

int WindowManager::getKey(int key) const { return glfwGetKey(m_ctx->m_window, key); }

int WindowManager::getMouseButton(int button) const { return glfwGetMouseButton(m_ctx->m_window, button); }

WindowManager::WindowManager(std::shared_ptr<GLContext> ctx) : m_ctx(ctx) {
    if (!ctx) {
        FATAL("invalid context!");
    }

    auto window = m_ctx->m_window;
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetCursorEnterCallback(window, cursorEnterCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetDropCallback(window, dropCallback);
}

void WindowManager::onKey(int key, int scancode, int action, int mods) {
    for (auto func : m_on_key_funcs) {
        func(this, key, scancode, action, mods);
    }
}

void WindowManager::onCursorPos(double xpos, double ypos) {
    for (auto func : m_on_cursor_pos_funcs) {
        func(this, xpos, ypos);
    }
}

void WindowManager::onCursorEnter(bool entered) {
    for (auto func : m_on_cursor_enter_funcs) {
        func(this, entered);
    }
}

void WindowManager::onMouseButton(int button, int action, int mods) {
    for (auto func : m_on_mouse_button_funcs) {
        func(this, button, action, mods);
    }
}

void WindowManager::onScroll(double xoffset, double yoffset) {
    for (auto func : m_on_scroll_funcs) {
        func(this, xoffset, yoffset);
    }
}

void WindowManager::onFramebufferSize(int width, int height) {
    for (auto func : m_on_framebuffer_size_funcs) {
        func(this, width, height);
    }
}

void WindowManager::onWindowSize(int width, int height) {
    for (auto func : m_on_window_size_funcs) {
        func(this, width, height);
    }
}

void WindowManager::onDrop(int path_count, const char *paths[]) {
    for (auto func : m_on_drop_funcs) {
        func(this, path_count, paths);
    }
}

void WindowManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto wm = reinterpret_cast<WindowManager *>(glfwGetWindowUserPointer(window));
    wm->onKey(key, scancode, action, mods);
}

void WindowManager::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    auto wm = reinterpret_cast<WindowManager *>(glfwGetWindowUserPointer(window));
    wm->onCursorPos(xpos, ypos);
}

void WindowManager::cursorEnterCallback(GLFWwindow *window, int entered) {
    auto wm = reinterpret_cast<WindowManager *>(glfwGetWindowUserPointer(window));
    wm->onCursorEnter(entered);
}

void WindowManager::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    auto wm = reinterpret_cast<WindowManager *>(glfwGetWindowUserPointer(window));
    wm->onMouseButton(button, action, mods);
}

void WindowManager::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    auto wm = reinterpret_cast<WindowManager *>(glfwGetWindowUserPointer(window));
    wm->onScroll(xoffset, yoffset);
}

void WindowManager::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    auto wm = reinterpret_cast<WindowManager *>(glfwGetWindowUserPointer(window));
    wm->onFramebufferSize(width, height);
}

void WindowManager::windowSizeCallback(GLFWwindow *window, int width, int height) {
    auto wm = reinterpret_cast<WindowManager *>(glfwGetWindowUserPointer(window));
    wm->onWindowSize(width, height);
}

void WindowManager::dropCallback(GLFWwindow *window, int path_count, const char *paths[]) {
    auto wm = reinterpret_cast<WindowManager *>(glfwGetWindowUserPointer(window));
    wm->onDrop(path_count, paths);
}