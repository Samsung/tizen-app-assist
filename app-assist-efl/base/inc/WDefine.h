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

#ifndef _APP_ASSIST_WDEFINE_H_
#define _APP_ASSIST_WDEFINE_H_

#include <memory>

/**
* @file	WDefine.h
* @brief	This file defines version number MACRO and functions.
*
*/

const int WAPP_ASSIST_VERSION_MAJOR	= 1;
const int WAPP_ASSIST_VERSION_MINOR	= 0;
const int WAPP_ASSIST_VERSION_PATCH	= 0;
const char* const WAPP_ASSIST_VERSION_PRE = "";

#define WAPP_ASSIST_EXPORT __attribute__((visibility("default")))

namespace app_assist {
WAPP_ASSIST_EXPORT int WVersion_getMajor();
WAPP_ASSIST_EXPORT int WVersion_getMinor();
WAPP_ASSIST_EXPORT int WVersion_getPatch();
WAPP_ASSIST_EXPORT const char* WVersion_getPre();
WAPP_ASSIST_EXPORT const char* WVersion_get();

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&& ... params) {
	return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
}

#define WDISABLE_COPY_AND_ASSIGN( A ) \
		A( const A& ) = delete; \
		const A& operator=( const A& ) = delete

#define WDECLARE_PRIVATE_IMPL( A ) \
	class _##A##Impl* _pv; \
	friend class _##A##Impl

#endif /* _APP_ASSIST_WDEFINE_H_ */
