#ifndef __DANNY__CORE__TIMER__
#define __DANNY__CORE__TIMER__

#include <chrono>

namespace danny
{
    namespace core
    {
        class Timer
        {
        public:
            void start();
            double getTime();

        private:
            std::chrono::time_point<std::chrono::system_clock> m_start_time{std::chrono::system_clock::now()};
        };
    }
}

#endif