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

#include "WPopupController.h"
#include <string>
#include <Evas.h>
#include <efl_extension.h>
#include "WDebugInternal.h"


using namespace app_assist;

#define _MAX_BUTTON_NUM	3

namespace app_assist {

class _WPopupControllerImpl {
public:
	_WPopupControllerImpl();
	~_WPopupControllerImpl();

public:
	std::string _title;
	std::string _text;
	std::string _textDomain;
	WViewController* _content;
	std::string _listButtonLabel[_MAX_BUTTON_NUM];
	int _count;
	bool _isTranslatable;

	std::function<Evas_Object* (Evas_Object* parent)> _contentCreator;
	std::function<void(Evas_Object* popup)> _addPropertiesCb;
	std::function<void (bool* destroyPopup)> _backCb;

	std::function<void (bool* destroyPopup)> _vButtonCb[_MAX_BUTTON_NUM];
	std::function<void (Evas_Object* button)>* buttonPropertyCb[_MAX_BUTTON_NUM];

	void _setTextPart(Evas_Object* obj, const char* part, const char* text);
};
}

_WPopupControllerImpl::_WPopupControllerImpl()
	: _content(nullptr), _count(0), _isTranslatable(false) {
	for (int i = 0; i < _MAX_BUTTON_NUM; i++) {
		buttonPropertyCb[i] = nullptr;
	}
}

_WPopupControllerImpl::~_WPopupControllerImpl() {
	for (int i = 0; i < _MAX_BUTTON_NUM; i++) {
		delete buttonPropertyCb[i];
	}
}

void _WPopupControllerImpl::_setTextPart(Evas_Object* obj, const char* part, const char* text) {
	if (!_textDomain.empty()) {
		elm_object_domain_translatable_part_text_set(obj, part, _textDomain.c_str(), text);
	}
	else {
		if (_isTranslatable)
			elm_object_translatable_part_text_set(obj, part, text);
		else
			elm_object_part_text_set(obj, part, text);
	}
}

WPopupController::WPopupController()
	: _pv(new _WPopupControllerImpl()) {
	WHIT();
}

WPopupController::WPopupController(const char* title, const char* textContent)
	: _pv(new _WPopupControllerImpl()) {
	if (title)
		_pv->_title = title;
	if (textContent)
		_pv->_text = textContent;
}

WPopupController::~WPopupController() {
	WHIT();
	delete _pv;
}

void WPopupController::setTitle(const char* title) {
	if (title)
		_pv->_title = title;
	else
		_pv->_title.clear();
}

void WPopupController::setContent(const char* text) {
	if (text)
		_pv->_text = text;
	else
		_pv->_text.clear();
	if (_pv->_content) {
		_pv->_content->destroy();
		_pv->_content = nullptr;
	}
}

void WPopupController::setContent(WViewController* control) {
	_pv->_text = "";
	_pv->_content = control;
}

void WPopupController::setContent(std::function<Evas_Object* (Evas_Object* parent)> contentCreateCb) {
	_pv->_text = "";
	if (_pv->_content) {
		_pv->_content->destroy();
		_pv->_content = nullptr;
	}
	_pv->_contentCreator = contentCreateCb;
}

void WPopupController::setTextTranslatable() {
	_pv->_isTranslatable = true;
}

void WPopupController::setTextTranslatable(const char* textDomain) {
	if (textDomain)
		_pv->_textDomain = textDomain;
	else
		_pv->_textDomain.clear();
}

// Popup can be destroyed in button callback, so check its validity by calling expired of weak pointer.
#define BUTTON_CALLBACK(_buttonIndex) \
[](void* data, Evas_Object* obj, void* eventInfo) {\
	WPopupController* self = (WPopupController*)data;\
	if (self->_pv->_vButtonCb[_buttonIndex]) { \
		bool destroyPopup = true; \
		auto selfWeakPtr = self->getWeakPtr(); \
		self->_pv->_vButtonCb[_buttonIndex](&destroyPopup); \
		if (!selfWeakPtr.expired() && destroyPopup) \
			evas_object_del(self->getEvasObject());\
	}\
	else \
		evas_object_del(self->getEvasObject());\
}

bool WPopupController::addButton(const char* buttonText, std::function<void (bool* destroyPopup)> buttonCb, std::function<void (Evas_Object* button)> setPropertyCb) {
	if (_pv->_count >= _MAX_BUTTON_NUM) {
		WERROR("Over the max button number! =%d", _pv->_count);
		return false;
	}
	_pv->_listButtonLabel[_pv->_count] = buttonText;
	_pv->_vButtonCb[_pv->_count] = buttonCb;
	if (setPropertyCb)
		_pv->buttonPropertyCb[_pv->_count] = new std::function<void (Evas_Object* button)>(setPropertyCb);

	_pv->_count++;
	return true;
}

bool WPopupController::addButton(const char* buttonText, std::function<void (bool* destroyPopup)> buttonCb) {
	return addButton(buttonText, buttonCb, nullptr);
}

void WPopupController::setOnBack(std::function<void (bool* destroyPopup)> backCb) {
	_pv->_backCb = backCb;
}

void WPopupController::setOnAddProperties(std::function<void(Evas_Object* popup)> addPropertiesCb) {
	_pv->_addPropertiesCb = addPropertiesCb;
}

Evas_Object* WPopupController::onCreateView(Evas_Object* parent, void* param) {
	WHIT();

	Evas_Object* popup = elm_popup_add(parent);

	// It make the width to fit to screen width in portrait mode. 2014.09.30 modified as EFL notice.
	elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);

	if (_pv->_backCb)
		eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, [](void* data, Evas_Object * obj, void* event_info) {
		WPopupController* self = (WPopupController*)data;
		bool destroyPopup = true;
		auto selfWeakPtr = self->getWeakPtr();
		self->_pv->_backCb(&destroyPopup);
		if (!selfWeakPtr.expired() && destroyPopup) {
			self->destroy();
		}
	}, this);
	else
		eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_popup_back_cb, nullptr);

	if (_pv->_addPropertiesCb)
		_pv->_addPropertiesCb(popup);

	if (!_pv->_title.empty()) {
		_pv->_setTextPart(popup, "title,text", _pv->_title.c_str());
	}

	if (!_pv->_text.empty()) {
		_pv->_setTextPart(popup, nullptr, _pv->_text.c_str());
	}
	else if (_pv->_content) {
		_pv->_content->create(popup, nullptr);
		elm_object_content_set(popup, _pv->_content->getEvasObject());
	}
	else if (_pv->_contentCreator) {
		Evas_Object* obj = _pv->_contentCreator(popup);
		elm_object_content_set(popup, obj);
	}

	for (int i = 0; i < _pv->_count; i++) {
		Evas_Object* button = elm_button_add(popup);
		elm_object_style_set(button, "popup");

		_pv->_setTextPart(button, nullptr, _pv->_listButtonLabel[i].c_str());

		char partname[100] = {0};
		snprintf(partname, sizeof(partname) - 1, "button%d", i + 1);
		elm_object_part_content_set(popup, partname, button);

		switch (i) {
		case 0:
			evas_object_smart_callback_add(button, "clicked", BUTTON_CALLBACK(0), this);
			break;
		case 1:
			evas_object_smart_callback_add(button, "clicked", BUTTON_CALLBACK(1), this);
			break;
		case 2:
			evas_object_smart_callback_add(button, "clicked", BUTTON_CALLBACK(2), this);
			break;
		default:
			break;
		}

		if (_pv->buttonPropertyCb[i]) {
			(*_pv->buttonPropertyCb[i])(button);
		}
		evas_object_show(button);
	}

	evas_object_show(popup);

	return popup;

}

void WPopupController::onDestroy() {
}



