#pragma once
#include "PlayerBase.h"
class UserPlayer : public PlayerBase
{
public:
    UserPlayer(Gender gender, PlayLevel level) : gender(gender), level(level)
    {
        initStatBlock(); /* 스탯 초기화 */
        turnModifier = getDifficultyModifier(level); /* 가중치 적용 */
    }
    void print_stat(StatDelta delta)
    {
        printStatWithDelta(stat, delta);
    }

    bool turn(StatDelta delta)
    {
        stat += delta;

        return stat.health > 0;
    }

    /* 외부로 나가는 public은 로우 스네이크 케이스로 구성 */
    void print_stat()
    {
        printStatBlock(stat, "cur state");
    }

private:
    Gender gender;
    PlayLevel level;
    TurnModifier turnModifier;
    TurnDelta turnDelta;

protected:

    void initStatBlock() override
    {
        setStatOfGender();
        setStatOfLevel();
    }


private:
    void setStatOfGender();
    void setStatOfLevel();
};
