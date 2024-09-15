#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "base/noncopyable.h"
#include "base/nonmovable.h"

class GLContext;

class GLFWwindow;

/**
 *  @class WindowManager
 *
 *  @brief Manages window-related events and interactions.
 *
 *  @note This class can only be created by GLContext, and used only in main thread.
 */
class WindowManager {
    friend GLContext;

public:
    using OnKeyFunc = std::function<void(WindowManager *wm, int key, int scancode, int action, int mods)>;
    using OnCursorPosFunc = std::function<void(WindowManager *wm, double xpos, double ypos)>;
    using OnCursorEnterFunc = std::function<void(WindowManager *wm, bool entered)>;
    using OnMouseButtonFunc = std::function<void(WindowManager *wm, int button, int action, int mods)>;
    using OnScrollFunc = std::function<void(WindowManager *wm, double xoffset, double yoffset)>;
    using OnFramebufferSizeFunc = std::function<void(WindowManager *wm, int width, int height)>;
    using OnWindowSizeFunc = std::function<void(WindowManager *wm, int width, int height)>;
    using OnDropFunc = std::function<void(WindowManager *wm, int path_count, const char *paths[])>;

    NONCOPYABLE(WindowManager)
    NONMOVABLE(WindowManager)

    ~WindowManager();

    /**
     *  @brief Processes all pending events.
     *
     *  @note This function must only be called from the main thread.
     */
    void pollEvents() const;

    bool shouldClose() const;
    void setShouldClose(bool value) const;

    /**
     *  @brief Retrieves the state of a keyboard key.
     *
     *  @param key The key whose state to retrieve.
     *
     *  @return The state of the key (GLFW_PRESS or GLFW_RELEASE).
     *
     *  @note This function must only be called from the main thread.
     */
    int getKey(int key) const;

    /**
     *  @brief Retrieves the state of a mouse button.
     *
     *  @param button The button whose state to retrieve.
     *
     *  @return The state of the mouse button (GLFW_PRESS or GLFW_RELEASE).
     *
     *  @note This function must only be called from the main thread.
     */
    int getMouseButton(int button) const;

    void registerOnKeyFunc(OnKeyFunc func) { m_on_key_funcs.push_back(std::move(func)); }
    void registerOnCursorPosFunc(OnCursorPosFunc func) { m_on_cursor_pos_funcs.push_back(std::move(func)); }
    void registerOnCursorEnterFunc(OnCursorEnterFunc func) { m_on_cursor_enter_funcs.push_back(std::move(func)); }
    void registerOnMouseButtonFunc(OnMouseButtonFunc func) { m_on_mouse_button_funcs.push_back(std::move(func)); }
    void registerOnScrollFunc(OnScrollFunc func) { m_on_scroll_funcs.push_back(std::move(func)); };
    void registerOnFramebufferSizeFunc(OnFramebufferSizeFunc func) {
        m_on_framebuffer_size_funcs.push_back(std::move(func));
    }
    void registerOnWindowSizeFunc(OnWindowSizeFunc func) { m_on_window_size_funcs.push_back(std::move(func)); }
    void registerOnDropFunc(OnDropFunc func) { m_on_drop_funcs.push_back(std::move(func)); }

private:
    std::shared_ptr<GLContext> m_ctx {};

    std::vector<OnKeyFunc> m_on_key_funcs {};
    std::vector<OnCursorPosFunc> m_on_cursor_pos_funcs {};
    std::vector<OnCursorEnterFunc> m_on_cursor_enter_funcs {};
    std::vector<OnMouseButtonFunc> m_on_mouse_button_funcs {};
    std::vector<OnScrollFunc> m_on_scroll_funcs {};
    std::vector<OnFramebufferSizeFunc> m_on_framebuffer_size_funcs {};
    std::vector<OnWindowSizeFunc> m_on_window_size_funcs {};
    std::vector<OnDropFunc> m_on_drop_funcs {};

    WindowManager(std::shared_ptr<GLContext> ctx_ref);

    void onKey(int key, int scancode, int action, int mods);
    void onCursorPos(double xpos, double ypos);
    void onCursorEnter(bool entered);
    void onMouseButton(int button, int action, int mods);
    void onScroll(double xoffset, double yoffset);
    void onFramebufferSize(int width, int height);
    void onWindowSize(int width, int height);
    void onDrop(int path_count, const char *paths[]);

    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    static void cursorEnterCallback(GLFWwindow *window, int entered);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    static void windowSizeCallback(GLFWwindow *window, int width, int height);
    static void dropCallback(GLFWwindow *window, int path_count, const char *paths[]);
};
