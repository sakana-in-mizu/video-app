#include "window_manager.h"

#include "log/log_system.h"

void WindowManager::init(std::weak_ptr<GLContext> context) {
    if (context.expired()) {
        FATAL("invalid context!");
    }
    m_window = context.lock()->m_window;
}
