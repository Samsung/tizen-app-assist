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

#include "WNaviframeController.h"
#include <memory.h>
#include <efl_extension.h>
#include "WDebugInternal.h"
#include "IWWindowController.h"
#include "WDefineInternal.h"
#include "WContentViewController.h"
#include "WContentViewControllerImpl.h"

using namespace app_assist;

#define _CLASS_NAME	"WNaviframeController"

namespace app_assist {

class _WNaviframeControllerImpl {
public:
	_WNaviframeControllerImpl() {};
	~_WNaviframeControllerImpl() {};

public:
	std::function<void(bool*)> _lastItemPopDelegate;
	static Eina_Bool _popCb(void* data, Elm_Object_Item* item);
};
}

Eina_Bool _WNaviframeControllerImpl::_popCb(void* data, Elm_Object_Item* item) {
	auto vc = (WContentViewController*)data;

	Evas_Object* naviObj = elm_object_item_widget_get(item);
	auto nf = (WNaviframeController*)evas_object_data_get(naviObj, WKEY_FRAME_INSTANCE);

	if (nf->onItemPop(vc, item))
		return EINA_TRUE;
	else
		return EINA_FALSE;
}

WNaviframeController::WNaviframeController()
	: _pv(new _WNaviframeControllerImpl()) {
	WHIT();
}

WNaviframeController::~WNaviframeController() {
	WHIT();
	delete _pv;
}

const char* WNaviframeController::getClassName() const {
	return _CLASS_NAME;
}

bool WNaviframeController::create(Evas_Object* parent, void* param) {
	if (WViewController::create(parent, param)) {
		evas_object_data_set(getEvasObject(), WKEY_FRAME_INSTANCE, this);
		return true;
	}
	return false;
}

void WNaviframeController::destroy() {
	WHIT();
	if (getEvasObject()) {
		destroyAllViewControllers();
		evas_object_del(getEvasObject());
		// Do not leave any code here.
		// After executing upper statement "evas_object_del", this object will be deleted at evas object deletion callback!
	}
	else {
		onDestroy();
		delete this;
	}
}

bool WNaviframeController::push(WContentViewController* vc, const char* viewName, void* viewCreationParam) {
	if (viewName)
		vc->setName(viewName);

	return onPush(vc, viewCreationParam);
}

bool WNaviframeController::push(WContentViewController* vc, const char* viewName) {
	return push(vc, viewName, nullptr);
}

bool WNaviframeController::push(WContentViewController* vc) {
	return push(vc, nullptr, nullptr);
}


void WNaviframeController::destroyAllViewControllers() {
	WHIT();

	Eina_List* list = elm_naviframe_items_get(getEvasObject());
	Eina_List* temp = nullptr;
	void* it = nullptr;
	EINA_LIST_FOREACH(list, temp, it) {
		elm_object_item_del((Elm_Object_Item*)it);
	}
}

bool WNaviframeController::isTopContentViewController(WContentViewController* vc) const {
	if (getEvasObject() == nullptr) return false;

	Elm_Object_Item* item = elm_naviframe_top_item_get(getEvasObject());
	if (item && vc->getNaviItem() == item)
		return true;
	else
		return false;
}

WContentViewController* WNaviframeController::getTopContentViewController() const {
	if (getEvasObject() == nullptr) return nullptr;

	Elm_Object_Item* item = elm_naviframe_top_item_get(getEvasObject());
	return (WContentViewController*)elm_object_item_data_get(item);
}

void WNaviframeController::setOnLastItemPop(std::function<void(bool*)> delegateFunc) {
	_pv->_lastItemPopDelegate = delegateFunc;
}



Evas_Object* WNaviframeController::onCreateView(Evas_Object* parent, void* param) {
	Evas_Object* obj = elm_naviframe_add(parent);
	eext_object_event_callback_add(obj, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, nullptr);
	eext_object_event_callback_add(obj, EEXT_CALLBACK_MORE, eext_naviframe_more_cb, nullptr);

	evas_object_show(obj);

	return obj;
}

void WNaviframeController::onDestroy() {

}

void WNaviframeController::onLastItemPop(bool* popOut) {
	WHIT();
	*popOut = false;
	elm_win_lower(getWindowController()->getEvasObject());
}

bool WNaviframeController::onPush(WContentViewController* vc, void* viewParam) {
	WASSERT(getEvasObject() != nullptr);

	if (vc->create(getEvasObject(), viewParam) == false) {
		WERROR("Failed to create a vc!");
		return false;
	}

	wcontentviewcontroller_set_container_properties(vc, this);

	Elm_Object_Item* item = vc->onPushToNaviframe(getEvasObject());
	if (item == nullptr)
		item = elm_naviframe_item_push(getEvasObject(), vc->getTitle(), nullptr, nullptr, vc->getEvasObject(), nullptr);

	elm_object_item_data_set(item, vc);
	wcontentviewcontroller_set_container_naviitem_property(vc, item, true);
	wcontentviewcontroller_propagate_container_properties_to_childviews(vc);

	if (vc->_pv->callPushedHandlerFunc(item) == false)
		vc->onPushed(item);

	elm_naviframe_item_pop_cb_set(item, _WNaviframeControllerImpl::_popCb, vc);

	return true;
}

bool WNaviframeController::onItemPop(WContentViewController* vc, Elm_Object_Item* item) {
	WNaviframeController* nf = vc->getNaviframeController();

	if (item == elm_naviframe_bottom_item_get(nf->getEvasObject())) {
		if (!vc->onPop())
			return false;

		bool popOut = true;

		if (nf->_pv->_lastItemPopDelegate)
			nf->_pv->_lastItemPopDelegate(&popOut);
		else
			nf->onLastItemPop(&popOut);
		return popOut;
	}
	else {
		if (vc->onPop()) {
			Eina_List* itemList = elm_naviframe_items_get(getEvasObject());

			int nth = eina_list_count(itemList) - 2;
			if (nth >= 0) {
				Elm_Object_Item* tempItem = (Elm_Object_Item*)eina_list_nth(itemList, nth);
				WContentViewController* targetView = (WContentViewController*)elm_object_item_data_get(tempItem);
				WASSERT(targetView);
				targetView->onBecomeTop();
			}
			eina_list_free(itemList);

			return true;
		}
		else {
			return false;
		}
	}
}

