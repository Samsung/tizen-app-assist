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

#include "SmplTabViewController.h"
#include "SmplDebug.h"

using namespace app_assist;

#define _SWALLOW_CONTENT_PART	"elm.swallow.content"

SmplTabViewController::SmplTabViewController()
	: _tabbar(nullptr),
	_alarmView(nullptr),
	_clockView(nullptr),
	_stopWatchView(nullptr) {
}

SmplTabViewController::~SmplTabViewController() {
	WHIT();
}

Evas_Object* SmplTabViewController::onCreateView(Evas_Object* parent, void* viewParam) {

	Evas_Object* layout = elm_layout_add(parent);
	elm_layout_theme_set(layout, "layout", "application", "default");
	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(layout);

	return layout;

}
void SmplTabViewController::onDestroy() {
	WENTER();
	elm_object_part_content_unset(getEvasObject(), _SWALLOW_CONTENT_PART);

	if (_alarmView) _alarmView->destroy();
	if (_clockView) _clockView->destroy();
	if (_stopWatchView) _stopWatchView->destroy();

	_alarmView = nullptr;
	_clockView = nullptr;
	_stopWatchView = nullptr;

	WLEAVE();
}


Elm_Object_Item* SmplTabViewController::onPushToNaviframe(Evas_Object* naviFrame) {
	WHIT();
	return elm_naviframe_item_push(naviFrame, nullptr, nullptr, nullptr, getEvasObject(), "tabbar/notitle");
}

void SmplTabViewController::onPushed(Elm_Object_Item* naviItem) {
	Evas_Object* tabbar = elm_toolbar_add(getNaviframeController()->getEvasObject());
	elm_toolbar_shrink_mode_set(tabbar, ELM_TOOLBAR_SHRINK_EXPAND);
	elm_toolbar_transverse_expanded_set(tabbar, EINA_TRUE);
	elm_object_style_set(tabbar, "tabbar");
	elm_toolbar_select_mode_set(tabbar, ELM_OBJECT_SELECT_MODE_ALWAYS);

	Elm_Object_Item* item;
	item = elm_toolbar_item_append(tabbar, nullptr, "IDS_TAB_ALARM", _tabAlarmChangedCb, this);
	elm_object_item_text_translatable_set(item, EINA_TRUE);
	item = elm_toolbar_item_append(tabbar, nullptr, "IDS_TAB_CLOCK", _tabClockChangedCb, this);
	elm_object_item_text_translatable_set(item, EINA_TRUE);
	item = elm_toolbar_item_append(tabbar, nullptr, "IDS_TAB_STOPWATCH", _tabStopwatchChangedCb, this);
	elm_object_item_text_translatable_set(item, EINA_TRUE);

	elm_object_item_part_content_set(naviItem, "tabbar", tabbar);

}

void SmplTabViewController::_tabAlarmChangedCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();

	SmplTabViewController* self = (SmplTabViewController*)data;
	self->_unsetPrevContent();

	if (self->_alarmView == nullptr) {
		self->_alarmView = new WContentViewController("Alarm");
	}

	self->setPartContent(_SWALLOW_CONTENT_PART, self->_alarmView);
}

void SmplTabViewController::_tabClockChangedCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();


	SmplTabViewController* self = (SmplTabViewController*)data;
	self->_unsetPrevContent();

	if (self->_clockView == nullptr) {
		self->_clockView = new WContentViewController("Clock");
	}

	self->setPartContent(_SWALLOW_CONTENT_PART, self->_clockView);
}

void SmplTabViewController::_tabStopwatchChangedCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();

	SmplTabViewController* self = (SmplTabViewController*)data;
	self->_unsetPrevContent();

	if (self->_stopWatchView == nullptr) {
		self->_stopWatchView = new WContentViewController("Stopwatch");
	}

	self->setPartContent(_SWALLOW_CONTENT_PART, self->_stopWatchView);
}

void SmplTabViewController::_unsetPrevContent() {
	WViewController* oldContent = unsetPartContent(_SWALLOW_CONTENT_PART);
	if (oldContent)
		evas_object_hide(oldContent->getEvasObject());
}
