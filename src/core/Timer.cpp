#include "Timer.h"
namespace danny
{
    namespace core
    {
        void Timer::start()
        {
            m_start_time = std::chrono::system_clock::now();
        }

        double Timer::getTime()
        {
            using second = std::chrono::duration<double, std::ratio<1>>;
            return std::chrono::duration_cast<second>(std::chrono::system_clock::now() - m_start_time).count();
        }

        int64_t Timer::nowTime()
        {
            const auto p1 = std::chrono::system_clock::now();
            return std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
        }
    }
}