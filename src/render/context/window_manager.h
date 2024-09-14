#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "base/noncopyable.h"
#include "base/nonmovable.h"

class GLContext;

class GLFWwindow;

class WindowManager {
    friend GLContext;

public:
    using OnKeyFunc = std::function<void(WindowManager *wm, int key, int scancode, int action, int mods)>;

    NONCOPYABLE(WindowManager)
    NONMOVABLE(WindowManager)

    ~WindowManager();

    void pollEvents() const;
    bool shouldClose() const;

    void setShouldClose(bool value) const;

    void registerOnKeyFunc(OnKeyFunc func) { m_on_key_funcs.push_back(std::move(func)); }

private:
    std::shared_ptr<GLContext> m_ctx {};

    std::vector<OnKeyFunc> m_on_key_funcs {};

    WindowManager(std::shared_ptr<GLContext> ctx_ref);

    void onKey(int key, int scancode, int action, int mods);

    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};
