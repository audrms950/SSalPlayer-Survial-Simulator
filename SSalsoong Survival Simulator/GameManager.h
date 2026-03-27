#pragma once
#include "UserPlayer.h"



/* 메뉴 아이템 */
struct MenuItem
{
    string text;
    Mode mode;
};

class GameManager
{
private : /* 일단 대충 채워두고 나중에 리팩토링 */
    /* 기본 상태 */
    StatDelta Start
    {
        0, 0,
        0, 0, 0, 0, 0, 0,
        0
    };
    /* 기본 상태 */
    StatDelta Idle
    {
        -2, -5,
        0, 0, 0, 0, 0, 0,
        0
    };

    StatDelta Rest
    {
        +10, +20,
        0, 5, 0, 0, 0, 0,
        0
    };

    StatDelta Sleep
    {
        +30, +40,
        0, -5, 0, 0, 0, 0,
        0
    };

    /* 성장 / 활동 */
    StatDelta Study
    {
        -5, -15,
        5, 10, 15, 0, 0, 10,
        0
    };

    StatDelta Work
    {
        -10, -20,
        10, 5, 0, -5, 0, 5,
        50000
    };

    StatDelta Exercise
    {
        -10, -10,
        5, 10, 0, 0, 0, 5,
        0
    };

    StatDelta Social
    {
        -5, -10,
        0, 5, 0, 15, 5, 5,
        0
    };

    /* 특수 상태 */
    StatDelta Event
    {
        0, 0,
        0, 0, 0, 0, 10, 0,
        10000
    };

    StatDelta Crisis
    {
        -20, -30,
        -10, -10, 0, -5, -5, 0,
        0
    };

    StatDelta Burnout
    {
        -15, -40,
        -20, -30, -10, -10, 0, -20,
        0
    };

    /* 종료 */
    StatDelta GameOver
    {
        0, 0,
        0, 0, 0, 0, 0, 0,
        0
    };

    StatDelta getDeltaByMode(Mode mode)
    {
        switch (mode)
        {
        case Mode::Idle: return Idle;
        case Mode::Rest: return Rest;
        case Mode::Sleep: return Sleep;
        case Mode::Study: return Study;
        case Mode::Work: return Work;
        case Mode::Exercise: return Exercise;
        case Mode::Social: return Social;
        case Mode::Event: return Event;
        case Mode::Crisis: return Crisis;
        case Mode::Burnout: return Burnout;
        default: return {};
        }
    }

    std::string modeToString(Mode mode)
    {
        switch (mode)
        {
        case Mode::None:      return "None";

            /* 기본 상태 */
        case Mode::Idle:      return "Idle";
        case Mode::Rest:      return "Rest";
        case Mode::Sleep:     return "Sleep";

            /* 성장 / 활동 */
        case Mode::Study:     return "Study";
        case Mode::Work:      return "Work";
        case Mode::Exercise:  return "Exercise";
        case Mode::Social:    return "Social";

            /* 특수 상태 */
        case Mode::Event:     return "Event";
        case Mode::Crisis:    return "Crisis";
        case Mode::Burnout:   return "Burnout";

            /* 종료 */
        case Mode::GameOver:  return "GameOver";

        default:              return "Unknown";
        }
    }


    /* 메뉴 출력 */
    void drawMenu(const vector<MenuItem>& menu, int selectedIndex, const UserPlayer& user)
    {
        system("cls");

        cout << "==== 행동 선택 ====\n\n";
        printStatBlock(user.getStatBlock(), "스탯");
        cout << "\n\n";
        for (int i = 0; i < menu.size(); ++i)
        {
            if (i == selectedIndex)
            {
                cout << Color::GREEN << "> " << menu[i].text << Color::RESET << "\n";
            }
            else
            {
                cout << "  " << menu[i].text << "\n";
            }
        }
    }

    int selectedIndex = 0;

    /* 선택 UI */
    int selectMenu(const vector<MenuItem>& menu, const UserPlayer& user)
    {


        while (true)
        {
            drawMenu(menu, selectedIndex, user);

            int key = _getch();

            if (key == 224)
            {
                key = _getch();

                switch (key)
                {
                case 72: /* ↑ */
                    if (selectedIndex > 0)
                        selectedIndex--;
                    break;

                case 80: /* ↓ */
                    if (selectedIndex < menu.size() - 1)
                        selectedIndex++;
                    break;
                }
            }
            else if (key == 13) /* ENTER */
            {
                return selectedIndex;
            }
        }
    }

    vector<MenuItem> menu =
    {
        { "아무것도 안함", Mode::Idle },
        { "휴식", Mode::Rest },
        { "잠자기", Mode::Sleep },
        { "공부", Mode::Study },
        { "일", Mode::Work },
        { "운동", Mode::Exercise },
        { "사교", Mode::Social },
        { "종료", Mode::GameOver }
    };

    UserPlayer user = UserPlayer(Gender::Female, PlayLevel::Easy);

public :
    void process()
    {
        while (true)
        {

            int selectedIndex = selectMenu(menu, user);
            Mode selectedMode = menu[selectedIndex].mode;

            system("cls");

            if (user.activity(selectedMode, getDeltaByMode(selectedMode)))
            {
                user.print_stat(getDeltaByMode(selectedMode));
                cout << getModeDescription(selectedMode) << "\n";



                if (selectedMode == Mode::GameOver)
                {
                    break;
                }

                cout << "\n계속하려면 아무 키나 누르세요...";
                _getch();
            }
            else
            {
                cout << "\n 사망....";
                break;
            }
        }

    }
public :
};

