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

#include "WServiceApp.h"
#include <memory.h>
#include "WDebugInternal.h"

using namespace app_assist;

namespace app_assist {

class _WServiceAppImpl {
public:
	_WServiceAppImpl() : _firstLaunch(true) {}
	~_WServiceAppImpl();
public:
	bool _firstLaunch;

	static bool _onCreate(void* data);
	static void _onTerminate(void* data);
	static void _onAppControl(app_control_h request, void* data);
};
}

_WServiceAppImpl::~_WServiceAppImpl() {

}

bool _WServiceAppImpl::_onCreate(void* data) {
	WHIT();

	auto app = (WServiceApp*)data;
	return app->onCreate();
}

void _WServiceAppImpl::_onTerminate(void* data) {
	WHIT();

	auto app = (WServiceApp*)data;
	app->onTerminate();
}

void _WServiceAppImpl::_onAppControl(app_control_h request, void* data) {
	WHIT();

	auto app = (WServiceApp*)data;
	app->onAppControl(request, app->_pv->_firstLaunch);

	app->_pv->_firstLaunch = false;
}

WServiceApp::WServiceApp() {
	_pv = new _WServiceAppImpl();
}

WServiceApp::~WServiceApp() {
	WHIT();
	delete _pv;
}

int WServiceApp::start(int argc, char* argv[]) {
	return onStart(argc, argv);
}

void WServiceApp::onAppControl(app_control_h request, bool firstLaunch) {
}

int WServiceApp::onStart(int argc, char* argv[]) {
	service_app_lifecycle_callback_s cb;
	memset(&cb, 0, sizeof(cb));
	cb.create = _WServiceAppImpl::_onCreate;
	cb.terminate = _WServiceAppImpl::_onTerminate;
	cb.app_control = _WServiceAppImpl::_onAppControl;

	return service_app_main(argc, argv, &cb, this);

}

