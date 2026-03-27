#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include <array>
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>

/* 콘솔창 clear */

using namespace std;


using playerStat = uint8_t;
using Money = uint64_t;
using Stat = std::vector<playerStat>;

/* ANSI 색상 코드 */
namespace Color
{
    const std::string RED = "\033[0;31m";
    const std::string GREEN = "\033[0;32m";
    const std::string YELLOW = "\033[0;33m";
    const std::string BLUE = "\033[0;34m";
    const std::string MAGENTA = "\033[0;35m";
    const std::string CYAN = "\033[0;36m";
    const std::string WHITE = "\033[0;37m";
    const std::string RESET = "\033[0m";
}

enum class Gender
{
    Unknown = 0,
    Male,
    Female,
    NonBinary
};

enum class PlayLevel
{
    Easy = 0,      /* 완화된 생존 난이도 */
    Normal,        /* 기본 밸런스 */
    Hard,          /* 자원 부족 + 패널티 증가 */
    Hardcore       /* 매우 높은 난이도 (실패 여지 적음) */
};

enum class Mode
{
    None = 0,

    /* 기본 상태 */
    LazyStart,
    Idle = LazyStart,           /* 아무 행동도 안 하는 상태 */
    Rest,           /* 휴식 (체력/컨디션 회복) */
    Sleep,          /* 수면 (대량 회복, 시간 소비 큼) */
    LazyEnd,

    /* 성장 / 활동 */
    ActivityStart,
    Study = ActivityStart,          /* 공부 (intelligence 증가) */
    Work,           /* 업무 / 돈 획득 */
    Exercise,       /* 운동 (health/condition 영향) */
    Social,         /* 인간관계 활동 */
    ActivityModeEnd,

    /* 특수 상태 */
    Event,          /* 랜덤 이벤트 진행 중 */
    Crisis,         /* 위기 상태 (health 낮음 등) */
    Burnout,        /* 번아웃 상태 */

    /* 종료 */
    GameOver        /* 게임 종료 */
};

inline std::string getModeDescription(Mode mode)
{
    switch (mode)
    {
        case Mode::Idle:
            return Color::WHITE + "아무것도 하지 않았다" + Color::RESET;

        case Mode::Rest:
            return Color::GREEN + "휴식을 취했다" + Color::RESET;

        case Mode::Sleep:
            return Color::CYAN + "잠을 잤다" + Color::RESET;

        case Mode::Study:
            return Color::BLUE + "공부를 했다" + Color::RESET;

        case Mode::Work:
            return Color::YELLOW + "일을 했다" + Color::RESET;

        case Mode::Exercise:
            return Color::MAGENTA + "운동을 했다" + Color::RESET;

        case Mode::Social:
            return Color::CYAN + "사람들을 만났다" + Color::RESET;

        case Mode::Event:
            return Color::YELLOW + "특수 이벤트가 발생했다" + Color::RESET;

        case Mode::Crisis:
            return Color::RED + "위기 상태에 빠졌다" + Color::RESET;

        case Mode::Burnout:
            return Color::MAGENTA + "번아웃 상태다" + Color::RESET;

        case Mode::GameOver:
            return Color::RED + "게임이 종료되었다" + Color::RESET;

        default:
            return Color::RED + "알 수 없는 상태" + Color::RESET;
    }
}
/* 난이도 가중치 */
struct TurnModifier
{
    float healthDecayRate;      /* 체력 감소 배율 */
    float conditionDecayRate;   /* 컨디션 감소 배율 */
    float moneyGainRate;        /* 돈 획득 배율 */
    float statGainRate;         /* 스탯 상승 배율 */
    float eventPenaltyRate;     /* 이벤트 패널티 배율 */
};

/* 턴마다 줄어드는 값 */
struct TurnDelta
{
    int health = 0;
    int condition = 0;
    int money = 0;
    int responsibility = 0;
    int willpower = 0;
    int intelligence = 0;
    int sociability = 0;
    int luck = 0;
    int immersion = 0;
};

struct StatDelta
{
    int health = 0;
    int condition = 0;

    int responsibility = 0;
    int willpower = 0;
    int intelligence = 0;
    int sociability = 0;
    int luck = 0;
    int immersion = 0;

    int64_t money = 0;
};

