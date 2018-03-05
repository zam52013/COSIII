/*
 **************************************************************************
 * @file    prefs.h
 * @author  Feilong Wang
 * @version V0.9.3
 * @date    Jul 16, 2016
 **************************************************************************
 * @attetion
 *
 * Copyright(c) 2016 QXWZ Corporation.  All rights reserved.
 */
#ifndef __QXWZ_PREFS_H
#define __QXWZ_PREFS_H

typedef int qxwz_prefs_flags_t;

/** 
 * A flag to enable sdk log.
 */
#define QXWZ_PREFS_FLAG_ENABLE_LOG                   (1 << 0)
/**
 * A flag to enable underlying connecting to remote through ip.
 */
#define QXWZ_PREFS_FLAG_CONNECT_USING_IP             (1 << 1)


/**
 * socket处理是否全异步
 */
#define QXWZ_PREFS_FLAG_SOCKET_ASYN                  (1 << 2)


/**
 * A set of flags to set up sdk behavior.
 */
void qxwz_prefs_flags_set(qxwz_prefs_flags_t flags);

/**
 * Return the sdk internal behavior flags.
 */
qxwz_prefs_flags_t qxwz_prefs_flags_get(void);

#endif // __QXWZ_PREFS_H
