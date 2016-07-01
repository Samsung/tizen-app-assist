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

#include "WWindowController.h"

#include <Elementary.h>
#include <efl_extension.h>
#include <app.h>
#include "WViewController.h"
#include "WDebugInternal.h"
#include "WDefineInternal.h"

using namespace app_assist;

#define _CLASS_NAME	"WWindowController"

namespace app_assist {

class _WWindowControllerImpl {
public:
	_WWindowControllerImpl(const char* inName, Elm_Win_Type inWinType);
	_WWindowControllerImpl() = delete;
	~_WWindowControllerImpl();

public:
	Evas_Object* _win;
	Evas_Object* _baseLayout;
	Evas_Object* _conformant;
	WViewController* _baseViewController;
	char* _name;
	Elm_Win_Type _winType;

	std::function<Evas_Object* ()> _createWinDelegate;
	std::function<Evas_Object* (Evas_Object*, Evas_Object**)> _createBaseLayoutDelegate;
	std::function<void ()> _backHandler;

	static void _delReqCb(void* data, Evas_Object* obj, void* event_info);
	static void _objDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info);
};
}

_WWindowControllerImpl::_WWindowControllerImpl(const char* name, Elm_Win_Type winType)
	: _win(nullptr),
	_baseLayout(nullptr),
	_conformant(nullptr),
	_baseViewController(nullptr),
	_name(nullptr),
	_winType(ELM_WIN_BASIC) {
	WASSERT(name);
	_name = (char*)malloc(strlen(name) + 1);
	strcpy(_name, name);
	_winType = winType;
}

_WWindowControllerImpl::~_WWindowControllerImpl() {
	free(_name);
}


void _WWindowControllerImpl::_delReqCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();
	elm_exit();
}
void _WWindowControllerImpl::_objDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info) {
	WHIT();

	auto win = (WWindowController*)data;

	win->onDestroy();
	delete win;
}


WWindowController::WWindowController(const char* name, Elm_Win_Type winType) {
	_pv = new _WWindowControllerImpl(name, winType);
}

WWindowController::~WWindowController() {
	WHIT();
	delete _pv;
}

const char* WWindowController::getClassName() const {
	return _CLASS_NAME;
}

bool WWindowController::create() {
	if (_pv->_win) {
		WDEBUG("Already created!");
		return true;
	}

	if (precreate()) {
		evas_object_show(_pv->_win);
		return true;
	}
	else {
		return false;
	}
}

bool WWindowController::precreate() {
	if (_pv->_win) {
		WDEBUG("Already created!");
		return true;
	}

	Evas_Object* win = nullptr;
	if (_pv->_createWinDelegate)
		win = _pv->_createWinDelegate();
	else
		win = onCreateWin();
	_pv->_win = win;
	if (win) {
		Evas_Object* cf = nullptr;
		if (_pv->_createBaseLayoutDelegate)
			_pv->_baseLayout = _pv->_createBaseLayoutDelegate(win, &cf);
		else
			_pv->_baseLayout = onCreateBaseLayout(win, &cf);
		_pv->_conformant = cf;
		evas_object_data_set(win, WKEY_WINDOW_INSTANCE, this);
		if (_pv->_baseLayout)
			evas_object_data_set(_pv->_baseLayout, WKEY_BASELAYOUT_TAG, this);
		// This callback function is called when 'x' close button on the window at desktop mode. In mobile, it will not be called.
		evas_object_smart_callback_add(win, "delete,request", _WWindowControllerImpl::_delReqCb, this);
		evas_object_event_callback_add(win, EVAS_CALLBACK_DEL, _WWindowControllerImpl::_objDelCb, this);

		if (_pv->_backHandler) {
			eext_object_event_callback_add(win, EEXT_CALLBACK_BACK,
			[](void* data, Evas_Object * obj, void* event_info)->void {
				auto wc = (WWindowController*)data;
				wc->_pv->_backHandler();
			}, this);
		}

		return true;
	}
	else {
		return false;
	}
}

void WWindowController::destroy() {
	if (_pv->_win) {
		evas_object_del(_pv->_win);
		// Do not leave any code here.
		// After executing upper statement "evas_object_del", this object will be deleted at evas object deletion callback!

	}
	else {
		onDestroy();
		delete this;
	}
}

Evas_Object* WWindowController::getEvasObject() const {
	return _pv->_win;
}

Evas_Object* WWindowController::getBaseLayoutEvasObject() const {
	return _pv->_baseLayout;
}

Evas_Object* WWindowController::getConformantEvasObject() const {
	return _pv->_conformant;
}

bool WWindowController::attachBaseViewController(WViewController* vc, void* creationParam) {
	WASSERT(_pv->_baseViewController == nullptr);

	// If baseLayout is nullptr, the vc should be already created before attaching.
	if (_pv->_baseLayout) {
		if (vc->create(_pv->_baseLayout, creationParam) == false) {
			return false;
		}
	}

	onAttachBaseViewController(vc);

	_pv->_baseViewController = vc;

//	evas_object_data_set( vc->getEvasObject(), WKEY_WINDOW_INSTANCE, this );

	return true;
}

bool WWindowController::attachBaseViewController(WViewController* vc) {
	return attachBaseViewController(vc, nullptr);
}

WViewController* WWindowController::getBaseViewController() const {
	return _pv->_baseViewController;
}

WViewController* WWindowController::detachBaseViewController() {
	WViewController* vc = _pv->_baseViewController;

	onDetachBaseViewController(vc);

	_pv->_baseViewController = nullptr;

	return vc;
}

void WWindowController::destroyBaseViewController() {
	if (_pv->_baseViewController)
		_pv->_baseViewController->destroy();

	_pv->_baseViewController = nullptr;
}

const char* WWindowController::getName() const {
	return _pv->_name;
}

Elm_Win_Type WWindowController::getWinType() const {
	return _pv->_winType;
}

void WWindowController::setOnCreateWin(std::function<Evas_Object* ()> delegateFunc) {
	_pv->_createWinDelegate = delegateFunc;
}

void WWindowController::setOnCreateBaseLayout(std::function<Evas_Object* (Evas_Object*, Evas_Object**)> delegateFunc) {
	_pv->_createBaseLayoutDelegate = delegateFunc;
}

void WWindowController::setOnBack(std::function<void()> backHandlerFunc) {
	_pv->_backHandler = backHandlerFunc;
}

Evas_Object* WWindowController::onCreateWin() {
	Evas_Object* win = elm_win_add(nullptr, getName(), getWinType());

	if (win == nullptr) {
		WERROR("Fail to create win!");
		return nullptr;
	}

	elm_win_title_set(win, getName());
	elm_win_conformant_set(win, EINA_TRUE);
	elm_win_autodel_set(win, EINA_TRUE);
	if (elm_win_wm_rotation_supported_get(win)) {
		int rotation[4] = {0, 90, 180, 270};
		elm_win_wm_rotation_available_rotations_set(win, (const int*)(&rotation), 4);

	}
	elm_win_indicator_mode_set(win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(win, ELM_WIN_INDICATOR_OPAQUE);


	return win;
}

Evas_Object* WWindowController::onCreateBaseLayout(Evas_Object* win, Evas_Object** outConformant) {
	Evas_Object* bg = elm_bg_add(win);

	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	Evas_Object* cf = elm_conformant_add(win);

	evas_object_size_hint_weight_set(cf, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, cf);
	evas_object_show(cf);

	Evas_Object* layout = elm_layout_add(cf);
	elm_layout_theme_set(layout, "layout", "application", "default");
	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_content_set(cf, layout);

	evas_object_show(layout);

	*outConformant = cf;

	return layout;
}

void WWindowController::onAttachBaseViewController(WViewController* vc) {
	if (getBaseLayoutEvasObject() == nullptr) {
		WWARNING("base layout does not exist!");
		return;
	}
	elm_object_part_content_set(getBaseLayoutEvasObject(), "elm.swallow.content", vc->getEvasObject());
}

void WWindowController::onDetachBaseViewController(WViewController* vc) {

}

void WWindowController::onDestroy() {

}
