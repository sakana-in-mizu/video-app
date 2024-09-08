#pragma once

class Noncopyable {
public:
    virtual ~Noncopyable() {}

    Noncopyable(Noncopyable &&)      = default;
    Noncopyable(const Noncopyable &) = delete;

    Noncopyable &operator=(Noncopyable &)       = default;
    Noncopyable &operator=(const Noncopyable &) = delete;

protected:
    Noncopyable() = default;
};