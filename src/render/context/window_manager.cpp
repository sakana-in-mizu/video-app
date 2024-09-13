#include "window_manager.h"

#include "log/log_system.h"
#include "gl_context.h"

void WindowManager::pollEvents() const { glfwPollEvents(); }

bool WindowManager::shouldClose() const {
    auto safe_ctx_ref = m_ctx_ref.lock();
    if (safe_ctx_ref) {
        return glfwWindowShouldClose(safe_ctx_ref->m_window);
    }
    return true;
}

WindowManager::WindowManager(std::weak_ptr<GLContext> ctx_ref) : m_ctx_ref(ctx_ref) {
    if (m_ctx_ref.expired()) {
        FATAL("invalid context!");
    }
}
