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

#include "WMenuPopupController.h"
#include "WMenuPopupControllerImpl.h"
#include "WDebugInternal.h"

using namespace app_assist;

WMenuPopupController::WMenuPopupController()
	: _pv(new _WMenuPopupControllerImpl()) {
	WHIT();
}

WMenuPopupController::~WMenuPopupController() {
	WHIT();
	if (_pv->_resizableBaseObj)
		evas_object_event_callback_del(_pv->_resizableBaseObj, EVAS_CALLBACK_RESIZE, _WMenuPopupControllerImpl::_popupResizeCb);

	if (_pv->_win)
		evas_object_smart_callback_del(_pv->_win, "rotation,changed",  _WMenuPopupControllerImpl::_popupWinRotateCb);

	delete _pv;
}

Elm_Object_Item* WMenuPopupController::appendItem(const char* label, std::function< void()> selectCb) {
	if (getEvasObject() == nullptr) {
		WERROR("Create pop-up first!");
		return nullptr;
	}

	_pv->_vSelectItemCb.push_back(selectCb);
	return elm_ctxpopup_item_append(getEvasObject(), label, nullptr, _WMenuPopupControllerImpl::_popupSelectCb, (void*)(_pv->_vSelectItemCb.size() - 1));
}

bool WMenuPopupController::createMenu(Evas_Object* win, Evas_Object* resizableObj) {
	_pv->_init(win, resizableObj);
	return WViewController::create(nullptr, nullptr);
}

bool WMenuPopupController::showMenu() {
	if (getEvasObject() == nullptr) {
		WERROR("Pop-up is Not created!");
		return false;
	}

	_pv->_popupMove();
	evas_object_show(getEvasObject());
	return true;
}

Evas_Object* WMenuPopupController::onCreateView(Evas_Object* parent, void* param) {
	return _pv->_create();
}

void WMenuPopupController::onDestroy() {
	WENTER();
}

