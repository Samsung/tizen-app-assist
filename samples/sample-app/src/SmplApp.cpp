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

#include "SmplApp.h"
#include <libintl.h>
#include <string>

#include "WWindowController.h"
#include "WNaviframeController.h"
#include "WSignal.h"

#include "SmplMainListViewController.h"
#include "SmplSignalManager.h"
#include "SmplDebug.h"
#include "SmplDefine.h"

using namespace app_assist;

SmplApp::SmplApp() {
}

SmplApp::~SmplApp() {
}

void SmplApp::onPause() {
	WHIT();
	SmplSignalManager::getInstance()->getAppStateSignal().emit("pause");
}

void SmplApp::onResume() {
	WHIT();
	SmplSignalManager::getInstance()->getAppStateSignal().emit("resume");
}

bool SmplApp::onCreate() {
	_regionFormatChangedHandler.addEventHandler(APP_EVENT_REGION_FORMAT_CHANGED, [](app_event_info_h eventInfo, void* userData) {
		char* region = nullptr;

		app_event_get_region_format(eventInfo, &region);
		WDEBUG("changed region=%s", region);

		free(region);

	}, this);
	_languageChangedHandler.addEventHandler(APP_EVENT_LANGUAGE_CHANGED, [](app_event_info_h eventInfo, void* userData) {
		char* lang = nullptr;

		app_event_get_language(eventInfo, &lang);
		WDEBUG("changed language=%s", lang);

		elm_language_set(lang);
		free(lang);

	}, this);

	attachWindowController(new WWindowController("sample-app", ELM_WIN_BASIC));

	WNaviframeController* navi = new WNaviframeController();
	navi->setOnLastItemPop([](bool * popOut) {
		*popOut = true;
		elm_exit();
	});
	getWindowController()->attachBaseViewController(navi);
	return true;
}

void SmplApp::onTerminate() {
	_regionFormatChangedHandler.removeEventHandler();
	_languageChangedHandler.removeEventHandler();

	SmplSignalManager::destroy();
}

void SmplApp::onAppControl(app_control_h request, bool firstLaunch) {
	WENTER();

	if (firstLaunch) {
		WDEBUG("it is first launch!");
		//
		WNaviframeController* frame = (WNaviframeController*)getWindowController()->getBaseViewController();
		frame->push(new SmplMainListViewController());
	}

	WLEAVE();
}
