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

#include "SmplWatchFaceViewController.h"

using namespace app_assist;

#include <Elementary.h>
#include "SmplWatchDebug.h"

SmplWatchFaceViewController::SmplWatchFaceViewController(int width, int height)
	: _label(nullptr), _width(width), _height(height) {
	WDEBUG("screen size - width:%d, height:%d", width, height);
}

SmplWatchFaceViewController::~SmplWatchFaceViewController() {
}

void SmplWatchFaceViewController::update(int hour, int min, int sec) {
	WHIT();

	WDEBUG("watchface update - %2d:%2d:%2d", hour, min, sec);

	char text[256];
	snprintf(text, 256, "<align=center>Hello Watch<br/>%02d:%02d:%02d</align>", hour, min, sec);

	elm_object_text_set(_label, text);
}

Evas_Object* SmplWatchFaceViewController::onCreateView(Evas_Object* parent, void* param) {
	WHIT();

	Evas_Object* layout = elm_layout_add(parent);
	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(layout);

	_label = elm_label_add(layout);
	evas_object_resize(_label, _width, _height / 3);
	evas_object_move(_label, 0, _height / 3);
	evas_object_show(_label);

	return layout;
}
