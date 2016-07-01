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

#include "SmplCustomSettingsData.h"
#include "SmplDebug.h"

SmplCustomSettingsData::SmplCustomSettingsData()
	: _soundMode(CustomSettingsValue::SOUND),
	_wifi(CustomSettingsValue::OFF),
	_bt(CustomSettingsValue::OFF),
	_dataNetwork(CustomSettingsValue::OFF),
	_blockingMode(CustomSettingsValue::OFF),
	_soundModeEnabled(false),
	_wifiEnabled(false),
	_btEnabled(false),
	_dataNetworkEnabled(false),
	_blockingModeEnabled(false) {
	WHIT();
}

SmplCustomSettingsData::~SmplCustomSettingsData() {
	WHIT();
}

void SmplCustomSettingsData::load() {
	WDEBUG("data is loaded!");
}

void SmplCustomSettingsData::save() {
	WDEBUG("data is saved!");
}

CustomSettingsValue SmplCustomSettingsData::getSettingValue(CustomSettingsContent content) {
	if (content == CustomSettingsContent::SOUND_MODE)
		return _soundMode;
	else if (content == CustomSettingsContent::WIFI)
		return _wifi;
	else if (content == CustomSettingsContent::BT)
		return _bt;
	else if (content == CustomSettingsContent::DATA_NETWORK)
		return _dataNetwork;
	else if (content == CustomSettingsContent::BLOCKING_MODE)
		return _blockingMode;

	WERROR("Undefined!!!! content=%d", content);

	return CustomSettingsValue::ON;
}

bool SmplCustomSettingsData::setSettingValue(CustomSettingsContent content, CustomSettingsValue value) {
	WHIT();

	if (content == CustomSettingsContent::SOUND_MODE)
		_soundMode = value;
	else if (content == CustomSettingsContent::WIFI)
		_wifi = value;
	else if (content == CustomSettingsContent::BT)
		_bt = value;
	else if (content == CustomSettingsContent::DATA_NETWORK)
		_dataNetwork = value;
	else if (content == CustomSettingsContent::BLOCKING_MODE)
		_blockingMode = value;
	else
		WASSERT(0);
	//
	_signal.emit(content, value);

	return true;
}

bool SmplCustomSettingsData::getEnabledFlag(CustomSettingsContent content) {
	if (content == CustomSettingsContent::SOUND_MODE)
		return _soundModeEnabled;
	else if (content == CustomSettingsContent::WIFI)
		return _wifiEnabled;
	else if (content == CustomSettingsContent::BT)
		return _btEnabled;
	else if (content == CustomSettingsContent::DATA_NETWORK)
		return _dataNetworkEnabled;
	else if (content == CustomSettingsContent::BLOCKING_MODE)
		return _blockingModeEnabled;

	WASSERT(0);

	return false;
}

void SmplCustomSettingsData::setEnabledFlag(CustomSettingsContent content, bool value) {
	WDEBUG("%d, v=%d", content, value);
	if (content == CustomSettingsContent::SOUND_MODE)
		_soundModeEnabled = value;
	else if (content == CustomSettingsContent::WIFI)
		_wifiEnabled = value;
	else if (content == CustomSettingsContent::BT)
		_btEnabled = value;
	else if (content == CustomSettingsContent::DATA_NETWORK)
		_dataNetworkEnabled = value;
	else if (content == CustomSettingsContent::BLOCKING_MODE)
		_blockingModeEnabled = value;
	else
		WASSERT(0);
}

const char* SmplCustomSettingsData::convert2SettingValueText(CustomSettingsValue value) {
	if (value == CustomSettingsValue::SOUND)
		return "Sound";
	else if (value == CustomSettingsValue::VIBRATE)
		return "Vibrate";
	else if (value == CustomSettingsValue::MUTE)
		return "Mute";
	else if (value == CustomSettingsValue::OFF)
		return "OFF";
	else if (value == CustomSettingsValue::ON)
		return "ON";
	else
		WASSERT(0);

	return nullptr;
}
