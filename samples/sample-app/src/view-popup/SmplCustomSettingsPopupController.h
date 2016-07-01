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

#ifndef _SMPL_CUSTOM_SETTINGS_POPUP_CONTROLLER_H_
#define _SMPL_CUSTOM_SETTINGS_POPUP_CONTROLLER_H_

#include "WPopupController.h"
#include <string>
#include <list>
#include <functional>

class SmplCustomSettingsPopupController: public app_assist::WPopupController {
public:
	SmplCustomSettingsPopupController();
protected:
	~SmplCustomSettingsPopupController();

public:
	void setInfo(const char* title, std::list<const char*>& itemList, int selectedIndex);
	void setResultCb(std::function<void (int)> resultCb);
private:
	virtual Evas_Object* onCreateView(Evas_Object* parent, void* viewParam) override;
	virtual void onDestroy() override;

	Evas_Object* _createContent(Evas_Object* parent);

	static void _itemSelectedCb(void* data, Evas_Object* obj, void* event_info);
private:
	std::function<void (int)> _resultCb;
	std::list<const char*>* _itemList;
	int _selectedIndex;

};

#endif // _SMPL_CUSTOM_SETTINGS_POPUP_CONTROLLER_H_
