#include "platform-posix.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <gobo/platform/alarm-micro.h>
#include <gobo/platform/alarm-milli.h>

#define MS_PER_S 1000
#define US_PER_MS 1000
#define US_PER_S 1000000

#define DEFAULT_TIMEOUT 10 // seconds

static bool     sIsMsRunning = false;
static uint32_t sMsAlarm     = 0;

static bool     sIsUsRunning = false;
static uint32_t sUsAlarm     = 0;

static uint32_t sSpeedUpFactor = 1;

static struct timeval sStart;

void platformAlarmInit(uint32_t aSpeedUpFactor)
{
    sSpeedUpFactor = aSpeedUpFactor;
    gettimeofday(&sStart, NULL);
}

uint64_t platformGetNow(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    timersub(&tv, &sStart, &tv);

    return (uint64_t)tv.tv_sec * sSpeedUpFactor * US_PER_S + (uint64_t)tv.tv_usec * sSpeedUpFactor;
}

uint32_t goPlatAlarmMilliGetNow(void)
{
    return (uint32_t)(platformGetNow() / US_PER_MS);
}

void goPlatAlarmMilliStartAt(goInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
    GO_UNUSED_VARIABLE(aInstance);

    sMsAlarm     = aT0 + aDt;
    sIsMsRunning = true;
}

void goPlatAlarmMilliStop(goInstance *aInstance)
{
    GO_UNUSED_VARIABLE(aInstance);

    sIsMsRunning = false;
}

uint32_t goPlatAlarmMicroGetNow(void)
{
    return (uint32_t)platformGetNow();
}

void goPlatAlarmMicroStartAt(goInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
    GO_UNUSED_VARIABLE(aInstance);

    sUsAlarm     = aT0 + aDt;
    sIsUsRunning = true;
}

void goPlatAlarmMicroStop(goInstance *aInstance)
{
    GO_UNUSED_VARIABLE(aInstance);

    sIsUsRunning = false;
}

void platformAlarmUpdateTimeout(struct timeval *aTimeout)
{
    int32_t usRemaining = DEFAULT_TIMEOUT * US_PER_S;
    int32_t msRemaining = DEFAULT_TIMEOUT * MS_PER_S;

    if (aTimeout == NULL)
    {
        return;
    }

    if (sIsUsRunning)
    {
        usRemaining = (int32_t)(sUsAlarm - goPlatAlarmMicroGetNow());
    }

    if (sIsMsRunning)
    {
        msRemaining = (int32_t)(sMsAlarm - goPlatAlarmMilliGetNow());
    }

    if (usRemaining <= 0 || msRemaining <= 0)
    {
        aTimeout->tv_sec  = 0;
        aTimeout->tv_usec = 0;
    }
    else
    {
        int64_t remaining = ((int64_t)msRemaining) * US_PER_MS;

        if (usRemaining < remaining)
        {
            remaining = usRemaining;
        }

        remaining /= sSpeedUpFactor;

        if (remaining == 0)
        {
            remaining = 1;
        }

        aTimeout->tv_sec  = (time_t)remaining / US_PER_S;
        aTimeout->tv_usec = remaining % US_PER_S;
    }
}

void platformAlarmProcess(goInstance *aInstance)
{
    int32_t remaining;

    if (sIsMsRunning)
    {
        remaining = (int32_t)(sMsAlarm - goPlatAlarmMilliGetNow());

        if (remaining <= 0)
        {
            sIsMsRunning = false;

            goPlatAlarmMilliFired(aInstance);
        }
    }

#if GOBO_CONFIG_ENABLE_PLATFORM_USEC_TIMER

    if (sIsUsRunning)
    {
        remaining = (int32_t)(sUsAlarm - goPlatAlarmMicroGetNow());

        if (remaining <= 0)
        {
            sIsUsRunning = false;

            goPlatAlarmMicroFired(aInstance);
        }
    }

#endif // GOBO_CONFIG_ENABLE_PLATFORM_USEC_TIMER
}

uint64_t goPlatTimeGet(void)
{
    return platformGetNow();
}

uint16_t goPlatTimeGetXtalAccuracy(void)
{
    return 0;
}
