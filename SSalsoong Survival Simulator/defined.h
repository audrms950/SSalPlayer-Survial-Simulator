#pragma once
#include <cstdint>
#include <vector>

/* 콘솔창 clear */

using playerStat = uint8_t;
using Money = uint64_t;
using Stat = std::vector<playerStat>;


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
    Idle,           /* 아무 행동도 안 하는 상태 */
    Rest,           /* 휴식 (체력/컨디션 회복) */
    Sleep,          /* 수면 (대량 회복, 시간 소비 큼) */

    /* 성장 / 활동 */
    Study,          /* 공부 (intelligence 증가) */
    Work,           /* 업무 / 돈 획득 */
    Exercise,       /* 운동 (health/condition 영향) */
    Social,         /* 인간관계 활동 */

    /* 특수 상태 */
    Event,          /* 랜덤 이벤트 진행 중 */
    Crisis,         /* 위기 상태 (health 낮음 등) */
    Burnout,        /* 번아웃 상태 */

    /* 종료 */
    GameOver        /* 게임 종료 */
};

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