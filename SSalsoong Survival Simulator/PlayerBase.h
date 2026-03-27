#pragma once
#include "SSalPlayerStat.h"
#include <cstdint>
#include <cassert>

class PlayerBase
{
public:
    PlayerBase() = default;
    virtual ~PlayerBase() = default;

    const StatBlock& getStatBlock() const
    {
        return stat;
    }

    Stat getSerializedStat() const
    {
        return toSerialize(stat);
    }
private:
    uint16_t todaySeq = 0; 

protected:
    StatBlock stat;
    virtual void initStatBlock() = 0;

    inline TurnModifier getDifficultyModifier(PlayLevel difficulty)
    {
        switch (difficulty)
        {
        case PlayLevel::Easy:
            return { 0.7f, 0.7f, 1.5f, 1.2f, 0.5f };

        case PlayLevel::Normal:
            return { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

        case PlayLevel::Hard:
            return { 1.3f, 1.3f, 0.8f, 0.9f, 1.5f };

        case PlayLevel::Hardcore:
            return { 1.7f, 1.7f, 0.5f, 0.7f, 2.0f };

        default:
            return { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
        }
    }
private: 
};