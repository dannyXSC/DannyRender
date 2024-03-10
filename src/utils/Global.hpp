#ifndef __DANNY__XML__PARSER__
#define __DANNY__XML__PARSER__

#include <iostream>

namespace danny
{
    namespace utils
    {
        inline void UpdateProgress(float progress)
        {
            int barWidth = 70;

            std::cout << "[";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i)
            {
                if (i < pos)
                    std::cout << "=";
                else if (i == pos)
                    std::cout << ">";
                else
                    std::cout << " ";
            }
            std::cout << "] " << int(progress * 100.0) << " %\r";
            std::cout.flush();
        };
    }
}

#endif