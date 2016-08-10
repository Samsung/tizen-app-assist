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

#include "WGengridItem.h"
#include <memory.h>
#include <Elementary.h>
#include "WDebugInternal.h"
#include "WDefineInternal.h"
#include "WViewController.h"

using namespace app_assist;

namespace app_assist {

class _WGengridItemImpl {
public:
	_WGengridItemImpl() : _objectItem(nullptr) {};
	~_WGengridItemImpl() {};
	static void _selectCb(void* data, Evas_Object* obj, void* event_info);

public:
	Elm_Object_Item* _objectItem;
};
}

WGengridItem::WGengridItem() : _pv(new _WGengridItemImpl()) {
}

WGengridItem::~WGengridItem() {
	delete _pv;
}

Elm_Gengrid_Item_Class* WGengridItem::getItemClassStatic() const {
	return nullptr;
}

Elm_Gengrid_Item_Class* WGengridItem::getItemClassNew() const {
	return nullptr;
}

void WGengridItem::onSelect(Elm_Object_Item* objItem) {
}

void WGengridItem::onSetElmObjectItem(Elm_Object_Item* objItem) {
}

Elm_Object_Item* WGengridItem::getElmObjectItem() const {
	return _pv->_objectItem;
}

void WGengridItem::setElmObjectItem(Elm_Object_Item* objItem) {
	_pv->_objectItem = objItem;
	onSetElmObjectItem(objItem);
}

void _WGengridItemImpl::_selectCb(void* data, Evas_Object* obj, void* event_info) {
	Elm_Object_Item* gengridItem = (Elm_Object_Item*) event_info;
	elm_gengrid_item_selected_set(gengridItem, EINA_FALSE);

	auto item = (WGengridItem*)data;
	item->onSelect(gengridItem);
}

Evas_Smart_Cb WGengridItem::getSelectCb() {
	return _WGengridItemImpl::_selectCb;
}

bool WGengridItem::attachToParentViewController(WViewController* popup) {
	if (_pv->_objectItem == nullptr) {
		WERROR("object item is not created!");
		return false;
	}

	Evas_Object* obj = elm_object_item_widget_get(_pv->_objectItem);
	while (obj != nullptr) {
		WViewController* view = wviewcontroller_get_instance_from_evasobj(obj);
		if (view != nullptr) {
			view->attachPopup(popup);
			return true;
		}
		obj = elm_object_parent_widget_get(obj);
	}
	WERROR("Can not find view instance!");
	return false;
}

Elm_Object_Item* app_assist::wgengrid_item_append(Evas_Object* gengrid, const Elm_Gengrid_Item_Class* itc, app_assist::WGengridItem* item) {
	Elm_Object_Item* obj = elm_gengrid_item_append(gengrid, itc, item, item->getSelectCb(), item);
	item->setElmObjectItem(obj);
	return obj;
}

Elm_Object_Item* app_assist::wgengrid_item_append_with_callback(Evas_Object* gengrid, const Elm_Gengrid_Item_Class* itc, app_assist::WGengridItem* item, Evas_Smart_Cb selectCb, const void* userData) {
	Elm_Object_Item* obj = elm_gengrid_item_append(gengrid, itc, item, selectCb, userData);
	if (item)
		item->setElmObjectItem(obj);
	return obj;
}
