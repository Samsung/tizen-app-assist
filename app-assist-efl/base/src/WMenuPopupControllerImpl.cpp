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

#include "WMenuPopupControllerImpl.h"
#include <vector>
#include <efl_extension.h>
#include "WMenuPopupController.h"
#include "WDebugInternal.h"

using namespace app_assist;

Evas_Object* _WMenuPopupControllerImpl::_lastMenuPopup = nullptr;

_WMenuPopupControllerImpl::_WMenuPopupControllerImpl()
	: _win(nullptr), _resizableBaseObj(nullptr), _popup(nullptr) {
}

_WMenuPopupControllerImpl::~_WMenuPopupControllerImpl() {
	if (_popup == _lastMenuPopup)
		_lastMenuPopup = nullptr;
}

void _WMenuPopupControllerImpl::destroyLastMenuPopup() {
	if (_lastMenuPopup)
		evas_object_del(_lastMenuPopup);
	_lastMenuPopup = nullptr;
}

void _WMenuPopupControllerImpl::_init(Evas_Object* win, Evas_Object* resizableObj) {
	_win = win;
	_resizableBaseObj = resizableObj;
}

void _WMenuPopupControllerImpl:: _popupMove() {
	WENTER();
	int w1, h1;
	int rotate;

	elm_win_screen_size_get(_win, nullptr, nullptr, &w1, &h1);
	rotate = elm_win_rotation_get(_win);

	if (rotate == 90)
		evas_object_move(_popup, h1 / 2, w1);
	else if (rotate == 270)
		evas_object_move(_popup, h1 / 2, w1);
	else
		evas_object_move(_popup, w1 / 2, h1);

	evas_object_show(_popup);
}

Evas_Object* _WMenuPopupControllerImpl::_create() {
	if (_win == nullptr) {
		WERROR("Call createMenu first!");
		return nullptr;
	}
	Evas_Object* popup = nullptr;
	popup = elm_ctxpopup_add(_win);
	elm_object_style_set(popup, "more/default");
	elm_ctxpopup_auto_hide_disabled_set(popup, EINA_TRUE);

	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_ctxpopup_back_cb, nullptr);
	eext_object_event_callback_add(popup, EEXT_CALLBACK_MORE, eext_ctxpopup_back_cb, nullptr);
	evas_object_event_callback_add(_resizableBaseObj, EVAS_CALLBACK_RESIZE, _WMenuPopupControllerImpl::_popupResizeCb, this);
	evas_object_smart_callback_add(_win, "rotation,changed",  _WMenuPopupControllerImpl::_popupWinRotateCb, this);
	elm_ctxpopup_direction_priority_set(popup, ELM_CTXPOPUP_DIRECTION_DOWN, ELM_CTXPOPUP_DIRECTION_UP, ELM_CTXPOPUP_DIRECTION_UNKNOWN, ELM_CTXPOPUP_DIRECTION_UNKNOWN);
	evas_object_smart_callback_add(popup, "dismissed", _WMenuPopupControllerImpl::_popupDismissedCb, this);
	evas_object_smart_callback_add(popup, "language,changed", _WMenuPopupControllerImpl::_popupDismissedCb, this);

	_popup = popup;
	_lastMenuPopup = popup;

	return popup;

}

void _WMenuPopupControllerImpl::_popupSelectCb(void* data, Evas_Object* obj, void* event_info) {
	WENTER();
	auto p = (WMenuPopupController*)wviewcontroller_get_instance_from_evasobj(obj);
	const unsigned int itemIndex = (unsigned int)data;
	p->_pv->_vSelectItemCb[itemIndex]();
	evas_object_del(obj);
}

void _WMenuPopupControllerImpl::_popupResizeCb(void* data, Evas* e, Evas_Object* obj, void* event_info) {
	WENTER();
	auto pImpl = (_WMenuPopupControllerImpl*)data;
	pImpl->_popupMove();
}

void _WMenuPopupControllerImpl::_popupWinRotateCb(void* data, Evas_Object* obj, void* event_info) {
	WENTER();
	auto pImpl = (_WMenuPopupControllerImpl*)data;
	pImpl->_popupMove();
}

void _WMenuPopupControllerImpl::_popupDismissedCb(void* data, Evas_Object* obj, void* event_info) {
	WENTER();
	auto pImpl = (_WMenuPopupControllerImpl*)data;
	evas_object_del(pImpl->_popup);
}

