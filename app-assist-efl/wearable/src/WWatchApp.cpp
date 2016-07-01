/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
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

#include "WWatchApp.h"

#include <memory.h>
#include "WDebugInternal.h"
#include "WWatchWindowController.h"
#include "WMenuPopupControllerImpl.h"

using namespace app_assist;

namespace app_assist {

class _WWatchAppImpl {
public:
	_WWatchAppImpl() : _window(nullptr), _firstLaunch(true), _width(0), _height(0) {}
	~_WWatchAppImpl();
public:
	WWatchWindowController* _window;
	bool _firstLaunch;
	int _width;
	int _height;

	static bool _onCreate(int width, int height, void* data);
	static void _onTerminate(void* data);
	static void _onPause(void* data);
	static void _onResume(void* data);
	static void _onAppControl(app_control_h request, void* data);
	static void _onTimeTick(watch_time_h watchTime, void* data);
	static void _onAmbientTick(watch_time_h watchTime, void* data);
	static void _onAmbientChanged(bool ambientMode, void* data);
};
}

_WWatchAppImpl::~_WWatchAppImpl() {
}

bool _WWatchAppImpl::_onCreate(int width, int height, void* data) {
	WHIT();

	auto app = static_cast<WWatchApp*>(data);
	app->_pv->_width = width;
	app->_pv->_height = height;

	return app->onCreate();
}

void _WWatchAppImpl::_onTerminate(void* data) {
	WHIT();

	auto app = static_cast<WWatchApp*>(data);
	app->onTerminate();

	// To reverse destruction order
	// It will destroy View and then Naviframe instance before the window is destroyed.
	if (app->_pv->_window)
		app->_pv->_window->destroyBaseViewController();

	// window evas object "del" callback will be called by system.
}

void _WWatchAppImpl::_onPause(void* data) {
	WHIT();

	_WMenuPopupControllerImpl::destroyLastMenuPopup();

	auto app = static_cast<WWatchApp*>(data);
	app->onPause();
}

void _WWatchAppImpl::_onResume(void* data) {
	WHIT();

	auto app = static_cast<WWatchApp*>(data);
	app->onResume();
}

void _WWatchAppImpl::_onAppControl(app_control_h request, void* data) {
	WHIT();

	auto app = static_cast<WWatchApp*>(data);
	app->onAppControl(request, app->_pv->_firstLaunch);

	app->_pv->_firstLaunch = false;
}

void _WWatchAppImpl::_onTimeTick(watch_time_h watchTime, void* data) {
	auto app = static_cast<WWatchApp*>(data);
	app->onTimeTick(watchTime);
}

void _WWatchAppImpl::_onAmbientTick(watch_time_h watchTime, void* data) {
	auto app = static_cast<WWatchApp*>(data);
	app->onAmbientModeTimeTick(watchTime);
}

void _WWatchAppImpl::_onAmbientChanged(bool ambientMode, void* data) {
	auto app = static_cast<WWatchApp*>(data);
	app->onAmbientModeChanged(ambientMode);
}


WWatchApp::WWatchApp() {
	WDEBUG("app-assist version=%s", WVersion_get());

	_pv = new _WWatchAppImpl();
}

WWatchApp::~WWatchApp() {
	WHIT();
	delete _pv;
}

int WWatchApp::start(int argc, char* argv[]) {
	WHIT();

	watch_app_lifecycle_callback_s cb;
	memset(&cb, 0, sizeof(cb));

	cb.create = _WWatchAppImpl::_onCreate;
	cb.terminate = _WWatchAppImpl::_onTerminate;
	cb.pause = _WWatchAppImpl::_onPause;
	cb.resume = _WWatchAppImpl::_onResume;
	cb.app_control = _WWatchAppImpl::_onAppControl;
	cb.time_tick = _WWatchAppImpl::_onTimeTick;
	cb.ambient_tick = _WWatchAppImpl::_onAmbientTick;
	cb.ambient_changed = _WWatchAppImpl::_onAmbientChanged;

	int ret = watch_app_main(argc, argv, &cb, this);
	if (ret != APP_ERROR_NONE)
		WERROR("watch_app_main is failed!");

	return ret;
}

app_assist::WWatchWindowController* WWatchApp::getWindowController() {
	return _pv->_window;
}

bool WWatchApp::attachWindowController(app_assist::WWatchWindowController* win) {
	WHIT();

	WASSERT(_pv->_window == nullptr);

	_pv->_window = win;

	if (!win->create()) {
		WERROR("Window creation failed");
		return false;
	}

	evas_object_resize(win->getEvasObject(), _pv->_width, _pv->_height);
	evas_object_show(win->getEvasObject());

	return true;
}

int WWatchApp::getScreenWidth() {
	return _pv->_width;
}

int WWatchApp::getScreenHeight() {
	return _pv->_height;
}
