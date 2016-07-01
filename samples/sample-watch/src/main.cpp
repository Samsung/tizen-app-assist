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

#include "SmplWatchDebug.h"
#include "SmplWatchApp.h"

WAPP_ASSIST_EXPORT int main(int argc, char* argv[]) {
	WHIT();

	SmplWatchApp watchApp;
	int ret = watchApp.start(argc, argv);

	WDEBUG("watchApp return=%d", ret);

	return ret;
}
