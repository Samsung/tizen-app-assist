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

#include "WDefine.h"
#include <stdio.h>
#include <string.h>

int app_assist::WVersion_getMajor() { return WAPP_ASSIST_VERSION_MAJOR; }
int app_assist::WVersion_getMinor() { return WAPP_ASSIST_VERSION_MINOR; }
int app_assist::WVersion_getPatch() { return WAPP_ASSIST_VERSION_PATCH; }
const char* app_assist::WVersion_getPre() { return WAPP_ASSIST_VERSION_PRE; }
const char* app_assist::WVersion_get() {
	static char ver[24] = {0,};
	if (ver[0] == '\0') {
		if (strlen(WAPP_ASSIST_VERSION_PRE))
			snprintf(ver, sizeof(ver), "%d.%d.%d.%s", WAPP_ASSIST_VERSION_MAJOR, WAPP_ASSIST_VERSION_MINOR, WAPP_ASSIST_VERSION_PATCH, WAPP_ASSIST_VERSION_PRE);
		else
			snprintf(ver, sizeof(ver), "%d.%d.%d", WAPP_ASSIST_VERSION_MAJOR, WAPP_ASSIST_VERSION_MINOR, WAPP_ASSIST_VERSION_PATCH);
	}

	return ver;
}
