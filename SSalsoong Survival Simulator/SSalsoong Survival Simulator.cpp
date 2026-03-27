#include "UserPlayer.h"
#include "GameManager.h"
#define con_clear std::cout << "\033[2J\033[H"

int main()
{
	GameManager game;

	game.process();

	return 0;
}


///* 메인 */
//int main()
//{
//    vector<MenuItem> menu =
//    {
//        { "아무것도 안함", Mode::Idle },
//        { "휴식", Mode::Rest },
//        { "잠자기", Mode::Sleep },
//        { "공부", Mode::Study },
//        { "일", Mode::Work },
//        { "운동", Mode::Exercise },
//        { "사교", Mode::Social },
//        { "종료", Mode::GameOver }
//    };
//
//    UserPlayer user(Gender::Female, PlayLevel::Easy);
//
//
//    while (true)
//    {
//        
//        int selectedIndex = selectMenu(menu, user);
//        Mode selectedMode = menu[selectedIndex].mode;
//
//        system("cls");
//
//        if (user.activity(selectedMode, getDeltaByMode(selectedMode)))
//        {
//            user.print_stat(getDeltaByMode(selectedMode));
//            cout << getModeDescription(selectedMode) << "\n";
//
//
//
//            if (selectedMode == Mode::GameOver)
//            {
//                break;
//            }
//
//            cout << "\n계속하려면 아무 키나 누르세요...";
//            _getch();
//        }
//        else
//        {
//            cout << "\n 사망....";
//            break;
//        }
//    }
//
//    return 0;
//}
