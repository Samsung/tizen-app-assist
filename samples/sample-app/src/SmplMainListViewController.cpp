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

#include "SmplMainListViewController.h"
#include <app.h>

#include "WMenuPopupController.h"
#include "WPopupController.h"
#include "WTimer.h"
#include "IWWindowController.h"

#include "SmplTabViewController.h"
#include "SmplTabPageSlideViewController.h"
#include "SmplCustomSettingsViewController.h"
#include "SmplDebug.h"
#include "SmplDefine.h"

using namespace app_assist;

SmplMainListViewController::MenuIts SmplMainListViewController::_menuIts[MENU_ITEM_NUM] = {
	{ true, "IDS_MENU_GROUP_VIEW_NAVIGATION", nullptr }, // group title
	{ false, "IDS_MENU_NAVIFRAME", _showNaviframe },
	{ false, "IDS_MENU_TAB_VIEW", _showTabView },
	{ false, "IDS_MENU_PAGE_SLIDE_VIEW", _showPageSlideView },
	{ false, "IDS_MENU_TAB_PAGE_SLIDE_VIEW", _showTabPageSlideView },
	{ true, "Utility", nullptr }, // group title
	{ true, "IDS_MENU_GROUP_PRACTICAL_EXAMPLES", nullptr }, // group title
	{ false, "View and Popup", _showViewPopup },
	{ false, "Show Popup on timer", _showPopupOnTimer },
	{ false, "App-control (contacts-pick)", _launchAppControl }
};

SmplMainListViewController::SmplMainListViewController() {
}

SmplMainListViewController::~SmplMainListViewController() {
}

Evas_Object* SmplMainListViewController::onCreateView(Evas_Object* parent, void* viewParam) {
	setTitle("IDS_MAIN_TITLE");

	Elm_Genlist_Item_Class* itc = elm_genlist_item_class_new();
	Elm_Genlist_Item_Class* itcGroup = elm_genlist_item_class_new();
	//
	itc->item_style = "default";
	itc->func.text_get = _glTextGet;

	itcGroup->item_style = "group_index";
	itcGroup->func.text_get = _glTextGet;

	Evas_Object* genlist = elm_genlist_add(parent);
	elm_genlist_homogeneous_set(genlist, EINA_TRUE);

	Elm_Object_Item* git = nullptr;
	for (int index = 0; index < MENU_ITEM_NUM; index++) {
		if (_menuIts[index].isGroup == true) {
			git = elm_genlist_item_append(genlist, itcGroup, (void*)index, nullptr, ELM_GENLIST_ITEM_NONE, nullptr, nullptr);
			elm_genlist_item_select_mode_set(git, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);
		}
		else {
			elm_genlist_item_append(genlist, itc, (void*)index, git,
									ELM_GENLIST_ITEM_NONE, _listSelectedCb, (void*)index);
		}
	}
	elm_genlist_item_class_free(itc);
	elm_genlist_item_class_free(itcGroup);
	return genlist;
}

void SmplMainListViewController::onDestroy() {

}

void SmplMainListViewController::onPushed(Elm_Object_Item* naviItem) {
	elm_object_item_text_translatable_set(naviItem, EINA_TRUE);
	//more Button
	setMoreButton(naviItem, _moreBtnSelectCb, this);
}

void SmplMainListViewController::_moreBtnSelectCb(void* data, Evas_Object* obj, void* event_info) {
	WHIT();
	SmplMainListViewController* p = (SmplMainListViewController*)data;

	WMenuPopupController* menu = new WMenuPopupController();
	menu->createMenu(p->getWindowController()->getEvasObject(), p->getNaviframeController()->getEvasObject());

	menu->appendItem("Menu Test 1", [p]() {
		WPopupController* popup = new WPopupController("Menu Selected!", "1");
		p->attachPopup(popup);
	});
	menu->appendItem("Menu Test 2", [p]() {
		WPopupController* popup = new WPopupController("Menu Selected!", "2");
		popup->addButton("OK", nullptr);
		p->attachPopup(popup);
	});
	menu->appendItem("Close the application", [p]() {
		WPopupController* popup = new WPopupController("Exit?", nullptr);
		popup->addButton("OK", [p](bool * destroyPopup) {
			p->popOut();
			*destroyPopup = true;
		});
		popup->addButton("Cancel", [](bool * destroyPopup) {
			*destroyPopup = true;
		});
		p->attachPopup(popup);
	});
	menu->showMenu();
	//
	p->attachPopup(menu);
}

char* SmplMainListViewController::_glTextGet(void* data, Evas_Object* obj, const char* part) {
	int index = (int)data;

	if (!strcmp(part, "elm.text"))  // title
		return strdup(_T(_menuIts[index].title));
	else
		return nullptr;
}
void SmplMainListViewController::_listSelectedCb(void* data, Evas_Object* obj, void* event_info) {
	int index = (int)data;
	SmplMainListViewController* vc = (SmplMainListViewController*)wviewcontroller_get_instance_from_evasobj(obj);

	elm_genlist_item_selected_set((Elm_Object_Item*)event_info, EINA_FALSE);
	_menuIts[index].func(vc, nullptr, nullptr);
	return;
}

void SmplMainListViewController::_showNaviframe(void* data, Evas_Object* btn, void* eventInfo) {
	WHIT();

	SmplMainListViewController* self = (SmplMainListViewController*)data;
	WContentViewController* vc = new WContentViewController("Hello World!");
	self->getNaviframeController()->push(vc);

}

void SmplMainListViewController::_showTabView(void* data, Evas_Object* btn, void* eventInfo) {
	WHIT();

	SmplMainListViewController* self = (SmplMainListViewController*)data;

	WContentViewController* vc = new SmplTabViewController;
	vc->create(self->getNaviframeController()->getEvasObject(), nullptr);

	self->getNaviframeController()->push(vc);

}

void SmplMainListViewController::_showPageSlideView(void* data, Evas_Object* btn, void* eventInfo) {
	WHIT();


	SmplMainListViewController* self = (SmplMainListViewController*)data;
	WPageSlideViewController* vc = new WPageSlideViewController();
	vc->create(self->getNaviframeController()->getEvasObject(), nullptr);
	vc->appendPage(new WContentViewController("Page1"));
	vc->appendPage(new WContentViewController("Page2"));
	vc->appendPage(new WContentViewController("Page3"));

	self->getNaviframeController()->push(vc);


}

void SmplMainListViewController::_showTabPageSlideView(void* data, Evas_Object* btn, void* eventInfo) {
	WHIT();


	SmplMainListViewController* self = (SmplMainListViewController*)data;

	self->getNaviframeController()->push(new SmplTabPageSlideViewController());
}

void SmplMainListViewController::_showViewPopup(void* data, Evas_Object* btn, void* eventInfo) {
	WHIT();
	SmplMainListViewController* self = (SmplMainListViewController*)data;
	self->getNaviframeController()->push(new SmplCustomSettingsViewController() , "customView");

}

void SmplMainListViewController::_showPopupOnTimer(void* data, Evas_Object* btn, void* eventInfo) {
	WHIT();
	SmplMainListViewController* self = (SmplMainListViewController*)data;

	WTimer::addAnimator([](void* data)->Eina_Bool {

		auto p = (SmplMainListViewController*)data;
		WPopupController* popup = new WPopupController("Timer Test", "Called on Animator!");
		p->attachPopup(popup);
		return ECORE_CALLBACK_CANCEL;

	}, self, self->getEvasObject());
}

void SmplMainListViewController::_launchAppControl(void* data, Evas_Object* btn, void* eventInfo) {
	WHIT();
	auto self = static_cast<SmplMainListViewController*>(data);

	app_control_h request = nullptr;
	app_control_create(&request);

	app_control_set_operation(request, "http://tizen.org/appcontrol/operation/social/pick");
	app_control_set_mime(request, "vnd.tizen.item.type/vnd.tizen.contact");
	app_control_add_extra_data(request, "http://tizen.org/appcontrol/data/selection_mode", "single");
	app_control_add_extra_data(request, "http://tizen.org/appcontrol/data/social/item_type", "person");
	app_control_add_extra_data(request, "http://tizen.org/appcontrol/data/social/result_type", "item_id");

	int ret = app_control_send_launch_request(request,
	[](app_control_h request, app_control_h reply, app_control_result_e result, void* userData) {
		WDEBUG("reply callback called!");
		//
		SmplMainListViewController* p = (SmplMainListViewController*)userData;
		char** id = nullptr;
		int len = 0;

		app_control_get_extra_data_array(reply, "http://tizen.org/appcontrol/data/social/item_id", &id, &len);
		WDEBUG("len=%d", len);
		WDEBUG("value=%s", id[0]);

		WPopupController* popup = new WPopupController("Selected item - Person Id!", id[0]);
		p->attachPopup(popup);

		for (int i = 0; i < len; i++)
			free(id[i]);
		free(id);
		//
	}
	, self);
	if (ret != APP_CONTROL_ERROR_NONE) {
		WERROR("error code = 0x%x", ret);
	}
	app_control_destroy(request);
}
