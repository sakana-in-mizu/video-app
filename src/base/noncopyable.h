#pragma once

#define NONCOPYABLE(T) \
    T(const T &)            = delete; \
    T &operator=(const T &) = delete;

class Noncopyable {
public:
    NONCOPYABLE(Noncopyable)

    Noncopyable(Noncopyable &&)           = default;
    Noncopyable &operator=(Noncopyable &) = default;

    virtual ~Noncopyable() {}

protected:
    Noncopyable() = default;
};
