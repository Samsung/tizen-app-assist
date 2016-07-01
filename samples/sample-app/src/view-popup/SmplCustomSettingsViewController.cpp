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

#include "SmplCustomSettingsViewController.h"
#include "WTimer.h"
#include "WPopupController.h"
#include "SmplDebug.h"
#include "SmplCustomSettingsListItem.h"
#include "SmplSignalManager.h"

using namespace app_assist;

SmplCustomSettingsViewController::SmplCustomSettingsViewController()
	: _genlist(nullptr), _listenerId(WSIGNAL_INVALID_ID) {
}

SmplCustomSettingsViewController::~SmplCustomSettingsViewController() {
	WHIT();
}

void SmplCustomSettingsViewController::onDestroy() {
	WTimer::destroy(_longTimer);
}

Evas_Object* SmplCustomSettingsViewController::onCreateView(Evas_Object* parent, void* viewParam) {
	WHIT();

	setTitle("View and Popup");

	_settingData = std::make_shared<SmplCustomSettingsData>();
	_settingData->load();

	Evas_Object* layout = elm_layout_add(parent);
	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_theme_set(layout, "layout", "application", "default");

	_genlist = elm_genlist_add(parent);

	elm_genlist_mode_set(_genlist, ELM_LIST_COMPRESS);
	evas_object_data_set(_genlist, "SmplCustomSettingsViewController", this);
	evas_object_data_set(_genlist, "parent", parent);
	evas_object_size_hint_weight_set(_genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(_genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);

	elm_object_part_content_set(layout, "elm.swallow.content", _genlist);


	Elm_Genlist_Item_Class* itcGroup = elm_genlist_item_class_new();
	itcGroup->item_style = "group_index";
	itcGroup->func.text_get = [](void* data, Evas_Object * obj, const char* part)->char*	{
		if (!strcmp(part, "elm.text")) return strdup("Custom settings");
		return nullptr;
	};

	Elm_Object_Item* objItem = elm_genlist_item_append(_genlist, itcGroup, nullptr, nullptr, ELM_GENLIST_ITEM_NONE, nullptr, nullptr);
	elm_genlist_item_select_mode_set(objItem, ELM_OBJECT_SELECT_MODE_NONE);

	Elm_Genlist_Item_Class* itc = SmplCustomSettingsListItem(0, _settingData).getItemClassNew();
	for (int i = 0; i < SmplCustomSettingsListItem::ITEM_NUM; i++) {
		auto listItem = new SmplCustomSettingsListItem(i, _settingData);
		wgenlist_item_append(_genlist, itc, listItem, ELM_GENLIST_ITEM_NONE);
	}

	elm_genlist_item_class_free(itcGroup);
	elm_genlist_item_class_free(itc);
	evas_object_show(layout);

	return layout;
}

void SmplCustomSettingsViewController::onCreated() {
	_listenerId = _settingData->getChangeSignal().addListener([this](CustomSettingsContent content, CustomSettingsValue value) {
		_onSettingsChanged(content, value);

		WDEBUG("First change is detected! The listener is removed!");
		_settingData->getChangeSignal().removeListener(this->_listenerId);

	} , getEvasObject());

	SmplSignalManager::getInstance()->getAppStateSignal().addListener([this](const char* state)->bool {
		if (strcmp(state, "resume") == 0) {
			if (getNaviframeController()->isTopContentViewController(this)) {
				WDEBUG("I'm on top! Add another listener instead of me!");

				SmplSignalManager::getInstance()->getAppStateSignal().addListener([this](const char* state)->bool {

					WDEBUG("I'm on another listener! state=%s!", state);
					return true;

				}, getEvasObject());
				return false;
			}
		}
		return true;

	}, getEvasObject());

}

void SmplCustomSettingsViewController::onPushed(Elm_Object_Item* naviItem) {
	WENTER();
	//Title Icon Right Button
	Evas_Object* cancelBtn = elm_button_add(getNaviframeController()->getEvasObject());
	elm_object_style_set(cancelBtn, "naviframe/title_cancel");
	evas_object_smart_callback_add(cancelBtn, "clicked", _cancelButtonClickedCb, this);

	Evas_Object* saveBtn = elm_button_add(getNaviframeController()->getEvasObject());
	elm_object_style_set(saveBtn, "naviframe/title_done");
	evas_object_smart_callback_add(saveBtn, "clicked", _saveButtonClickedCb, this);

	elm_object_item_part_content_set(naviItem, "title_left_btn", cancelBtn);
	elm_object_item_part_content_set(naviItem, "title_right_btn", saveBtn);

	WTimer::addTimer(2.0, [](void* data)->bool {

		auto p = (SmplCustomSettingsViewController*)data;
		WPopupController* popup = new WPopupController("Timer Test", "Called on Timer");
		p->attachPopup(popup);
		return false;

	}, this, getEvasObject());

	WTimer::destroy(_longTimer);

	_longTimer = WTimer::addTimer(10.0, [](void* data)->bool {
		auto p = (SmplCustomSettingsViewController*)data;
		WPopupController* popup = new WPopupController("Timer Test", "Called on Long Timer!!!");
		p->attachPopup(popup);
		return false;
	}, this, getEvasObject());

	WLEAVE();
}

void SmplCustomSettingsViewController::_cancelButtonClickedCb(void* data, Evas_Object* obj, void* eventInfo) {
	WHIT();
	SmplCustomSettingsViewController* p = (SmplCustomSettingsViewController*)data;
	p->popOut();
}

void SmplCustomSettingsViewController::_saveButtonClickedCb(void* data, Evas_Object* obj, void* eventInfo) {
	WHIT();
	SmplCustomSettingsViewController* p = (SmplCustomSettingsViewController*)data;
	p->_settingData->save();
	p->popOut();
}

void SmplCustomSettingsViewController::_onSettingsChanged(CustomSettingsContent content, CustomSettingsValue value) {
	WDEBUG("Settings value is changed %d = %d", (int)content, (int)value);
}
