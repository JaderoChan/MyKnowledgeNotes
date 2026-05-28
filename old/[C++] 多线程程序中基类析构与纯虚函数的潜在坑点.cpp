#include <atomic>
#include <thread>

class Base
{
public:
    Base() = default;

    virtual ~Base()
    {
        stop();
    }

    void start()
    {
        shouldClose_.store(false);
        th_ = std::thread([this]() { work(); });
    }

    void stop()
    {
        shouldClose_.store(true);
        if (th_.joinable())
            th_.join();
    }

protected:
    virtual void derivedWork() = 0;

private:
    void work()
    {
        while (!shouldClose_.load())
        {
            // Self works...
            derivedWork();
        }
    }

    std::atomic<bool> shouldClose_{false};
    std::thread th_;
};

class Derived : public Base
{
public:
    Derived()
    {
        start();
    }

    ~Derived()
    {
        // Correct: call `stop()` in here.
        // stop();
    }

protected:
    void derivedWork()
    {
        // Do something.
    }
};

int main(int argc, char* argv[])
{
    Derived derived;

    return 0;
}
