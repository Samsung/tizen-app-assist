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

#include "WWidgetWindowController.h"

#include <Elementary.h>
#include <widget_app_efl.h>
#include "WDebugBase.h"
#include "WDefineInternal.h"
#include "WViewController.h"

using namespace app_assist;

static const char* _CLASS_NAME = "WWidgetWindowController";

namespace app_assist {

class _WWidgetWindowControllerImpl {
public:
	_WWidgetWindowControllerImpl() : _win(nullptr), _baseLayout(nullptr), _conformant(nullptr), _baseViewController(nullptr) {};
	~_WWidgetWindowControllerImpl() {};

	static void _delReqCb(void* data, Evas_Object* obj, void* event_info);
	static void _objDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info);

	Evas_Object* _win;
	Evas_Object* _baseLayout;
	Evas_Object* _conformant;
	WViewController* _baseViewController;
};

} // namespace

void _WWidgetWindowControllerImpl::_delReqCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();
	elm_exit();
}

void _WWidgetWindowControllerImpl::_objDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info) {
	WHIT();

	auto win = static_cast<WWidgetWindowController*>(data);
	win->onDestroy();
	delete win;
}

WWidgetWindowController::WWidgetWindowController()
	: _pv(new _WWidgetWindowControllerImpl()) {
}

WWidgetWindowController::~WWidgetWindowController() {
	delete _pv;
}

bool WWidgetWindowController::create(widget_context_h context, int width, int height) {
	Evas_Object* win = nullptr;

	win = onCreateWin(context, width, height);
	if (win == nullptr) {
		WERROR("Failed to create an window!");
		return false;
	}

	evas_object_data_set(win, WKEY_WINDOW_INSTANCE, this);

	evas_object_smart_callback_add(win, "delete,request", _WWidgetWindowControllerImpl::_delReqCb, this);
	evas_object_event_callback_add(win, EVAS_CALLBACK_DEL, _WWidgetWindowControllerImpl::_objDelCb, this);

	Evas_Object* conform = nullptr;
	Evas_Object* layout = onCreateBaseLayout(win, &conform);

	_pv->_win = win;
	_pv->_baseLayout = layout;
	_pv->_conformant = conform;

	return true;
}

void WWidgetWindowController::destroy() {
	if (_pv->_win) {
		evas_object_event_callback_del(_pv->_win, EVAS_CALLBACK_DEL, _WWidgetWindowControllerImpl::_objDelCb);
		evas_object_del(_pv->_win);
		// Do not leave any code here.
		// After executing upper statement "evas_object_del", this object will be deleted at evas object deletion callback!

	}
	else {
		onDestroy();
		delete this;
	}
}

bool WWidgetWindowController::attachBaseViewController(WViewController* vc, void* creationParam) {
	WASSERT(_pv->_baseViewController == nullptr);

	bool result = false;
	if (_pv->_baseLayout) {
		result = vc->create(_pv->_baseLayout, creationParam);
	}
	else {
		result = vc->create(_pv->_win, creationParam);
		_pv->_baseLayout = vc->getEvasObject();
		elm_win_resize_object_add(_pv->_win, _pv->_baseLayout);
		evas_object_show(_pv->_baseLayout);
	}

	if (!result) {
		return false;
	}

	onAttachBaseViewController(vc);
	_pv->_baseViewController = vc;

	return true;
}

bool WWidgetWindowController::attachBaseViewController(WViewController* vc) {
	return attachBaseViewController(vc, nullptr);
}

WViewController* WWidgetWindowController::detachBaseViewController() {
	auto vc = _pv->_baseViewController;

	onDetachBaseViewController(vc);
	_pv->_baseViewController = nullptr;

	return vc;
}

const char* WWidgetWindowController::getClassName() const {
	return _CLASS_NAME;
}

Evas_Object* WWidgetWindowController::getEvasObject() const {
	return _pv->_win;
}

Evas_Object* WWidgetWindowController::getBaseLayoutEvasObject() const {
	return _pv->_baseLayout;
}

Evas_Object* WWidgetWindowController::getConformantEvasObject() const {
	return _pv->_conformant;
}

WViewController* WWidgetWindowController::getBaseViewController() const {
	return _pv->_baseViewController;
}

Evas_Object* WWidgetWindowController::onCreateWin(widget_context_h context, int width, int height) {
	Evas_Object* win = nullptr;
	int ret = widget_app_get_elm_win(context, &win);
	if (ret != WIDGET_ERROR_NONE) {
		WERROR("Fail to get window. %d", ret);
		return nullptr;
	}

	evas_object_resize(win, width, height);
	evas_object_show(win);
	return win;
}

void WWidgetWindowController::onAttachBaseViewController(WViewController* vc) {
	if (getBaseLayoutEvasObject() == nullptr) {
		WWARNING("base layout does not exist!");
		return;
	}

	elm_object_content_set(getBaseLayoutEvasObject(), vc->getEvasObject());
}
