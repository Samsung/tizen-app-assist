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

#include "WApp.h"

#include <memory.h>

#include "WDebugInternal.h"
#include "WWindowController.h"
#include "WMenuPopupControllerImpl.h"

using namespace app_assist;

namespace app_assist {

class _WAppImpl {
public:
	_WAppImpl() : _window(nullptr), _firstLaunch(true) {}
	~_WAppImpl();

public:
	WWindowController* _window;
	bool _firstLaunch;

	static bool _onCreate(void* data);
	static void _onTerminate(void* data);
	static void _onPause(void* data);
	static void _onResume(void* data);
	static void _onAppControl(app_control_h request, void* data);
};
}

_WAppImpl::~_WAppImpl() {

}

bool _WAppImpl::_onCreate(void* data) {
	WHIT();

	auto app = (WApp*)data;
	return app->onCreate();
}

void _WAppImpl::_onTerminate(void* data) {
	WHIT();

	auto app = (WApp*)data;
	app->onTerminate();

	// To reverse destruction order
	// It will destroy View and then Naviframe instance before the window is destroyed.
	if (app->_pv->_window)
		app->_pv->_window->destroyBaseViewController();

	// window evas object "del" callback will be called by system.
}

void _WAppImpl::_onPause(void* data) {
	WHIT();

	_WMenuPopupControllerImpl::destroyLastMenuPopup();

	auto app = (WApp*)data;
	app->onPause();

}

void _WAppImpl::_onResume(void* data) {
	WHIT();

	auto app = (WApp*)data;
	app->onResume();

}

void _WAppImpl::_onAppControl(app_control_h request, void* data) {
	WHIT();

	auto app = (WApp*)data;
	app->onAppControl(request, app->_pv->_firstLaunch);

	app->_pv->_firstLaunch = false;
}

WApp::WApp() {
	WDEBUG("app-assist version=%s", WVersion_get());

	_pv = new _WAppImpl();
}

WApp::~WApp() {
	WHIT();
	delete _pv;
}

int WApp::start(int argc, char* argv[]) {
	return onStart(argc, argv);
}

app_assist::WWindowController* WApp::getWindowController() const {
	return _pv->_window;
}

bool WApp::attachWindowController(app_assist::WWindowController* win) {
	WASSERT(_pv->_window == nullptr);
	_pv->_window = win;
	return win->create();
}

void WApp::onAppControl(app_control_h request, bool firstLaunch) {
}

int WApp::onStart(int argc, char* argv[]) {
	ui_app_lifecycle_callback_s cb;
	memset(&cb, 0, sizeof(cb));
	cb.create = _WAppImpl::_onCreate;
	cb.terminate = _WAppImpl::_onTerminate;
	cb.pause = _WAppImpl::_onPause;
	cb.resume = _WAppImpl::_onResume;
	cb.app_control = _WAppImpl::_onAppControl;

	return ui_app_main(argc, argv, &cb, this);
}

