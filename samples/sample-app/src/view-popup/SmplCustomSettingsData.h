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

#ifndef _SMPL_CUSTOM_SETTINGS_DATA_H_
#define _SMPL_CUSTOM_SETTINGS_DATA_H_

#include "SmplDebug.h"
#include "WSignal.h"

enum class CustomSettingsContent {
	SOUND_MODE,
	WIFI,
	BT,
	DATA_NETWORK,
	BLOCKING_MODE
};

enum class CustomSettingsValue {
	ON,
	OFF,
	SOUND,
	VIBRATE,
	MUTE
};

class SmplCustomSettingsData {
public:
	SmplCustomSettingsData();
	~SmplCustomSettingsData();
public:


public:
	void load();
	void save();
public:
	app_assist::WSignal<void (CustomSettingsContent, CustomSettingsValue)>& getChangeSignal() { return _signal; }

	CustomSettingsValue getSettingValue(CustomSettingsContent content);
	bool setSettingValue(CustomSettingsContent content, CustomSettingsValue value);
	bool getEnabledFlag(CustomSettingsContent content);
	void setEnabledFlag(CustomSettingsContent content, bool value);

	static const char* convert2SettingValueText(CustomSettingsValue value);

private:
	CustomSettingsValue _soundMode;
	CustomSettingsValue _wifi;
	CustomSettingsValue _bt;
	CustomSettingsValue _dataNetwork;
	CustomSettingsValue _blockingMode;
	bool _soundModeEnabled;
	bool _wifiEnabled;
	bool _btEnabled;
	bool _dataNetworkEnabled;
	bool _blockingModeEnabled;

	app_assist::WSignal<void (CustomSettingsContent, CustomSettingsValue)> _signal;

};


#endif // _SMPL_CUSTOM_SETTINGS_DATA_H_
