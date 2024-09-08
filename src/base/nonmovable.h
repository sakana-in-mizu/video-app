#pragma once

class Nonmovable {
public:
    virtual ~Nonmovable() {}

    Nonmovable(Nonmovable &&)      = delete;
    Nonmovable(const Nonmovable &) = default;

    Nonmovable &operator=(Nonmovable &&)      = delete;
    Nonmovable &operator=(const Nonmovable &) = default;

protected:
    Nonmovable() = default;
};