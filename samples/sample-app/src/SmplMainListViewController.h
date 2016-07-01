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

#ifndef _SMPL_MAIN_LIST_VIEW_CONTROLLER_H_
#define _SMPL_MAIN_LIST_VIEW_CONTROLLER_H_

#include "WContentViewController.h"

class SmplMainListViewController: public app_assist::WContentViewController {
public:
	SmplMainListViewController();
protected:
	~SmplMainListViewController();

private:
	virtual Evas_Object* onCreateView(Evas_Object* parent, void* viewParam) override;
	virtual void onDestroy() override;

	virtual void onPushed(Elm_Object_Item* naviItem) override;
private:
	static void _moreBtnSelectCb(void* data, Evas_Object* obj, void* event_info);
	//
	static char* _glTextGet(void* data, Evas_Object* obj, const char* part);
	static void _listSelectedCb(void* data, Evas_Object* obj, void* eventInfo);
	//
	static void _showNaviframe(void* data, Evas_Object* btn, void* eventInfo);
	static void _showTabView(void* data, Evas_Object* btn, void* eventInfo);
	static void _showPageSlideView(void* data, Evas_Object* btn, void* eventInfo);
	static void _showTabPageSlideView(void* data, Evas_Object* btn, void* eventInfo);
	static void _showViewPopup(void* data, Evas_Object* btn, void* eventInfo);
	static void _showToastPopup(void* data, Evas_Object* btn, void* eventInfo);
	static void _showPopupOnTimer(void* data, Evas_Object* btn, void* eventInfo);
	static void _showUG(void* data, Evas_Object* btn, void* eventInfo);
	static void _launchAppControl(void* data, Evas_Object* btn, void* eventInfo);

private:
	static const int MENU_ITEM_NUM = 10;

	typedef struct {
		bool isGroup;
		const char* title;
		void (*func)(void* data, Evas_Object* btn, void* eventInfo);
	} MenuIts;

	static MenuIts _menuIts[MENU_ITEM_NUM];
};



#endif // _SMPL_MAIN_LIST_VIEW_CONTROLLER_H_
