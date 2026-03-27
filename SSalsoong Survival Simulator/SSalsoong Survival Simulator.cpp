#include "UserPlayer.h"
#define clear std::cout << "\033[2J\033[H"

StatDelta start
{
    /* health, condition */
    0, 0,

    /* responsibility, willpower, intelligence, sociability, luck, immersion */
    0, /* 책임감 */
    0, /* 의지력 */
    0, /* 지능 */
    0, /* 사회성 */
    0, /* 운 */
    1, /* 몰입도 */

    /* money */
    0
};


StatDelta Normal
{
    /* health, condition */
    -5, -10,

    /* responsibility, willpower, intelligence, sociability, luck, immersion */
    0, /* 책임감 */
    0, /* 의지력 */
    0, /* 지능 */
    0, /* 사회성 */
    0, /* 운 */
    1, /* 몰입도 */

    /* money */
    30000
};

int main()
{
    UserPlayer user(Gender::Female, PlayLevel::Easy);

    StatDelta prev = start;
    int flag = 0;
    while(flag != 99)
    {
        clear;
        user.print_stat(prev);
        std::cout << "99. 종료 " << std::endl;
        std::cout << "입력: ";
        std::cin >> flag;
        std::cout << std::endl;
        prev = Normal;
        if (!user.turn(Normal))
        {
            clear;
            user.print_stat(prev);
            std::cout << "사망" << std::endl;
            break;
        }
        
        
    }

    return 0;
}
