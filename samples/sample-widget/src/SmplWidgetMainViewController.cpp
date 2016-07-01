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

#include "SmplWidgetMainViewController.h"

#include <Elementary.h>
#include <app.h>
#include "SmplWidgetDebug.h"
#include "IWWindowController.h"

#define EDJ_FILE "edje/sample-widget.edj"
#define GRP_MAIN "main"


using namespace app_assist;

SmplWidgetMainViewController::SmplWidgetMainViewController() {
}

SmplWidgetMainViewController::~SmplWidgetMainViewController() {
}

Evas_Object* SmplWidgetMainViewController::onCreateView(Evas_Object* parent, void* viewParam) {
	WHIT();

	char edjPath[PATH_MAX] = {0,};
	char* resPath = app_get_resource_path();
	if (resPath) {
		snprintf(edjPath, sizeof(edjPath), "%s%s", resPath, EDJ_FILE);
		free(resPath);
	}

	Evas_Object* layout = elm_layout_add(parent);
	elm_layout_file_set(layout, edjPath, GRP_MAIN);
	elm_object_part_text_set(layout, "txt_title", "Hello World!");
	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(layout);

	return layout;
}

void SmplWidgetMainViewController::onDestroy() {
	WHIT();
}




