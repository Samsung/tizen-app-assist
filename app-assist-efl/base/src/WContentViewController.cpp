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

#include "WContentViewController.h"
#include <memory.h>
#include <string>
#include <app.h>
#include <Elementary.h>
#include <efl_extension.h>
#include "WDebugInternal.h"
#include "WDefineInternal.h"
#include "WContentViewControllerImpl.h"

using namespace app_assist;

#define _CLASS_NAME	"WContentViewController"

WContentViewController::WContentViewController() {
	WHIT();
	_pv = new _WContentViewControllerImpl();
}

WContentViewController::WContentViewController(const char* name) : _pv(new _WContentViewControllerImpl()) {
	WHIT();
	setName(name);
}

WContentViewController::~WContentViewController() {
	WHIT();
	delete _pv;
}

const char* WContentViewController::getClassName() const {
	return _CLASS_NAME;
}

bool WContentViewController::create(Evas_Object* parent, void* param) {
	return WViewController::create(parent, param);
}

void WContentViewController::destroy() {
	if (getEvasObject()) {
		if (_pv->_naviItem && _pv->_isViewNaviItem) {
			elm_object_item_del(_pv->_naviItem);
		}
		else {
			evas_object_del(getEvasObject());
		}
		// Do not leave any code here.
		// After executing upper statement "evas_object_del", this object will be deleted at evas object deletion callback!
	}
	else {
		onDestroy();
		delete this;
	}
}

void WContentViewController::setTitle(const char* title) {
	if (_pv->_title) {
		free(_pv->_title);
		_pv->_title = nullptr;
	}
	if (title) {
		_pv->_title = (char*)malloc(strlen(title) + 1);
		strcpy(_pv->_title, title);
	}
}

const char* WContentViewController::getTitle() const {
	return _pv->_title;
}

void WContentViewController::setOnPushed(std::function<void (Elm_Object_Item*)> delegateFunc) {
	_pv->_pushedDelegate = delegateFunc;
}

bool WContentViewController::popOut() {
	if (getEvasObject() == nullptr) {
		WWARNING("Not created view! No Evas Object!");
		return false;
	}
	if (getNaviframeController() == nullptr) {
		WWARNING("Not pushed to naviframe!");
		return false;
	}

	if (_pv->_naviItem != elm_naviframe_top_item_get(getNaviframeController()->getEvasObject())) {
		WASSERT_EX(0, "This view is not on Top of Naviframe!");
		return false;
	}

	destroyPopup(); // Before popping out view, pop-up is destroyed, if it has.Because pop-up is disappeared too late.

	elm_naviframe_item_pop(getNaviframeController()->getEvasObject());
	return true;
}

bool WContentViewController::onPop() {
	return true;
}

void WContentViewController::onBecomeTop() {
}

WNaviframeController* WContentViewController::getNaviframeController() const {
	if (_pv->_naviframe == nullptr) {
		WWARNING("naviframe does not exist!");
	}

	return _pv->_naviframe;
}

Elm_Object_Item* WContentViewController::getNaviItem() const {
	if (_pv->_naviItem == nullptr) {
		WWARNING("naviItem does not exist!");
	}

	return _pv->_naviItem;
}

void WContentViewController::onPushed(Elm_Object_Item* naviItem) {

}

Elm_Object_Item* WContentViewController::onPushToNaviframe(Evas_Object* naviFrame) {
	return nullptr;
}


Evas_Object* WContentViewController::onCreateView(Evas_Object* parent, void* param) {
	Evas_Object* layout = elm_layout_add(parent);
	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	char* path = app_get_resource_path();
	if (path == nullptr) {
		WERROR("Failed to get resource path=nullptr");
		return layout;
	}
	std::string edjPath(path);
	free(path);
	edjPath += "edje/app-assist-efl.edj";

	if (elm_layout_file_set(layout, edjPath.c_str(), "default-view") == EINA_TRUE) {
		Evas_Object* rect = evas_object_rectangle_add(evas_object_evas_get(layout));
		evas_object_color_set(rect, 256, 256, 256, 255);
		evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(rect, EVAS_HINT_FILL, EVAS_HINT_FILL);

		elm_object_part_content_set(layout, "rect", rect);
		if (getName() != nullptr)
			elm_object_part_text_set(layout, "name", getName());

	}
	else {
		WERROR("Fail to set layout. Check EDJ file(%s)", edjPath.c_str());
	}

	evas_object_show(layout);

	return layout;
}

void WContentViewController::onSetContainerProperties(WNaviframeController* naviframe, Elm_Object_Item* item) {

}

void WContentViewController::onSetAsPageContent(Elm_Object_Item* parentNaviItem) {

}

void WContentViewController::onSetMoreButton(Elm_Object_Item* naviItem, Evas_Smart_Cb clickedCb, void* userData) {
//	eext_object_event_callback_add(getEvasObject(), EEXT_CALLBACK_MORE, clickedCb, userData); --> It has a bug: back key is not working!
	Evas_Object* btn = elm_button_add(getNaviframeController()->getEvasObject());
	elm_object_style_set(btn, "naviframe/more/default");
	evas_object_show(btn);
	evas_object_smart_callback_add(btn, "clicked", clickedCb, userData);
	elm_object_item_part_content_set(naviItem, "toolbar_more_btn", btn);
}


bool WContentViewController::setMoreButton(Elm_Object_Item* naviItem, Evas_Smart_Cb clickedCb, void* userData) {
	onSetMoreButton(naviItem, clickedCb, userData);
	return true;
}

void app_assist::wcontentviewcontroller_set_container_properties(WContentViewController* view, WNaviframeController* naviframe) {
	_WContentViewControllerImpl::setContainerProperties(view, naviframe);
}

void app_assist::wcontentviewcontroller_set_container_naviitem_property(WContentViewController* view, Elm_Object_Item* item, bool isViewNaviItem) {
	_WContentViewControllerImpl::setContainerNaviitemProperty(view, item, isViewNaviItem);
}

void app_assist::wcontentviewcontroller_propagate_container_properties_to_childviews(WContentViewController* view) {
	_WContentViewControllerImpl::propagateContainerPropertiesToChildViews(view);
}

