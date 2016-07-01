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

#include "SmplCustomSettingsListItem.h"
#include "SmplCustomSettingsPopupController.h"
#include "SmplDebug.h"

SmplCustomSettingsListItem::_ContentInfo SmplCustomSettingsListItem::_contentInfo[SmplCustomSettingsListItem::ITEM_NUM] = {
	{ "Sound mode", CustomSettingsContent::SOUND_MODE },
	{ "Wi-Fi", CustomSettingsContent::WIFI },
	{ "Bluetooth", CustomSettingsContent::BT },
	{ "Use data network", CustomSettingsContent::DATA_NETWORK },
	{ "Blocking mode", CustomSettingsContent::BLOCKING_MODE }
};

SmplCustomSettingsListItem::SmplCustomSettingsListItem(int index, std::shared_ptr<SmplCustomSettingsData>& settingData) {
	WASSERT(index >= 0 && index < SmplCustomSettingsListItem::ITEM_NUM);
	WASSERT(settingData != nullptr);

	_settingData = settingData;
	_index = index;
}

Elm_Genlist_Item_Class* SmplCustomSettingsListItem::getItemClassNew() const {
	Elm_Genlist_Item_Class* itc = elm_genlist_item_class_new();
	itc->item_style = "2line.top"; //"2text.2icon.7";
	itc->func.text_get = _glTextGetCb;
	itc->func.content_get = _glContentGetCb;
	itc->func.del = WGENLIST_ITEM_DEL();
	return itc;
}

void SmplCustomSettingsListItem::onSelect(Elm_Object_Item* objItem) {
	WHIT();
	SmplCustomSettingsPopupController* popup = new SmplCustomSettingsPopupController();

	static const char* const soundValue[] = { "Sound", "Vibrate", "Mute" };
	static const char* const normalValue[] = { "ON", "OFF" };
	int vindex = _convertValue2Index(_contentInfo[_index].content, _settingData->getSettingValue(_contentInfo[_index].content));
	if (_index  == 0) {
		std::list<const char*> list(soundValue, soundValue + sizeof(soundValue) / sizeof(const char*));
		popup->setInfo(_contentInfo[_index].title, list, vindex);
	}
	else {
		std::list<const char*> list(normalValue, normalValue + sizeof(normalValue) / sizeof(const char*));
		popup->setInfo(_contentInfo[_index].title, list, vindex);
	}
	Evas_Object* genlist = elm_object_item_widget_get(objItem);
	popup->setResultCb([this, genlist](int index)	{
		//
		WHIT();
		if (index != -1) {
			_settingData->setSettingValue(_contentInfo[_index].content, _convertIndex2Value(_contentInfo[_index].content, index));
			elm_genlist_realized_items_update(genlist);
		}
	});
	//
	attachToParentViewController(popup);
}

char* SmplCustomSettingsListItem::_glTextGetCb(void* data, Evas_Object* obj, const char* part) {
	auto item = (SmplCustomSettingsListItem*)data;

	if (!strcmp(part, "elm.text.sub.left.bottom")) {
		return strdup(SmplCustomSettingsData::convert2SettingValueText(item->_settingData->getSettingValue(_contentInfo[item->_index].content)));
	}
	else if (!strcmp(part, "elm.text.main.left.top")) {
		return strdup(_contentInfo[item->_index].title);
	}

	return nullptr;
}

Evas_Object* SmplCustomSettingsListItem::_glContentGetCb(void* data, Evas_Object* obj, const char* part) {
	auto item = (SmplCustomSettingsListItem*)data;

	if (!strcmp(part, "elm.icon.right")) {  //"elm.icon.1"))
		Evas_Object* icon = item->_getCheckIcon(obj);
		evas_object_smart_callback_add(icon, "clicked", _checkSelectedCb, item);
		return icon;
	}

	return nullptr;
}

Evas_Object* SmplCustomSettingsListItem::_getCheckIcon(Evas_Object* obj) {
	Evas_Object* layout = elm_layout_add(obj);
	elm_layout_theme_set(layout, "layout", "list/C/type.2", "default");

	Evas_Object* icon = elm_check_add(obj);

	elm_object_style_set(icon, "default");
	evas_object_repeat_events_set(icon, EINA_FALSE);
	evas_object_propagate_events_set(icon, EINA_FALSE);

	if (_settingData->getEnabledFlag(_contentInfo[_index].content)) {
		elm_check_state_set(icon, EINA_TRUE);
	}
	else {
		elm_check_state_set(icon, EINA_FALSE);
	}

	elm_layout_content_set(layout, "elm.swallow.content", icon);

	return layout;//icon;
}
void SmplCustomSettingsListItem::_checkSelectedCb(void* data, Evas_Object* obj, void* eventInfo) {

	WHIT();
	auto item = (SmplCustomSettingsListItem*)data;

	Elm_Object_Item* objItem = elm_genlist_selected_item_get(obj);
	elm_genlist_item_selected_set(objItem, EINA_FALSE);
	// Update check button
	Evas_Object* ck = elm_object_item_part_content_get((const Elm_Object_Item*)eventInfo, "elm.icon.right");

	if (ck) {
		Eina_Bool state = elm_check_state_get(ck);
		elm_check_state_set(ck, !state);

		item->_settingData->setEnabledFlag(_contentInfo[item->_index].content, !state);
	}
}

int SmplCustomSettingsListItem::_convertValue2Index(CustomSettingsContent content, CustomSettingsValue value) {
	CustomSettingsValue v = _settingData->getSettingValue(content);
	if ((int)v >= (int)CustomSettingsValue::SOUND) {
		return (int)v - (int)CustomSettingsValue::SOUND;
	}
	return (int)v;
}

CustomSettingsValue SmplCustomSettingsListItem::_convertIndex2Value(CustomSettingsContent content, int index) {
	if (content == CustomSettingsContent::SOUND_MODE) {
		return (CustomSettingsValue)(index + 2);
	}
	return (CustomSettingsValue)index;
}
