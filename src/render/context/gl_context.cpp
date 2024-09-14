#include "gl_context.h"

#include "log/log_system.h"
#include "window_manager.h"

std::weak_ptr<GLContext::GLFWOwnership> GLContext::s_glfw_existence {};

thread_local std::weak_ptr<GLContext> GLContext::s_current_context {};

GLContext::GLFWOwnership::GLFWOwnership() {
    if (!glfwInit()) {
        FATAL("failed to initialize GLFW!");
    }
}

GLContext::GLFWOwnership::~GLFWOwnership() {
    glfwTerminate();
    DEBUG("shutdown GLFW");
}

void GLContext::getGLFWOwnership() {
    m_glfw_ownership = s_glfw_existence.lock();
    if (!m_glfw_ownership) {
        m_glfw_ownership = std::make_shared<GLFWOwnership>();
        s_glfw_existence = m_glfw_ownership;
    }
    DEBUG("GLFW owner: {}", m_glfw_ownership.use_count());
}

GLContext::GLContext(const WindowInfo &info, bool visible) : m_visible(visible) {
    getGLFWOwnership();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_VISIBLE, visible);

    m_window = glfwCreateWindow(info.width, info.height, info.title.c_str(), nullptr, nullptr);
    if (!m_window) {
        FATAL("failed to create window and GL context!");
    }

    makeCurrentContext();
    int version = gladLoadGLContext(&m_gl, reinterpret_cast<GLADloadfunc>(glfwGetProcAddress));
    if (!version) {
        FATAL("failed to load function pointers!");
    }
    INFO("loaded OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    resetCurrentContext();
}

GLContext::~GLContext() {
    glfwDestroyWindow(m_window);
    DEBUG("release GLContext: {}", (void *)this);
}

std::shared_ptr<GLContext> GLContext::createWithWindow(const WindowInfo &info, bool visible) {
    return std::shared_ptr<GLContext> {new GLContext {info, visible}};
}

void GLContext::resetCurrentContext() {
    glfwMakeContextCurrent(nullptr);
    s_current_context.reset();
}

void GLContext::makeCurrentContext() {
    if (!m_window) {
        FATAL("invalid context!");
    }

    glfwMakeContextCurrent(m_window);
    s_current_context = weak_from_this();
}

void GLContext::swapBuffers() const {
    if (!m_window) {
        FATAL("invalid context!");
    }

    glfwSwapBuffers(m_window);
}

std::shared_ptr<WindowManager> GLContext::createWindowManager() {
    return std::shared_ptr<WindowManager> {new WindowManager {shared_from_this()}};
}
