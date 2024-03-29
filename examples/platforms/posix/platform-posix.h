#ifndef PLATFORM_POSIX_H_
#define PLATFORM_POSIX_H_

#include <gobo-core-config.h>
#include <gobo/config.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#define POLL poll

#include <gobo/instance.h>

#include "gobo-core-config.h"

extern uint32_t NODE_ID;

extern uint32_t WELLKNOWN_NODE_ID;

void platformAlarmInit(uint32_t aSpeedUpFactor);

void platformAlarmUpdateTimeout(struct timeval *aTimeout);

void platformAlarmProcess(goInstance *aInstance);

uint64_t platformAlarmGetNow(void);

void platformRandomInit(void);

void platformUartUpdateFdSet(fd_set *aReadFdSet, fd_set *aWriteFdSet, fd_set *aErrorFdSet, int *aMaxFd);

void platformUartProcess(void);

void platformUartRestore(void);

#endif // PLATFORM_POSIX_H_
