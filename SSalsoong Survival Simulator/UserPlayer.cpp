#include "UserPlayer.h"

void UserPlayer::setStatOfGender()
{
    StatBlock adjustedStat = stat;

    /* 밸런스를 위해 성별 가중치는 허용 총합 내에서만 적용 */
    uint32_t maxScore = stat.toScore() + stat_range::ADDED_MAX_GENDER;

    switch (gender)
    {
    case Gender::Male:
        setPlayerStat(adjustedStat.health, 10);
        setPlayerStat(adjustedStat.willpower, 10);
        setPlayerStat(adjustedStat.sociability, 5);
        setPlayerStat(adjustedStat.immersion, 5);
        break;

    case Gender::Female:
        setPlayerStat(adjustedStat.intelligence, 10);
        setPlayerStat(adjustedStat.sociability, 10);
        setPlayerStat(adjustedStat.luck, 10);
        break;

    default:
        break;
    }

    /* 스코어 감지 assert */
    assert(adjustedStat.toScore() <= maxScore);

    if (adjustedStat.toScore() <= maxScore)
    {
        stat = adjustedStat;
    }
}

void UserPlayer::setStatOfLevel()
{
    StatBlock adjustedStat = stat;

    switch (level)
    {
    case PlayLevel::Easy:
        setPlayerStat(adjustedStat.health, 10);
        setPlayerStat(adjustedStat.condition, 10);
        setMoney(adjustedStat.money, 50000);
        setPlayerStat(adjustedStat.willpower, 5);
        break;

    case PlayLevel::Normal:
        /* 기본값 유지 */
        break;

    case PlayLevel::Hard:
        setPlayerStat(adjustedStat.health, -5);
        setPlayerStat(adjustedStat.condition, -10);
        setMoney(adjustedStat.money, -50000);
        setPlayerStat(adjustedStat.willpower, -5);
        break;

    case PlayLevel::Hardcore:
        setPlayerStat(adjustedStat.health, -10);
        setPlayerStat(adjustedStat.condition, -15);
        setMoney(adjustedStat.money, -250000);
        setPlayerStat(adjustedStat.willpower, -10);
        setPlayerStat(adjustedStat.luck, -10);
        break;

    default:
        break;
    }

    stat = adjustedStat;
}
