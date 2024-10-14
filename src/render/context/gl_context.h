#pragma once

#include <memory>
#include <string>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "base/noncopyable.h"
#include "base/nonmovable.h"
#include "log/log_system.h"

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

inline const char *glErrorString(GLenum err) {
    switch (err) {
        case GL_INVALID_ENUM:
            return "INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GINVALID_OPERATION";
        case GL_OUT_OF_MEMORY:
            return "OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "INVALID_FRAMEBUFFER_OPERATION";
        default:
            return "UNKNOWN_ERROR";
    }
}

template<typename GlFunction, typename... Params>
auto glCallImpl(const char *file,
                int line,
                const char *function,
                GLContext *ctx,
                GlFunction glFunction,
                Params... params) -> std::enable_if_t<std::is_same_v<void, decltype(glFunction(params...))>> {
    glFunction(params...);
    auto logger = LogSystem::get()->getLogger();
    GLenum err;
    while ((err = ctx->getGL().GetError()) != GL_NO_ERROR) {
        logger->log({file, line, function}, spdlog::level::err, "OpenGL error: {}", glErrorString(err));
    }
}

template<typename GlFunction, typename... Params>
auto glCallImpl(const char *file,
                int line,
                const char *function,
                GLContext *ctx,
                GlFunction glFunction,
                Params... params)
    -> std::enable_if_t<!std::is_same_v<void, decltype(glFunction(params...))>, decltype(glFunction(params...))> {
    auto ret = glFunction(params...);
    auto logger = LogSystem::get()->getLogger();
    GLenum err;
    while ((err = ctx->getGL().GetError()) != GL_NO_ERROR) {
        logger->log({file, line, function}, spdlog::level::err, "OpenGL error: {}", glErrorString(err));
    }
    return ret;
}

#ifdef GL_ERROR_CHECK
#define glCall(ctx, func, ...) glCallImpl(__FILE__, __LINE__, __FUNCTION__, ctx.get(), ctx->getGL().func, __VA_ARGS__)
#else
#define glCall(ctx, func, ...) ctx->getGL().func(__VA_ARGS__)
#endif