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
    void print_stat(StatDelta delta);

    bool activity(Mode mode, StatDelta delta)
    {
        std::array<int, static_cast<int>(Mode::GameOver) + 1> cntOfActivity{};
        history.push_back(mode);
        return deltaApply(delta);
    }

    bool deltaApply(StatDelta delta)
    {
        stat += delta;

        return stat.health > 0;
    }

    inline int getCount(Mode mode) const
    {
        return cntOfActivity[static_cast<int>(mode)];
    }


    inline PlayLevel get_level() const { return level; }
    inline const std::vector<Mode>& get_history() const { return history; }

    inline void setCertificate(bool is) { isCertificate = is; }


private:  /* history */
    
    std::array<int, static_cast<int>(Mode::GameOver) + 1> cntOfActivity{};
    std::vector<Mode> history;
private:
    bool isCertificate = false;
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
