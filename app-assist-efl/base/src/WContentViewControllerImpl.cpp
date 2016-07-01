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

#include "WContentViewControllerImpl.h"
#include "WContentViewController.h"

using namespace app_assist;

_WContentViewControllerImpl::_WContentViewControllerImpl()
	: _title(nullptr), _naviItem(nullptr), _isViewNaviItem(true), _naviframe(nullptr) {
}

_WContentViewControllerImpl::~_WContentViewControllerImpl() {
	free(_title);
}

bool _WContentViewControllerImpl::callPushedHandlerFunc(Elm_Object_Item* naviItem) {
	if (_pushedDelegate) {
		_pushedDelegate(naviItem);
		return true;
	}
	return false;
}

void _WContentViewControllerImpl::setContainerProperties(WContentViewController* view, WNaviframeController* naviframe) {
	view->_pv->_naviframe = naviframe;
}

void _WContentViewControllerImpl::setContainerNaviitemProperty(WContentViewController* view, Elm_Object_Item* item, bool isViewNaviItem) {
	view->_pv->_isViewNaviItem = isViewNaviItem;
	view->_pv->_naviItem = item;
}

void _WContentViewControllerImpl::propagateContainerPropertiesToChildViews(WContentViewController* view) {
	view->onSetContainerProperties(view->getNaviframeController(), view->getNaviItem());
}


