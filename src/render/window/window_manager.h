#pragma once

#include "base/singleton.h"
#include "render/context/gl_context.h"

class WindowManager : public Singleton<WindowManager> {
    friend Singleton<WindowManager>;

public:
    void init(std::weak_ptr<GLContext> context);

    void pollEvents() const { return glfwPollEvents(); }

    bool shouldClose() const { return glfwWindowShouldClose(m_window); }

private:
    GLFWwindow *m_window {};
};
