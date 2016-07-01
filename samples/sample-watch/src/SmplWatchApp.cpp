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

#include "SmplWatchApp.h"

#include <WWatchWindowController.h>
#include "SmplWatchDebug.h"
#include "SmplWatchFaceViewController.h"

using namespace app_assist;

SmplWatchApp::SmplWatchApp()
	: _watchFaceView(nullptr) {
	WHIT();
}

SmplWatchApp::~SmplWatchApp() {
	WHIT();
}

bool SmplWatchApp::onCreate() {
	WHIT();

	attachWindowController(new WWatchWindowController());

	_watchFaceView = new SmplWatchFaceViewController(getScreenWidth(), getScreenHeight());
	getWindowController()->attachBaseViewController(_watchFaceView);

	return true;
}

void SmplWatchApp::onTerminate() {
	WHIT();
}

void SmplWatchApp::onPause() {
	WHIT();
}

void SmplWatchApp::onResume() {
	WHIT();
}

void SmplWatchApp::onAppControl(app_control_h request, bool firstLaunch) {
	WHIT();
}

void SmplWatchApp::onTimeTick(watch_time_h watchTime) {
	WHIT();

	if (_watchFaceView) {
		int hour, min, sec;
		watch_time_get_hour24(watchTime, &hour);
		watch_time_get_minute(watchTime, &min);
		watch_time_get_second(watchTime, &sec);

		_watchFaceView->update(hour, min, sec);
	}
}

void SmplWatchApp::onAmbientModeTimeTick(watch_time_h watchTime) {
	WHIT();

	if (_watchFaceView) {
		int hour, min;
		watch_time_get_hour24(watchTime, &hour);
		watch_time_get_minute(watchTime, &min);

		_watchFaceView->update(hour, min, 0);
	}
}

void SmplWatchApp::onAmbientModeChanged(bool ambientMode) {
	WHIT();
}
