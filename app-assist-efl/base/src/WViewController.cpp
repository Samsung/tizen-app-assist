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

#include "WViewController.h"
#include <memory.h>
#include <Elementary.h>
#include "WDebugInternal.h"
#include "WDefineInternal.h"
#include "WViewControllerImpl.h"

using namespace app_assist;

#define _CLASS_NAME	"WViewController"

WViewController::WViewController() {
	_pv = new _WViewControllerImpl();
}

WViewController::~WViewController() {
	if (_pv->_name) {
		WDEBUG("name=%s", _pv->_name);
	}
	else {
		WHIT();
	}

	delete _pv;
}

const char* WViewController::getClassName() const {
	return _CLASS_NAME;
}

bool WViewController::create(Evas_Object* parent, void* param) {
	if (_pv->_name)
		WDEBUG("name=%s", _pv->_name);
	else
		WHIT();

	if (_pv->_obj) {
		WDEBUG("Already created!");
		return true;
	}
	if (!onCreate(param)) {
		WDEBUG("Failed to create!");
		return false;
	}

	if (_pv->_createViewDelegate)
		_pv->_obj = (*_pv->_createViewDelegate)(parent, param);
	else
		_pv->_obj = onCreateView(parent, param);

	if (_pv->_obj == nullptr) {
		WERROR("No view object created!");
		return false;
	}

	// get window controller
	Evas_Object* obj = _pv->_obj;
	while ((obj = elm_object_parent_widget_get(obj)) != nullptr) {
		if ((_pv->_windowController = (IWWindowController*)evas_object_data_get(obj, WKEY_WINDOW_INSTANCE)) != nullptr) {
			break;
		}
	}
	WASSERT_EX(_pv->_windowController != nullptr, "Cannot find window controll.");

	if (_pv->_name) {   // for easy debugging
		evas_object_name_set(_pv->_obj, _pv->_name);
	}

	evas_object_data_set(_pv->_obj, WKEY_VIEW_INSTANCE, this);
	evas_object_event_callback_add(_pv->_obj, EVAS_CALLBACK_DEL, _WViewControllerImpl::_objDelCb, this);

	// Do not place this before "evas_object_event_callback_add",
	// The reason is that if additional del callback is added at child class, it should be called first to keep state the class object is still alive.
	onCreated();

	return true;
}

void WViewController::destroy() {
	if (_pv->_obj) {
		evas_object_del(_pv->_obj);
		// Do not leave any code here.
		// After executing upper statement "evas_object_del", this object will be deleted at evas object deletion callback!

	}
	else {
		onDestroy();
		delete this;
	}
}

Evas_Object* WViewController::getEvasObject() const {
	return _pv->_obj;
}

void WViewController::setName(const char* name) {
	if (_pv->_name) {
		free(_pv->_name);
		_pv->_name = nullptr;
	}
	if (name) {
		_pv->_name = (char*)malloc(strlen(name) + 1);
		strcpy(_pv->_name, name);
	}
}

const char* WViewController::getName() const {
	return _pv->_name;
}

void WViewController::setOnCreateView(std::function<Evas_Object* (Evas_Object*, void*)> delegateFunc) {
	if (_pv->_createViewDelegate == nullptr)
		_pv->_createViewDelegate = new std::function<Evas_Object* (Evas_Object*, void*)>;
	*_pv->_createViewDelegate = delegateFunc;
}

std::weak_ptr<WViewController> WViewController::getWeakPtr() const {
	if (_pv->_selfPtr) {
		return std::weak_ptr<WViewController>(*_pv->_selfPtr);
	}
	_pv->_selfPtr = new std::shared_ptr<WViewController>(const_cast<WViewController*>(this), [](WViewController * p) {});
	return std::weak_ptr<WViewController>(*_pv->_selfPtr);
}

void WViewController::attachPopup(WViewController* popup) {
	Evas_Object* parent = nullptr;
	Evas_Object* obj = getEvasObject();
	while ((obj = elm_object_parent_widget_get(obj)) != nullptr) {
		if (evas_object_data_get(obj, WKEY_BASELAYOUT_TAG) != nullptr) {
			parent = obj;
			break;
		}
	}

	if (parent == nullptr) {
		WERROR("Cannot find parent View!");
		// If the pop-up has been already created, the following statement will just return without creating pop-up.
		popup->create(elm_object_top_widget_get(getEvasObject()), nullptr);
	}
	else {
		// If the pop-up has been already created, the following statement will just return without creating pop-up.
		popup->create(parent, nullptr);
	}

	// destroy old popup after new popup created
	destroyPopup();

	if (_pv->_popupMonitor == nullptr)
		_pv->_popupMonitor = new std::weak_ptr<WViewController>;
	*_pv->_popupMonitor = popup->getWeakPtr();
}

void WViewController::destroyPopup() {
	if (_pv->_popupMonitor == nullptr) return;

	if (auto p = _pv->_popupMonitor->lock()) {
		p->destroy();
		_pv->_popupMonitor->reset();
	}
}

bool WViewController::setPartContent(const char* partName, WViewController* vc) {
	if (getEvasObject() == nullptr) {
		WERROR("Container object is not yet created!");
		return false;
	}

	if (vc->create(getEvasObject(), nullptr) == false) {
		WERROR("Fail to create the view");
		return false;
	}

	elm_object_part_content_set(getEvasObject(), partName, vc->getEvasObject());
	return true;
}

WViewController* WViewController::getPartContent(const char* partName) const {
	if (getEvasObject() == nullptr) {
		WERROR("Container object is not yet created!");
		return nullptr;
	}
	Evas_Object* obj = elm_object_part_content_get(getEvasObject(), partName);
	if (obj == nullptr) {
		WWARNING("Content has not been set or the part does not exist!");
		return nullptr;
	}
	WViewController* vc = wviewcontroller_get_instance_from_evasobj(obj);

	if (vc == nullptr) {
		WERROR("This is not a view controller object");
	}
	return vc;
}

WViewController* WViewController::unsetPartContent(const char* partName) {
	if (getEvasObject() == nullptr) {
		WERROR("Container object is not yet created!");
		return nullptr;
	}
	Evas_Object* obj = elm_object_part_content_unset(getEvasObject(), partName);
	if (obj == nullptr) {
		WWARNING("Content has not been set or the part does not exist!");
		return nullptr;
	}
	WViewController* vc = wviewcontroller_get_instance_from_evasobj(obj);

	if (vc == nullptr) {
		WERROR("This is not a view controller object");
	}
	return vc;
}

IWWindowController* WViewController::getWindowController() const {
	return _pv->_windowController;
}

WViewController* app_assist::wviewcontroller_get_instance_from_evasobj(Evas_Object* obj) {
	return (WViewController*)evas_object_data_get(obj, WKEY_VIEW_INSTANCE);
}

Evas_Object* app_assist::wviewcontroller_get_window_baselayout_evasobj(WViewController* view) {
	return _WViewControllerImpl::getWindowBaseLayoutEvasObject(view);
}

Evas_Object* app_assist::wviewcontroller_get_window_evasobj(WViewController* view) {
	return _WViewControllerImpl::getWindowEvasObject(view);
}
