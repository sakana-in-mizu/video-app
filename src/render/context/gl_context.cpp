#include "gl_context.h"

#include "log/log_system.h"

std::weak_ptr<GLContext::GLFWOwnership> GLContext::s_glfw_existence {};

std::weak_ptr<GLContext> GLContext::s_current_context {};

GLContext::GLFWOwnership::GLFWOwnership() {
    if (!glfwInit()) {
        FATAL("failed to initialize GLFW!");
    }
}

GLContext::GLFWOwnership::~GLFWOwnership() { glfwTerminate(); }

void GLContext::getGLFWOwnership() {
    if (!s_glfw_existence.expired()) {
        m_glfw_ownership = s_glfw_existence.lock();
    } else {
        m_glfw_ownership = std::make_shared<GLFWOwnership>();
        s_glfw_existence = m_glfw_ownership;
    }
    TRACE("GLFW owner: {}.", m_glfw_ownership.use_count());
}

GLContext::~GLContext() { glfwDestroyWindow(m_window); }

std::shared_ptr<GLContext> GLContext::createWithWwindow(const WindowInfo &info) {
    return std::shared_ptr<GLContext> {new GLContext(info)};
}

void GLContext::makeCurrentContext() {
    if (!m_window) {
        FATAL("invalid context!");
    }

    glfwMakeContextCurrent(m_window);
    s_current_context = weak_from_this();
}

void GLContext::detachContext() const {
    glfwMakeContextCurrent(nullptr);
    s_current_context.reset();
}

void GLContext::swapBuffers() const {
    if (!m_window) {
        FATAL("invalid context!");
    }

    glfwSwapBuffers(m_window);
}

GLContext::GLContext(const WindowInfo &info) {
    getGLFWOwnership();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
    detachContext();
}
