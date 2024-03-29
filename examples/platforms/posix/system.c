#include "platform-posix.h"

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <libgen.h>
#include <syslog.h>

#include <gobo/platform/alarm-milli.h>
#include <gobo/tasklet.h>

uint32_t NODE_ID           = 1;
uint32_t WELLKNOWN_NODE_ID = 34;

extern bool gPlatformPseudoResetWasRequested;

static volatile bool gTerminate = false;

int    gArgumentsCount = 0;
char **gArguments      = NULL;

static void handleSignal(int aSignal)
{
    (void)aSignal;
    gTerminate = true;
}

void goSysInit(int aArgCount, char *aArgVector[])
{
    char *   endptr;
    uint32_t speedUpFactor = 1;

    if (gPlatformPseudoResetWasRequested)
    {
        gPlatformPseudoResetWasRequested = false;
        return;
    }

    if (aArgCount < 2)
    {
        fprintf(stderr, "Syntax:\n    %s NodeId [TimeSpeedUpFactor]\n", aArgVector[0]);
        exit(EXIT_FAILURE);
    }

    openlog(basename(aArgVector[0]), LOG_PID, LOG_USER);
    setlogmask(setlogmask(0) & LOG_UPTO(LOG_NOTICE));

    gArgumentsCount = aArgCount;
    gArguments      = aArgVector;

    signal(SIGTERM, &handleSignal);
    signal(SIGHUP, &handleSignal);

    NODE_ID = (uint32_t)strtol(aArgVector[1], &endptr, 0);

    if (*endptr != '\0' || NODE_ID < 1 || NODE_ID >= WELLKNOWN_NODE_ID)
    {
        fprintf(stderr, "Invalid NodeId: %s\n", aArgVector[1]);
        exit(EXIT_FAILURE);
    }

    if (aArgCount > 2)
    {
        speedUpFactor = (uint32_t)strtol(aArgVector[2], &endptr, 0);

        if (*endptr != '\0' || speedUpFactor == 0)
        {
            fprintf(stderr, "Invalid value for TimerSpeedUpFactor: %s\n", aArgVector[2]);
            exit(EXIT_FAILURE);
        }
    }

    platformAlarmInit(speedUpFactor);
    platformRandomInit();
}

bool goSysPseudoResetWasRequested(void)
{
    return gPlatformPseudoResetWasRequested;
}

void goSysDeInit(void)
{
}

void goSysProcessDrivers(goInstance *aInstance)
{
    fd_set         read_fds;
    fd_set         write_fds;
    fd_set         error_fds;
    int            max_fd = -1;
    struct timeval timeout;
    int            rval;

    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&error_fds);

    platformUartUpdateFdSet(&read_fds, &write_fds, &error_fds, &max_fd);
    platformAlarmUpdateTimeout(&timeout);

    if (!goTaskletsArePending(aInstance))
    {
        rval = select(max_fd + 1, &read_fds, &write_fds, &error_fds, &timeout);

        if ((rval < 0) && (errno != EINTR))
        {
            perror("select");
            exit(EXIT_FAILURE);
        }
    }

    if (gTerminate)
    {
        exit(0);
    }

    platformUartProcess();
    platformAlarmProcess(aInstance);
}
