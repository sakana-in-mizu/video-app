#pragma once

#include <memory>
#include <mutex>
#include <string>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "base/noncopyable.h"
#include "base/nonmovable.h"

class WindowManager;

class GLContext : public std::enable_shared_from_this<GLContext> {
    friend WindowManager;

public:
    NONCOPYABLE(GLContext)
    NONMOVABLE(GLContext)

    using GL = GladGLContext;

    struct WindowInfo {
        int width;
        int height;
        std::string title;
    };

    /**
     *  @brief Destroys GL context.
     *
     *  @note This function must only be called from the main thread.
     */
    ~GLContext();

    /**
     *  @brief Creates an OpenGL context with a window.
     *
     *  @param info The information describing the window to create (size, title).
     *  @param visible Determines whether the window should be visible upon creation. Defaults to true.
     *
     *  @return A shared pointer to the created `GLContext` instance.
     *
     *  @note This function must only be called from the main thread.
     */
    static std::shared_ptr<GLContext> createWithWindow(const WindowInfo &info, bool visible = true);

    static std::weak_ptr<GLContext> getCurrentContext() { return s_current_context; }

    static void resetCurrentContext();

    void makeCurrentContext();
    void swapBuffers() const;

    /**
     *  @brief Checks the visibility of the window.
     *
     *  @return true if the window is visible, false otherwise.
     *
     *  @note This function must only be called from the main thread.
     */
    bool isVisible() const { return glfwGetWindowAttrib(m_window, GLFW_VISIBLE); }

    /**
     *  @brief Makes the window visible.
     *
     *  @note This function must only be called from the main thread.
     */
    void showWindow() const { glfwShowWindow(m_window); }

    /**
     *  @brief Makes the window invisible.
     *
     *  @note This function must only be called from the main thread.
     */
    void hideWindow() const { glfwHideWindow(m_window); }

    /**
     *  @brief Creates a WindowManager instance.
     *
     *  @return A shared pointer to the created WindowManager instance.
     *
     *  @note This function must only be called from the main thread.
     */
    std::shared_ptr<WindowManager> createWindowManager();

    const GL &getGL() const { return m_gl; }

private:
    struct GLFWOwnership {
        GLFWOwnership();
        ~GLFWOwnership();
    };

    static std::weak_ptr<GLFWOwnership> s_glfw_existence;

    static thread_local std::weak_ptr<GLContext> s_current_context;

    std::shared_ptr<GLFWOwnership> m_glfw_ownership {};

    GLFWwindow *m_window {};
    GladGLContext m_gl {};

    void getGLFWOwnership();

    GLContext(const WindowInfo &info, bool visible);
};
