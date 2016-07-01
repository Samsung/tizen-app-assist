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

#include "SmplCustomSettingsPopupController.h"
#include <Elementary.h>
#include "WGenlistItem.h"
#include "SmplDebug.h"

using namespace app_assist;

class _RadioListItem : public WGenlistItem {
public:
	_RadioListItem(int selectedIndex, int index, std::list<const char*>* itemList) {
		_selectedIndex = selectedIndex;
		_index = index;
		_itemList = itemList;
	}
public:
	virtual Elm_Genlist_Item_Class* getItemClassNew() {
		Elm_Genlist_Item_Class* itc = elm_genlist_item_class_new();
		itc->item_style = "1line";
		itc->func.text_get = _glTextGetCb;
		itc->func.content_get = _glContentGetCb;
		itc->func.del = WGENLIST_ITEM_DEL();

		return itc;
	}
private:
	static char* _glTextGetCb(void* data, Evas_Object* obj, const char* part) {
		_RadioListItem* item = (_RadioListItem*)data;

		if (!strcmp(part, "elm.text.main.left")) {
			int index = 0;
			for (auto it = item->_itemList->begin(); it != item->_itemList->end(); it++) {
				if (index == item->_index) {
					return strdup(*it);
				}
				index++;
			}

		}
		return nullptr;
	}
	static Evas_Object* _glContentGetCb(void* data, Evas_Object* obj, const char* part) {
		_RadioListItem* item = (_RadioListItem*)data;
		//
		Evas_Object* radioGroup = (Evas_Object*)evas_object_data_get(obj, "radioGroup");
		if (!strcmp(part, "elm.icon.right")) {
			Evas_Object* radio = elm_radio_add(obj);

			elm_radio_state_value_set(radio, item->_index);
			elm_radio_group_add(radio, radioGroup);
			if (item->_selectedIndex == item->_index) {
				elm_radio_value_set(radio, item->_selectedIndex);
				elm_genlist_item_show(elm_genlist_nth_item_get(obj, item->_index) , ELM_GENLIST_ITEM_SCROLLTO_IN);
			}
			evas_object_size_hint_weight_set(radio, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			evas_object_size_hint_align_set(radio, EVAS_HINT_FILL, EVAS_HINT_FILL);
			return radio;

		}

		return nullptr;
	}
private:
	_RadioListItem();
	int _selectedIndex;
	int _index;
	std::list<const char*>* _itemList;

};


SmplCustomSettingsPopupController::SmplCustomSettingsPopupController() {
	_resultCb = nullptr;
	_itemList = nullptr;
	_selectedIndex = 0;
}

SmplCustomSettingsPopupController::~SmplCustomSettingsPopupController() {
	WHIT();
	delete _itemList;
}

void SmplCustomSettingsPopupController::setInfo(const char* title, std::list<const char*>& itemList, int selectedIndex) {
	setTitle(title);

	_itemList = new std::list<const char*>;
	*_itemList = itemList;
	_selectedIndex = selectedIndex;
}
void SmplCustomSettingsPopupController::setResultCb(std::function<void (int)> resultCb) {
	_resultCb = resultCb;
}

Evas_Object* SmplCustomSettingsPopupController::onCreateView(Evas_Object* parent, void* viewParam) {
	WHIT();

	setContent([this](Evas_Object * parent)->Evas_Object* {
		return _createContent(parent);
	});

	addButton("IDS_POPUP_CANCEL", [this](bool * destroyPopup) {
		if (_resultCb)
			_resultCb(-1);
		*destroyPopup = true;
	});

	setOnBack([parent](bool * destroyPopup) {
		WDEBUG("Back button is pressed!");

		*destroyPopup = false;
	});

	setTextTranslatable();

	return WPopupController::onCreateView(parent, viewParam);
}

Evas_Object* SmplCustomSettingsPopupController::_createContent(Evas_Object* parent) {
	WHIT();

	Evas_Object* genlist = elm_genlist_add(parent);
	elm_object_style_set(genlist, "popup");
	elm_genlist_homogeneous_set(genlist, EINA_TRUE);
	elm_genlist_mode_set(genlist, ELM_LIST_COMPRESS);
	elm_scroller_content_min_limit(genlist, EINA_FALSE, EINA_TRUE);   // Automatically pop-up height is calculated

	Evas_Object* radioGroup = elm_radio_add(genlist);
	elm_radio_state_value_set(radioGroup, 0);
	elm_radio_value_set(radioGroup, 0);
	evas_object_data_set(genlist, "radioGroup", radioGroup);

	elm_object_content_set(parent, genlist);

	// genlist class
	Elm_Genlist_Item_Class* itc = _RadioListItem(0, 0, _itemList).getItemClassNew();
	for (int i = 0; i < (int)_itemList->size(); i++) {
		auto item = new _RadioListItem(_selectedIndex, i, _itemList);
		wgenlist_item_append_with_callback(genlist, itc, item, ELM_GENLIST_ITEM_NONE, _itemSelectedCb, this);
	}
	elm_genlist_item_class_free(itc);
	evas_object_show(genlist);

	return genlist;
}

void SmplCustomSettingsPopupController::onDestroy() {

}

void SmplCustomSettingsPopupController::_itemSelectedCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();
	SmplCustomSettingsPopupController* p = (SmplCustomSettingsPopupController*)data;
	Elm_Object_Item* item = (Elm_Object_Item*) event_info;
	int index = elm_genlist_item_index_get(item) - 1;   // genlist item index is starting from 1
	if (p->_resultCb)
		p->_resultCb(index);

	p->destroy();
}
