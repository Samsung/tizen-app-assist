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

#include "WGenlistItem.h"
#include <memory.h>
#include <Elementary.h>
#include "WDebugInternal.h"
#include "WDefineInternal.h"
#include "WViewController.h"

using namespace app_assist;

namespace app_assist {

class _WGenlistItemImpl {
public:
	_WGenlistItemImpl() : _objectItem(nullptr) {};
	~_WGenlistItemImpl() {};
	static void _selectCb(void* data, Evas_Object* obj, void* event_info);

public:
	Elm_Object_Item* _objectItem;
};
}

WGenlistItem::WGenlistItem() : _pv(new _WGenlistItemImpl()) {
}

WGenlistItem::~WGenlistItem() {
	delete _pv;
}

Elm_Genlist_Item_Class* WGenlistItem::getItemClassStatic() const {
	return nullptr;
}

Elm_Genlist_Item_Class* WGenlistItem::getItemClassNew() const {
	return nullptr;
}

void WGenlistItem::onSelect(Elm_Object_Item* objItem) {
}

void WGenlistItem::onSetElmObjectItem(Elm_Object_Item* objItem) {
}

Elm_Object_Item* WGenlistItem::getElmObjectItem() const {
	return _pv->_objectItem;
}

void WGenlistItem::setElmObjectItem(Elm_Object_Item* objItem) {
	_pv->_objectItem = objItem;
	onSetElmObjectItem(objItem);
}


void _WGenlistItemImpl::_selectCb(void* data, Evas_Object* obj, void* event_info) {
	Elm_Object_Item* genlistItem = (Elm_Object_Item*) event_info;
	elm_genlist_item_selected_set(genlistItem, EINA_FALSE);

	auto item = (WGenlistItem*)data;
	item->onSelect(genlistItem);
}

Evas_Smart_Cb WGenlistItem::getSelectCb() {
	return _WGenlistItemImpl::_selectCb;
}

bool WGenlistItem::attachToParentViewController(WViewController* popup) {
	if (_pv->_objectItem == nullptr) {
		WERROR("object item has not been set! call setElmObjectItem");
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


Elm_Object_Item* app_assist::wgenlist_item_append(Evas_Object* genlist, const Elm_Genlist_Item_Class* itc, WGenlistItem* item, Elm_Genlist_Item_Type type) {
	Elm_Object_Item* obj = elm_genlist_item_append(genlist, itc, item, nullptr, type, item->getSelectCb(), item);
	item->setElmObjectItem(obj);
	return obj;
}

Elm_Object_Item* app_assist::wgenlist_item_append_with_callback(Evas_Object* genlist, const Elm_Genlist_Item_Class* itc, WGenlistItem* item, Elm_Genlist_Item_Type type, Evas_Smart_Cb selectCb, const void* userData) {
	Elm_Object_Item* obj = elm_genlist_item_append(genlist, itc, item, nullptr, type, selectCb, userData);
	if (item)
		item->setElmObjectItem(obj);
	return obj;
}

