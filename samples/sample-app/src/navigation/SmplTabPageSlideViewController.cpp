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

#include "SmplTabPageSlideViewController.h"
#include "WPageSlideViewController.h"
#include "SmplDebug.h"


using namespace app_assist;

#define _SWALLOW_CONTENT_PART	"elm.swallow.content"

SmplTabPageSlideViewController::SmplTabPageSlideViewController()
	: _tabbar(nullptr),
	_alarmView(nullptr),
	_clockView(nullptr),
	_stopwatchView(nullptr),
	_timerView(nullptr) {
	for (int i = 0; i < _ITEM_NUM; i++)
		_tabItem[i] = nullptr;
}

SmplTabPageSlideViewController::~SmplTabPageSlideViewController() {
	WHIT();
}

Evas_Object* SmplTabPageSlideViewController::onCreateView(Evas_Object* parent, void* viewParam) {
	Evas_Object* layout = elm_layout_add(parent);
	elm_layout_theme_set(layout, "layout", "application", "default");
	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	setOnInitScroller([](Evas_Object * scroller) {
		elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
	});

	Evas_Object* slideViewObj = WPageSlideViewController::onCreateView(layout, nullptr);

	elm_object_part_content_set(layout, _SWALLOW_CONTENT_PART, slideViewObj);
	evas_object_smart_callback_add(getScroller(), "scroll,anim,stop", _pageScrollStopCb, this);

	evas_object_show(layout);

	return layout;

}
void SmplTabPageSlideViewController::onDestroy() {

}

Elm_Object_Item* SmplTabPageSlideViewController::onPushToNaviframe(Evas_Object* naviFrame) {
	WHIT();
	return elm_naviframe_item_push(naviFrame, nullptr, nullptr, nullptr, getEvasObject(), "tabbar/notitle");
}

void SmplTabPageSlideViewController::onPushed(Elm_Object_Item* naviItem) {
	Evas_Object* tabbar = elm_toolbar_add(getNaviframeController()->getEvasObject());
	elm_toolbar_shrink_mode_set(tabbar, ELM_TOOLBAR_SHRINK_SCROLL);
	elm_toolbar_transverse_expanded_set(tabbar, EINA_TRUE);
	elm_object_style_set(tabbar, "tabbar");
	elm_toolbar_select_mode_set(tabbar, ELM_OBJECT_SELECT_MODE_ALWAYS);

	elm_object_item_part_content_set(naviItem, "tabbar", tabbar);

	_tabItem[0] = elm_toolbar_item_append(tabbar, nullptr, "IDS_TAB_ALARM", _tabAlarmChangedCb, this);
	_tabItem[1] = elm_toolbar_item_append(tabbar, nullptr, "IDS_TAB_CLOCK", _tabClockChangedCb, this);
	_tabItem[2] = elm_toolbar_item_append(tabbar, nullptr, "IDS_TAB_STOPWATCH", _tabStopwatchChangedCb, this);
	_tabItem[3] = elm_toolbar_item_append(tabbar, nullptr, "IDS_TAB_TIMER", _tabTimerChangedCb, this);

	elm_object_item_text_translatable_set(_tabItem[0], EINA_TRUE);
	elm_object_item_text_translatable_set(_tabItem[1], EINA_TRUE);
	elm_object_item_text_translatable_set(_tabItem[2], EINA_TRUE);
	elm_object_item_text_translatable_set(_tabItem[3], EINA_TRUE);

	_tabbar = tabbar;

	// Attach page here! if the view to be attached requires naviItem of PageSlideView when its onSetAsPageContent method is called.
	appendPage(_alarmView = new WContentViewController("alarm"));
	appendPage(_clockView = new WContentViewController("clock"));
	appendPage(_clockView = new WContentViewController("stopwatch"));
	appendPage(_timerView = new WContentViewController("timer"));
}

void SmplTabPageSlideViewController::_jumpToPage(int index) {
	elm_toolbar_item_selected_set(_tabItem[index], EINA_TRUE);
	showPage(index, false);
}

void SmplTabPageSlideViewController::_tabAlarmChangedCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();

	SmplTabPageSlideViewController* self = (SmplTabPageSlideViewController*)data;
	Evas_Object* scroller = self->getScroller();
	elm_scroller_page_show(scroller, 0, 0);
}

void SmplTabPageSlideViewController::_tabClockChangedCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();

	SmplTabPageSlideViewController* self = (SmplTabPageSlideViewController*)data;
	Evas_Object* scroller = self->getScroller();
	elm_scroller_page_show(scroller, 1, 0);
}

void SmplTabPageSlideViewController::_tabStopwatchChangedCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();

	SmplTabPageSlideViewController* self = (SmplTabPageSlideViewController*)data;
	Evas_Object* scroller = self->getScroller();
	elm_scroller_page_show(scroller, 2, 0);
}
void SmplTabPageSlideViewController::_tabTimerChangedCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();

	SmplTabPageSlideViewController* self = (SmplTabPageSlideViewController*)data;
	Evas_Object* scroller = self->getScroller();
	elm_scroller_page_show(scroller, 3, 0);
}

void SmplTabPageSlideViewController::_pageScrollStopCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();
	SmplTabPageSlideViewController* self = (SmplTabPageSlideViewController*)data;

	int hpage = 0;
	elm_scroller_current_page_get(obj, &hpage, nullptr);
	elm_toolbar_item_selected_set(self->_tabItem[hpage], EINA_TRUE);

}
