#pragma once

#define NONMOVABLE(T) \
    T(T &&) = delete; \
    T &operator=(T &&) = delete;

class Nonmovable {
public:
    NONMOVABLE(Nonmovable)

    Nonmovable(const Nonmovable &) = default;
    Nonmovable &operator=(const Nonmovable &) = default;

    virtual ~Nonmovable() {}

protected:
    Nonmovable() = default;
};
