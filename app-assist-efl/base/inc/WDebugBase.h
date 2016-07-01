/*
 * Copyright (c) 2014-2016 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef _APP_ASSIST_WDEBUG_BASE_H_
#define _APP_ASSIST_WDEBUG_BASE_H_

#ifndef LOG_TAG
#define LOG_TAG "WAPP"
#endif

#include <dlog.h>
#include <string.h>
#include <assert.h>
#include "WDefine.h"

// If it is set as 1, assert() is called in WERROR
#define WENABLE_ASSERT_IN_ERROR (0)

#define WCOLOR_RED "\033[0;31m"
#define WCOLOR_GREEN "\033[0;32m"
#define WCOLOR_BROWN "\033[0;33m"
#define WCOLOR_BLUE "\033[0;34m"
#define WCOLOR_PURPLE "\033[0;35m"
#define WCOLOR_CYAN "\033[0;36m"
#define WCOLOR_LIGHTBLUE "\033[0;37m"
#define WCOLOR_END		"\033[0;m"

// normal log
#define _WFILE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/')+1 : __FILE__)

#define _WDLOG_PRINT( prio, fmt, args... ) \
	dlog_print( prio, LOG_TAG, "%s: %s(%d) > " fmt, _WFILE_NAME, __func__, __LINE__, ##args)

#define WDEBUG(fmt, args...) _WDLOG_PRINT(DLOG_DEBUG, fmt, ##args)
#define WDEBUG_(fmt, args...) _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_RED fmt WCOLOR_END, ##args) // temporal red-colored text for debugging

#define WVERBOSE(fmt, args...) _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_BROWN fmt WCOLOR_END, ##args)
#define WINFO(fmt, args...) _WDLOG_PRINT(DLOG_INFO, WCOLOR_GREEN fmt WCOLOR_END, ##args)
#define WWARNING(fmt, args...) _WDLOG_PRINT(DLOG_WARN, WCOLOR_PURPLE fmt WCOLOR_END, ##args)
//
#if (WENABLE_ASSERT_IN_ERROR == 1)
#define WERROR(fmt, args...) do { \
			_WDLOG_PRINT(DLOG_ERROR, WCOLOR_RED "* Critical * " fmt WCOLOR_END, ##args); \
			assert(0);	\
		} while(0)
#else
#define WERROR(fmt, args...) _WDLOG_PRINT(DLOG_ERROR, WCOLOR_RED "* Critical * " fmt WCOLOR_END, ##args)
#endif

#define WHIT() _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_BLUE ">>>HIT<<<" WCOLOR_END)
#define WHIT_() _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_RED ">>>HIT<<<" WCOLOR_END)
#define WHITM(fmt, args... ) _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_BLUE ">>>HIT<<<" fmt WCOLOR_END, ##args)

#define WENTER() _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_GREEN "BEGIN >>>>" WCOLOR_END)
#define WLEAVE() _WDLOG_PRINT(DLOG_DEBUG, WCOLOR_GREEN "END <<<<" WCOLOR_END)

// secure log
#define WSDEBUG(fmt, args...)
#define WSVERBOSE(fmt, args...)
#define WSINFO(fmt, args...)
//
#define WSERROR(fmt, args...)

#define WPRET_VM(expr, val, fmt, arg...) do { \
					if (expr) { \
						WERROR(fmt, ##arg); \
						return (val); \
						} \
					} while (0)

#define WPRET_M(expr, fmt, arg...) do { \
					if (expr) { \
					WERROR(fmt, ##arg); \
					return; \
					} \
				} while (0)

#define WASSERT(expr) do { \
		if(!(expr)) \
		{\
 			WERROR( "WASSERT! "#expr );\
			assert((expr)); \
		}\
	}while(0);

#define WASSERT_EX(expr, fmt, args... ) do { \
		if(!(expr)) \
		{\
 			WERROR( "WASSERT! " fmt, ##args);\
			assert((expr)); \
		}\
	}while(0);

#endif /* _APP_ASSIST_WDEBUG_BASE_H_ */
