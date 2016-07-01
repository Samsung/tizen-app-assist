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

#ifndef _SMPL_CUSTOM_SETTINGS_VIEW_CONTROLLER_H_
#define _SMPL_CUSTOM_SETTINGS_VIEW_CONTROLLER_H_

#include <memory>
#include "WContentViewController.h"
#include "WTimer.h"
#include "SmplCustomSettingsData.h"
#include "WSignal.h"

class SmplCustomSettingsViewController: public app_assist::WContentViewController {
public:
	SmplCustomSettingsViewController();
protected:
	~SmplCustomSettingsViewController();

private:
	virtual Evas_Object* onCreateView(Evas_Object* parent, void* viewParam) override;
	virtual void onCreated() override;
	virtual void onDestroy() override;
	virtual void onPushed(Elm_Object_Item* naviItem) override;
private:
	static char* _getItemTitle(int index);
	static Evas_Object* _getCheckIcon(void* data, Evas_Object* obj, const char* part);
	//
	static void _cancelButtonClickedCb(void* data, Evas_Object* obj, void* eventInfo);
	static void _saveButtonClickedCb(void* data, Evas_Object* obj, void* eventInfo);

	static char* _glTextGetCb(void* data, Evas_Object* obj, const char* part);
	static Evas_Object* _glContentGetCb(void* data, Evas_Object* obj, const char* part);

	static void _checkSelectedCb(void* data, Evas_Object* obj, void* eventInfo);
	static void _revealIconSelectedCb(void* data, Evas_Object* obj, void* eventInfo);

	void _onSettingsChanged(CustomSettingsContent content, CustomSettingsValue value);
private:
	std::shared_ptr<SmplCustomSettingsData> _settingData;
	Evas_Object* _genlist;
	app_assist::WTimerWeakPtr _longTimer;
	app_assist::WSignal_ListenerId _listenerId;
};

#endif // _SMPL_CUSTOM_SETTINGS_VIEW_CONTROLLER_H_
