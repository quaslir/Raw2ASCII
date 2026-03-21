#include <chrono>

class FPS {
    private:
    int fps;
    std::chrono::steady_clock::time_point last_time;
    int frameCount;
    public:
    void update(void);
    int getfps(void) const;
    FPS();
};