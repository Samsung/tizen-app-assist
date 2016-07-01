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

#include "WWatchWindowController.h"
#include <Elementary.h>

#include <app_types.h>
#include <watch_app_efl.h>

#include "WViewController.h"
#include "WDebugInternal.h"
#include "WDefineInternal.h"

static const char* _CLASS_NAME = "WWatchWindowController";

using namespace app_assist;

namespace app_assist {

class _WWatchWindowControllerImpl {
public:
	_WWatchWindowControllerImpl();
	~_WWatchWindowControllerImpl();

public:
	static void _objDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info);

	Evas_Object* _win;
	Evas_Object* _baseLayout;
	Evas_Object* _conformant;
	WViewController* _baseViewController;

	std::function<Evas_Object* ()> _createWinDelegate;
	std::function<Evas_Object* (Evas_Object*, Evas_Object**)> _createBaseLayoutDelegate;
};
}

_WWatchWindowControllerImpl::_WWatchWindowControllerImpl()
	: _win(nullptr), _baseLayout(nullptr), _conformant(nullptr), _baseViewController(nullptr) {
}

_WWatchWindowControllerImpl::~_WWatchWindowControllerImpl() {
}

void _WWatchWindowControllerImpl::_objDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info) {
	WHIT();

	auto win = (WWatchWindowController*)data;

	win->onDestroy();
	delete win;
}


WWatchWindowController::WWatchWindowController() {
	_pv = new _WWatchWindowControllerImpl();
}

WWatchWindowController::~WWatchWindowController() {
	WHIT();
	delete _pv;
}

bool WWatchWindowController::create() {
	WHIT();

	if (_pv->_win) {
		WDEBUG("Already created!");
		return true;
	}

	Evas_Object* win = nullptr;
	if (_pv->_createWinDelegate)
		win = _pv->_createWinDelegate();
	else
		win = onCreateWin();

	if (win == nullptr) {
		WERROR("Failed to create an window!");
		return false;
	}

	evas_object_data_set(win, WKEY_WINDOW_INSTANCE, this);
	evas_object_event_callback_add(win, EVAS_CALLBACK_DEL, _WWatchWindowControllerImpl::_objDelCb, this);

	_pv->_win = win;

	Evas_Object* cf = nullptr;
	if (_pv->_createBaseLayoutDelegate)
		_pv->_baseLayout = _pv->_createBaseLayoutDelegate(win, &cf);
	else
		_pv->_baseLayout = onCreateBaseLayout(win, &cf);

	if (_pv->_baseLayout)
		evas_object_data_set(_pv->_baseLayout, WKEY_BASELAYOUT_TAG, this);

	evas_object_show(_pv->_win);

	return true;
}

void WWatchWindowController::destroy() {
	WHIT();

	if (_pv->_win) {
		evas_object_event_callback_del(_pv->_win, EVAS_CALLBACK_DEL, _WWatchWindowControllerImpl::_objDelCb);
	}
	onDestroy();
	delete this;
}

bool WWatchWindowController::attachBaseViewController(WViewController* vc, void* creationParam) {
	WHIT();

	WASSERT(_pv->_baseViewController == nullptr);

	// If baseLayout is nullptr, the vc should be already created before attaching.
	if (_pv->_baseLayout) {
		if (vc->create(_pv->_baseLayout, creationParam) == false) {
			return false;
		}
	}

	onAttachBaseViewController(vc);

	_pv->_baseViewController = vc;

	return true;
}

bool WWatchWindowController::attachBaseViewController(WViewController* vc) {
	return attachBaseViewController(vc, nullptr);
}

WViewController* WWatchWindowController::detachBaseViewController() {
	WHIT();

	WViewController* vc = _pv->_baseViewController;

	onDetachBaseViewController(vc);

	_pv->_baseViewController = nullptr;

	return vc;
}

void WWatchWindowController::destroyBaseViewController() {
	WHIT();

	if (_pv->_baseViewController)
		_pv->_baseViewController->destroy();

	_pv->_baseViewController = nullptr;
}

void WWatchWindowController::setOnCreateWin(const std::function<Evas_Object* ()>& delegateFunc) {
	_pv->_createWinDelegate = delegateFunc;
}

void WWatchWindowController::setOnCreateBaseLayout(std::function<Evas_Object* (Evas_Object*, Evas_Object**)> delegateFunc) {
	_pv->_createBaseLayoutDelegate = delegateFunc;
}

const char* WWatchWindowController::getClassName() const {
	return _CLASS_NAME;
}

Evas_Object* WWatchWindowController::getEvasObject() const {
	return _pv->_win;
}

Evas_Object* WWatchWindowController::getBaseLayoutEvasObject() const {
	return _pv->_baseLayout;
}

Evas_Object* WWatchWindowController::getConformantEvasObject() const {
	return _pv->_conformant;
}

WViewController* WWatchWindowController::getBaseViewController() const {
	return _pv->_baseViewController;
}

Evas_Object* WWatchWindowController::onCreateWin() {
	WHIT();

	Evas_Object* win = nullptr;
	int ret = watch_app_get_elm_win(&win);

	if (ret != APP_ERROR_NONE) {
		WERROR("Fail to get win!");
		return nullptr;
	}
	return win;
}

Evas_Object* WWatchWindowController::onCreateBaseLayout(Evas_Object* win, Evas_Object** outConformant) {
	WHIT();

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

void WWatchWindowController::onAttachBaseViewController(WViewController* vc) {
	elm_win_resize_object_add(getEvasObject(), vc->getEvasObject());
}

void WWatchWindowController::onDetachBaseViewController(WViewController* vc) {
	elm_win_resize_object_del(getEvasObject(), vc->getEvasObject());
}

void WWatchWindowController::onDestroy() {

}
