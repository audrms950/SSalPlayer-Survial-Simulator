#pragma once
#include "defined.h"
#include <vector>
#include <iostream>
#include <iomanip>

static inline void setPlayerStat(playerStat& target, int value);
static inline void setMoney(Money& target, int64_t value);

namespace stat_range
{
    constexpr uint8_t STATE_MAX_VALUE = 255;
    constexpr playerStat ADDED_MAX_GENDER = 60;
};

#pragma pack(push, 1)
struct StatBlock
{
    /* 생존 관련 기본 상태 */
    playerStat health = 100;        /* 체력 : 0이 되면 게임오버, 행동 수행 시 감소 */
    playerStat condition = 100;     /* 컨디션 : 행동 효율 및 실패 확률에 영향 */

    /* 성장 / 능력치 계열 */
    playerStat responsibility = 10;  /* 책임감 : 일정/업무 이벤트 성공률 증가 */
    playerStat willpower = 20;       /* 의지력 : 패널티 저항 (피로, 스트레스 감소 완화) */
    playerStat intelligence = 30;    /* 지능 : 학습/업무 효율 증가 */
    playerStat sociability = 40;     /* 사교성 : 인간관계 이벤트 성공률 증가 */
    playerStat luck = 20;            /* 운 : 랜덤 이벤트 결과 보정 */
    playerStat immersion = 40;       /* 몰입도 : 특정 행동 지속 시 보너스 효과 */

    /* 자원 */
    Money money = 500000;                /* 보유 금액 : 아이템 구매, 이벤트 선택 등에 사용 */

    /* 스탯 총 합 (돈 제외) */
    uint32_t toScore() const
    {
        return static_cast<uint32_t>(health)
            + static_cast<uint32_t>(condition)
            + static_cast<uint32_t>(responsibility)
            + static_cast<uint32_t>(willpower)
            + static_cast<uint32_t>(intelligence)
            + static_cast<uint32_t>(sociability)
            + static_cast<uint32_t>(luck)
            + static_cast<uint32_t>(immersion);
    }

    /* 스탯 구조체 바이트 크기 */
    uint32_t toSize() const
    {
        return sizeof(health)
            + sizeof(condition)
            + sizeof(responsibility)
            + sizeof(willpower)
            + sizeof(intelligence)
            + sizeof(sociability)
            + sizeof(luck)
            + sizeof(immersion)
            + sizeof(money);
    }

	void applyStat(const StatDelta& delta)
	{
		setPlayerStat(health, delta.health);

		setPlayerStat(condition, delta.condition);

		setPlayerStat(responsibility, delta.responsibility);

		setPlayerStat(willpower, delta.willpower);

		setPlayerStat(intelligence, delta.intelligence);

		setPlayerStat(sociability, delta.sociability);

		setPlayerStat(luck, delta.luck);

		setPlayerStat(immersion, delta.immersion);

		setMoney(money, delta.money);
	}

    void operator+=(const StatDelta& rhs)
    {
        applyStat(rhs);
    }

};
#pragma pack(pop)


/* 스탯 직렬화
(이어하기 저장용)
 TODO : 이어하기 만들기 
 */
static inline Stat toSerialize(const StatBlock& state)
{
    uint32_t statSize = state.toSize();
    Stat serializedData;

    serializedData.resize(statSize);
    std::memcpy(serializedData.data(), &state, statSize);

    return serializedData;
}

static inline StatBlock toStatBlock(const std::vector<playerStat>& target)
{
    StatBlock st;

    if (target.size() != st.toSize())
    {
        /* 사이즈 불일치 - 안전하게 처리 */
        return st;
    }

    std::memcpy(&st, target.data(), st.toSize());

    return st;
}

/* 직렬화된 바이트 데이터 출력  */
static inline void printSerializedData(const std::vector<playerStat>& serializedData)
{
    std::cout << "serialized bytes (" << serializedData.size() << " bytes)" << std::endl;

    for (size_t index = 0; index < serializedData.size(); ++index)
    {
        std::cout
            << "  ["
            << std::setw(2) << index
            << "] 0x"
            << std::hex
            << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(serializedData[index])
            << std::dec
            << std::setfill(' ')
            << std::endl;
    }
}


/* StatBlock 내부 값 출력 */
static inline void printStatBlock(const StatBlock& state, const char* title)
{
    std::cout << title << std::endl;
    std::cout << "  health         : " << static_cast<int>(state.health) << std::endl;
    std::cout << "  condition      : " << static_cast<int>(state.condition) << std::endl;
    std::cout << "  responsibility : " << static_cast<int>(state.responsibility) << std::endl;
    std::cout << "  willpower      : " << static_cast<int>(state.willpower) << std::endl;
    std::cout << "  intelligence   : " << static_cast<int>(state.intelligence) << std::endl;
    std::cout << "  sociability    : " << static_cast<int>(state.sociability) << std::endl;
    std::cout << "  luck           : " << static_cast<int>(state.luck) << std::endl;
    std::cout << "  immersion      : " << static_cast<int>(state.immersion) << std::endl;
    std::cout << "  money          : " << state.money << std::endl;
}

static inline void setPlayerStat(playerStat& target, int value)
{
    int result = static_cast<int>(target) + value;

    if (result < 0)
    {
        result = 0;
    }
    else if (result > stat_range::STATE_MAX_VALUE)
    {
        result = stat_range::STATE_MAX_VALUE;
    }

    target = static_cast<playerStat>(result);
}

static inline void setMoney(Money& target, int64_t value)
{
    int64_t result = static_cast<int64_t>(target) + value;

    if (result < 0)
    {
        result = 0;
    }

    target = static_cast<Money>(result);
}

/* 변화 출력 헬퍼 */
static inline void printStatChange(const char* name, int delta, int before, int after)
{
#ifdef STAT_DEBUG

    if (delta == 0)
    {
        return;
    }

    std::cout
        << name
        << " : "
        << before
        << " -> "
        << after
        << " ("
        << (delta > 0 ? "+" : "")
        << delta
        << ")"
        << std::endl;
#endif
}

static inline void printStatWithDelta(const StatBlock& stat, const StatDelta& delta)
{
    auto printLine = [](const char* name, int value, int deltaValue)
    {
        std::cout << name << " : " << value;

        if (deltaValue != 0)
        {
            std::cout << " ("
                << (deltaValue > 0 ? "+" : "")
                << deltaValue
                << ")";
        }

        std::cout << std::endl;
    };

    printLine("health        ", stat.health, delta.health);
    printLine("condition     ", stat.condition, delta.condition);
    printLine("responsibility", stat.responsibility, delta.responsibility);
    printLine("willpower     ", stat.willpower, delta.willpower);
    printLine("intelligence  ", stat.intelligence, delta.intelligence);
    printLine("sociability   ", stat.sociability, delta.sociability);
    printLine("luck          ", stat.luck, delta.luck);
    printLine("immersion     ", stat.immersion, delta.immersion);

    std::cout << "money         : " << stat.money;
    if (delta.money != 0)
    {
        std::cout << " ("
            << (delta.money > 0 ? "+" : "")
            << delta.money
            << ")";
    }
    std::cout << std::endl;
}