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

#ifndef _SMPL_CUSTOM_SETTING_LIST_ITEM_H_
#define _SMPL_CUSTOM_SETTING_LIST_ITEM_H_

#include "WGenlistItem.h"
#include "SmplCustomSettingsData.h"

class SmplCustomSettingsListItem: public app_assist::WGenlistItem {
public:
	SmplCustomSettingsListItem(int index, std::shared_ptr<SmplCustomSettingsData>& settingData);
	virtual ~SmplCustomSettingsListItem() {};
public:
	static const int ITEM_NUM = 5;

	virtual Elm_Genlist_Item_Class* getItemClassNew() const override;
private:
	virtual void onSelect(Elm_Object_Item* objItem) override;
private:
	Evas_Object* _getCheckIcon(Evas_Object* obj);
	static char* _glTextGetCb(void* data, Evas_Object* obj, const char* part);
	static Evas_Object* _glContentGetCb(void* data, Evas_Object* obj, const char* part);
	static void _checkSelectedCb(void* data, Evas_Object* obj, void* eventInfo);
	int _convertValue2Index(CustomSettingsContent content, CustomSettingsValue value);
	CustomSettingsValue _convertIndex2Value(CustomSettingsContent content, int index);
private:
	SmplCustomSettingsListItem() = delete;
private:
	std::shared_ptr<SmplCustomSettingsData> _settingData;
	int _index;
	//
	struct _ContentInfo {
		const char* title;
		CustomSettingsContent content;

	};
	static _ContentInfo _contentInfo[SmplCustomSettingsListItem::ITEM_NUM];
};

#endif /* _SMPL_CUSTOM_SETTING_LIST_ITEM_H_ */
