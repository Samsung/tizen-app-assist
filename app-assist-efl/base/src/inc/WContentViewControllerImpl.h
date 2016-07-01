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

#ifndef _APP_ASSIST_WCONTENT_VIEW_CONTROLLER_IMPL_H_
#define _APP_ASSIST_WCONTENT_VIEW_CONTROLLER_IMPL_H_

#include <functional>
#include <Elementary.h>

namespace app_assist {

class WContentViewController;
class WNaviframeController;

class _WContentViewControllerImpl {
public:
	_WContentViewControllerImpl();
	~_WContentViewControllerImpl();

public:
	bool callPushedHandlerFunc(Elm_Object_Item* naviItem);

private:
	char* _title;
	Elm_Object_Item* _naviItem;
	bool _isViewNaviItem;

	WNaviframeController* _naviframe;
	std::function<void (Elm_Object_Item*)> _pushedDelegate;

	static void setContainerProperties(WContentViewController* view, WNaviframeController* naviframe);
	static void setContainerNaviitemProperty(WContentViewController* view, Elm_Object_Item* item, bool isViewNaviItem);
	static void propagateContainerPropertiesToChildViews(WContentViewController* view);

	friend class WContentViewController;
	friend void wcontentviewcontroller_set_container_properties(WContentViewController* view, WNaviframeController* naviframe);
	friend void wcontentviewcontroller_set_container_naviitem_property(WContentViewController* view, Elm_Object_Item* item, bool isViewNaviItem);
	friend void wcontentviewcontroller_propagate_container_properties_to_childviews(WContentViewController* view);
};

} // namespace

#endif /* _APP_ASSIST_WCONTENT_VIEW_CONTROLLER_IMPL_H_ */
