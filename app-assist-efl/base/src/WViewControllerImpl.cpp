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

#include "WViewControllerImpl.h"
#include <memory.h>
#include "WViewController.h"
#include "IWWindowController.h"
#include "WDebugInternal.h"

using namespace app_assist;

_WViewControllerImpl::_WViewControllerImpl()
	: _obj(nullptr),
	_name(nullptr),
	_popupMonitor(nullptr),
	_createViewDelegate(nullptr),
	_windowController(nullptr),
	_selfPtr(nullptr) {
}

_WViewControllerImpl::~_WViewControllerImpl() {
	free(_name);

	if (_popupMonitor) {
		if (auto p = _popupMonitor->lock()) {
			p->destroy();
		}
	}
	delete _popupMonitor;
	delete _createViewDelegate;
	delete _selfPtr;
}

void _WViewControllerImpl::_objDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info) {
	auto ctrl = (WViewController*)data;
	ctrl->onDestroy();

	delete ctrl;
}

Evas_Object* _WViewControllerImpl::getWindowBaseLayoutEvasObject(WViewController* view) {
	if (view->getWindowController()) {
		return view->getWindowController()->getBaseLayoutEvasObject();
	}
	else {
		WERROR("Fail to get base layout!");
		return nullptr;
	}
}

Evas_Object* _WViewControllerImpl::getWindowEvasObject(WViewController* view) {
	if (view->getWindowController()) {
		return view->getWindowController()->getEvasObject();
	}
	else {
		WERROR("Fail to get window evas object!");
		return nullptr;
	}
}

