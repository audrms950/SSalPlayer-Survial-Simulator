#pragma once
#include "UserPlayer.h"

#define MAX_ENDING_FLAG 15

enum EventType
{
    et_certificate = 1<<1,
    et_get_job = 1<<2,


    et_parents_hlep = 1 << MAX_ENDING_FLAG,
    et_overwork,
    et_end
};

static inline void enable_evnet(EventType et, uint64_t& events)
{
    events |= (1ULL << static_cast<uint64_t>(et));
}

static inline void disable_event(EventType et, uint64_t& events)
{
    events &= ~(1ULL << static_cast<uint64_t>(et));
}

/* TODO : 업캐스팅으로 엔딩플레그 모아서 엔딩분기 해보자 */
struct EndingFlag 
{
    const int flag;

    EndingFlag(EventType et) : flag((int)et) {}
};

struct Event_Interface
{
    Event_Interface() = default;
    virtual ~Event_Interface() = default;
    virtual bool tryActivate(const UserPlayer& user) const = 0;
    virtual bool statCheck(const StatBlock& stat) const = 0;
    virtual inline const StatDelta& getDelta() const = 0;
    virtual bool activity(UserPlayer& user, uint64_t& events) const = 0;
};

struct Event_parents_help : Event_Interface
{
    const EventType event_no = et_parents_hlep;
    const std::string name = "부모님의 도움";
    const std::string description = "내 힘든 상황을 보더니 부모님이 도와주셨어요!!";
    const StatDelta delta
    {
        50, 50,
        0, 0, 0, 0, 0, 0,
        500000
    };
    int activePercentage = 80;


    bool activity(UserPlayer& user, uint64_t& events) const
    {
        bool result = tryActivate(user);
        if (result)
        {
            disable_event(event_no, events);
            user.deltaApply(delta);
        }

        return result;
    }

    bool tryActivate(const UserPlayer& user) const
    {
        PlayLevel cur = user.get_level();
        int percentageWeight = calPercentageWeight(user);
        
        if (cur > PlayLevel::Hard)
        {
            return false; /* 하드 이상은 부모님의 도움따위 필요 없다. */
        }
   
        return rand() % 100 < activePercentage + percentageWeight
            && statCheck(user.getStatBlock());
    }

    int calPercentageWeight(const UserPlayer& user) const
    {
        return (user.get_level() == PlayLevel::Easy ? 20 : 0)
            + (user.getStatBlock().luck / 10);
    }

    bool statCheck(const StatBlock& stat) const 
    {
        return stat.money == 30000 && stat.health < 10 && stat.condition < 30;
    }

    inline const StatDelta& getDelta() const { return delta; }
};


struct Event_overwork : Event_Interface
{
    const EventType event_no = et_overwork;
    const std::string name = "과로";
    const std::string description = "힘들어 죽겠어.. ";
    const StatDelta delta
    {
        -15, -40,
        -20, -30, -10, -10, 0, -20,
        0
    };

    int activePercentage = 50;


    bool activity(UserPlayer& user, uint64_t& events) const
    {
        bool result = tryActivate(user);
        if (result)
        {
            user.deltaApply(delta);
        }

        return result;
    }

    bool tryActivate(const UserPlayer& user) const
    {
        PlayLevel cur = user.get_level();
        const std::vector<Mode>& history = user.get_history();
        int percentageWeight = calPercentageWeight(user);
        int cnt_weightTime = 1;

        int cnt = 0; 
        for (auto it = history.rbegin(); it != history.rend(); ++it)
        {
            if (*it >= Mode::ActivityStart && *it <= Mode::ActivityModeEnd)
            {
                cnt++;
            }
            else
            {
                break;
            }
        }

        if (cur >= PlayLevel::Hard)
        {
            percentageWeight += 5; /* 하드 이상은 발동 확률이 올라감 */
            cnt_weightTime = static_cast<int>(PlayLevel::Hard);
        }

        int finalPercentage = activePercentage + percentageWeight;
        finalPercentage += (cnt * cnt_weightTime);

        if (finalPercentage > 95)
        {
            finalPercentage = 95;
        }

        return cnt >= 3 && rand() % 100 < finalPercentage
            && statCheck(user.getStatBlock());
    }

    int calPercentageWeight(const UserPlayer& user) const
    {
        
        return (user.getStatBlock().willpower/8) * -1;
    }

    bool statCheck(const StatBlock& stat) const
    {
        return (stat.condition <= 50);
    }

    inline const StatDelta& getDelta() const { return delta; }
};



struct Event_certificate : Event_Interface, EndingFlag
{
    const EventType event_no = et_certificate;
    const std::string name = "자격증 응시";
    const std::string description = "자격증 시험!!";
    const StatDelta delta
    {
        0, 0,
        0, 0, 0, 0, 0, 0,
        -50000 /* 응시료 */
    };

    int activePercentage = 10; /* 기본은 낮고 가중치 비율이 큼 */
    Event_certificate() : EndingFlag(et_certificate) {}

    bool activity(UserPlayer& user, uint64_t& events) const
    {
        bool result = tryActivate(user);
        if (result)
        {
            user.deltaApply(delta);
            disable_event(event_no, events);
        }

        return result;
    }

    bool tryActivate(const UserPlayer& user) const
    {
        PlayLevel cur = user.get_level();
        const std::vector<Mode>& history = user.get_history();
        bool condition = false;
        int percentageWeight = activePercentage + calPercentageWeight(user);
        int studyCnt = user.getCount(Mode::Study);

        int lazyScore = 0;

        for (int lazy = (int)Mode::LazyStart; lazy < (int)Mode::LazyEnd; ++lazy)
        {
            lazyScore += user.getCount((Mode)lazy);
        }

        condition = (studyCnt >= 10 && studyCnt - lazyScore > 5);

        if (cur >= PlayLevel::Hard)
        {
            percentageWeight -= 5;
        }

        return condition && rand() % 100 < percentageWeight && statCheck(user.getStatBlock());
    }

    int calPercentageWeight(const UserPlayer& user) const
    {
        /* 조건이 빡센만큼 가중치는 크게주자 */
        return (user.getStatBlock().luck / 10)          // 1. 잘 찍어봐
            + (user.getStatBlock().condition / 20)      // 2. 컨디션 괜찮지?
            + (user.getCount(Mode::Study))              // 3. 공부는 열심히 했고?
            + (user.getStatBlock().intelligence / 20);  // 4. 똑똑하지?
    }

    bool statCheck(const StatBlock& stat) const
    {
        return (stat.willpower >= 180)          // 1. 의지는 있고?
            && (stat.condition >= 100)          // 2. 컨디션도 좋아야지
            && (stat.intelligence >= 200)       // 3. 네 실력으로 붙을 수 있겠어?
            && (stat.money >= 50000);           // 4. 응시료는 기본이야
            
    }

    inline const StatDelta& getDelta() const { return delta; }

};
  