#pragma once

template<typename T>
class Singleton {
public:
    virtual ~Singleton() {}

    Singleton(const Singleton &) = delete;
    Singleton(Singleton &&)      = delete;

    Singleton &operator=(const Singleton &) = delete;
    Singleton &operator=(Singleton &&)      = delete;

    static T *get() {
        static T self;
        return &self;
    }

protected:
    Singleton() = default;
};

#ifdef __NEVER_DEFINED
class SingletonExample : public Singleton<SingletonExample> {
    friend Singleton<SingletonExample>;

public:
    virtual ~SingletonExample() {}

private:
    SingletonExample() = default;
};
#endif