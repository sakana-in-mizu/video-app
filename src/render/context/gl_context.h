#pragma once

#include <memory>
#include <string>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "base/noncopyable.h"
#include "base/nonmovable.h"

class GLContext : Noncopyable, Nonmovable, std::enable_shared_from_this<GLContext> {
public:
    using GL = GladGLContext;

    struct WindowInfo {
        int width;
        int height;

        std::string title;
    };

    ~GLContext();

    static std::shared_ptr<GLContext> createWithWwindow(const WindowInfo &info);

    GLFWwindow *getWindow() const { return m_window; }

    void makeCurrentContext();
    void detachContext() const;
    void swapBuffers() const;

    static std::weak_ptr<GLContext> getCurrentContext() { return s_current_context; }

    const GL &getGL() const { return m_gl; }

private:
    struct GLFWOwnership {
        GLFWOwnership();
        ~GLFWOwnership();
    };

    static std::weak_ptr<GLFWOwnership> s_glfw_existence;

    static std::weak_ptr<GLContext> s_current_context;

    std::shared_ptr<GLFWOwnership> m_glfw_ownership {};

    GLFWwindow *m_window {};

    GladGLContext m_gl {};

    void getGLFWOwnership();

    GLContext(const WindowInfo &info);
};
