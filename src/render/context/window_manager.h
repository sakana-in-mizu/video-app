#pragma once

#include <memory>

#include "base/noncopyable.h"
#include "base/nonmovable.h"

class GLContext;

class WindowManager {
    friend GLContext;

public:
    NONCOPYABLE(WindowManager)
    NONMOVABLE(WindowManager)

    void pollEvents() const;
    bool shouldClose() const;

private:
    std::weak_ptr<GLContext> m_ctx_ref;

    WindowManager(std::weak_ptr<GLContext> ctx_ref);

    void checkContextLifetime() const;
};
