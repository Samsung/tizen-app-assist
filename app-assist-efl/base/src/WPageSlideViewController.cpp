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

#include "WPageSlideViewController.h"
#include <list>
#include <string>
#include <app.h>
#include "WDebugInternal.h"
#include "WDefineInternal.h"
#include "WContentViewController.h"
#include "WTimer.h"

using namespace app_assist;

#define _CLASS_NAME	"WPageSlideViewController"

namespace app_assist {

class _WPageSlideViewControllerImpl {
public:
	_WPageSlideViewControllerImpl(WPageSlideViewController* host);
	_WPageSlideViewControllerImpl() = delete;
	~_WPageSlideViewControllerImpl();

public:
	WPageSlideViewController* _b;

	struct PageContent {
		WViewController* uiObj;
		Evas_Object* evasObj;
		bool isContentView;
	};

	Evas_Object* _scroller;
	Evas_Object* _box;
	int _currentPage;
	int _previousPage;
	int _pageWidth;
	int _pageHeight;
	std::list<PageContent>* _listPageContent;
	bool _showScroll;

	std::function<void (Evas_Object*)> _initScrollerHandler;
	std::function<void(int dir)> _scrollAnimStopCb;

	bool _appendPage(WViewController* uiObj, bool isContentView);
	bool _appendPage();
	bool _setPageContent(int index, WViewController* uiObj, bool isContentView);

	static void _scrollCallback(void* data, Evas_Object* obj, void* event_info);
	static void _scrollAnimStartCallback(void* data, Evas_Object* obj, void* event_info);
	static void _scrollAnimStopCallback(void* data, Evas_Object* obj, void* event_info);
	static Eina_Bool _onTimer(void* data);

	static void _layoutResizeCallback(void* data, Evas* e, Evas_Object* obj, void* event_info);
};
}

static Evas_Object* _createPageBase(Evas_Object* parent) {
	Evas_Object* pageBase = elm_layout_add(parent);
	evas_object_size_hint_weight_set(pageBase, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	char* path = app_get_resource_path();
	if (path == nullptr) {
		WERROR("Failed to get resource path=nullptr!!!");
		return pageBase;
	}
	std::string edjPath(path);
	free(path);
	edjPath += "edje/app-assist-efl.edj";

	if (elm_layout_file_set(pageBase, edjPath.c_str(), "page") == EINA_FALSE) {
		WERROR("Fail to set layout. Check EDJ file(%s)", edjPath.c_str());
		return pageBase;
	}

	Evas_Object* rect = evas_object_rectangle_add(evas_object_evas_get(pageBase));
	evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_color_set(rect, 0, 0, 0, 0);  // make it transparent to show bg
	evas_object_show(rect);

	elm_object_part_content_set(pageBase, "sizer", rect);

	evas_object_show(pageBase);
	return pageBase;
}

_WPageSlideViewControllerImpl::_WPageSlideViewControllerImpl(WPageSlideViewController* host)
	: _b(nullptr),
	_scroller(nullptr),
	_box(nullptr),
	_currentPage(0),
	_previousPage(0),
	_pageWidth(0),
	_pageHeight(0),
	_listPageContent(nullptr),
	_showScroll(true) {
	_b = host;
	_listPageContent = new std::list<PageContent>;
}

_WPageSlideViewControllerImpl::~_WPageSlideViewControllerImpl() {
	delete _listPageContent;
}

bool _WPageSlideViewControllerImpl::_appendPage(WViewController* uiObj, bool isContentView) {
	if (_scroller == nullptr) {
		WASSERT("First, create page slide view!");
		return false;
	}

	Evas_Object* pageBase = _createPageBase(_box);

	uiObj->create(pageBase, nullptr);

	elm_object_part_content_set(pageBase, "sw", uiObj->getEvasObject());

	elm_box_pack_end(_box, pageBase);

	_listPageContent->push_back(PageContent{uiObj, pageBase, isContentView});

	return true;
}

bool _WPageSlideViewControllerImpl::_appendPage() {
	if (_scroller == nullptr) {
		WASSERT("First, create page slide view!");
		return false;
	}

	Evas_Object* pageBase = _createPageBase(_box);

	elm_box_pack_end(_box, pageBase);

	_listPageContent->push_back(PageContent{nullptr, pageBase, false});

	return true;
}

bool _WPageSlideViewControllerImpl::_setPageContent(int index, WViewController* uiObj, bool isContentView) {
	int count = 0;
	Evas_Object* pageBase = nullptr;
	for (PageContent& it : *_listPageContent) {
		if (index == count) {
			if (it.uiObj) {
				WERROR("Index is already occupied(index=%d)", index);
				return false;

			}
			it.uiObj = uiObj;
			it.isContentView = isContentView;
			pageBase = it.evasObj;
			break;
		}
		count++;
	}

	if (count != index) {
		WERROR("Index is not in range(index=%d,count=%d)", index, count);
		return false;
	}

	Evas_Object* content = elm_object_part_content_get(pageBase, "sw");
	if (content == nullptr) {
		uiObj->create(pageBase, nullptr);
		elm_object_part_content_set(pageBase, "sw", uiObj->getEvasObject());
		return true;
	}

	return false;
}


void _WPageSlideViewControllerImpl::_scrollCallback(void* data, Evas_Object* obj, void* event_info) {
	_WPageSlideViewControllerImpl* pageViewImpl = ((WPageSlideViewController*)data)->_pv;
	int x;

	evas_object_geometry_get(pageViewImpl->_box, &x, nullptr, nullptr, nullptr);

	if (x % pageViewImpl->_pageWidth == 0) {
		int page;
		elm_scroller_current_page_get(obj, &page, nullptr);
		pageViewImpl->_currentPage = page;
	}
}

void _WPageSlideViewControllerImpl::_scrollAnimStartCallback(void* data, Evas_Object* obj, void* event_info) {
	_WPageSlideViewControllerImpl* pageViewImpl = ((WPageSlideViewController*)data)->_pv;
	pageViewImpl->_previousPage = pageViewImpl->_currentPage;
}

void _WPageSlideViewControllerImpl::_scrollAnimStopCallback(void* data, Evas_Object* obj, void* event_info) {
	_WPageSlideViewControllerImpl* pageViewImpl = ((WPageSlideViewController*)data)->_pv;
	if (pageViewImpl->_scrollAnimStopCb)
		pageViewImpl->_scrollAnimStopCb(pageViewImpl->_currentPage - pageViewImpl->_previousPage);
}

Eina_Bool _WPageSlideViewControllerImpl::_onTimer(void* data) {
	_WPageSlideViewControllerImpl* pageViewImpl = ((WPageSlideViewController*)data)->_pv;
	_layoutResizeCallback(data, nullptr, pageViewImpl->_scroller, nullptr);
	return ECORE_CALLBACK_CANCEL;
}


void _WPageSlideViewControllerImpl::_layoutResizeCallback(void* data, Evas* e, Evas_Object* obj, void* event_info) {

	WHIT();

	WPageSlideViewController* pageView = (WPageSlideViewController*)data;
	_WPageSlideViewControllerImpl* pv = pageView->_pv;
	int w, h;

	evas_object_geometry_get(obj, nullptr, nullptr, &w, &h);

	pv->_pageWidth = w;
	pv->_pageHeight = h;

	for (PageContent& it : *pv->_listPageContent) {
		Evas_Object* pageBase = it.evasObj;
		evas_object_size_hint_min_set(elm_object_part_content_get(pageBase, "sizer"), w, h);
	}

	elm_scroller_page_size_set(pv->_scroller, w, h);
	elm_scroller_page_show(pv->_scroller, pv->_currentPage, 0);
}


WPageSlideViewController::WPageSlideViewController() {
	_pv = new _WPageSlideViewControllerImpl(this);
}

WPageSlideViewController::~WPageSlideViewController() {
	delete _pv;
}

void WPageSlideViewController::setOnScrollAnimStop(std::function<void(int dir)> handlerFunc) {
	_pv->_scrollAnimStopCb = handlerFunc;
}

const char* WPageSlideViewController::getClassName() const {
	return _CLASS_NAME;
}

bool WPageSlideViewController::appendPage(WViewController* content) {
	if (_pv->_appendPage(content, false) == false) {
		WERROR("Fail to attach the page!");
		return false;
	}

	return true;
}

bool WPageSlideViewController::appendPage(WContentViewController* content) {
	if (_pv->_appendPage(content, true) == false) {
		WERROR("Fail to attach the page!");
		return false;
	}

	wcontentviewcontroller_set_container_properties(content, getNaviframeController());
	wcontentviewcontroller_set_container_naviitem_property(content, getNaviItem(), false);
	wcontentviewcontroller_propagate_container_properties_to_childviews(content);

	content->onSetAsPageContent(getNaviItem());

	return true;
}

bool WPageSlideViewController::appendPage() {
	if (_pv->_appendPage() == false) {
		WERROR("Fail to attach the page!");
		return false;
	}

	return true;
}

bool WPageSlideViewController::removePage(int index) {
	int pageNum = getPageCount();
	if (index < 0 || index >= pageNum) {
		WERROR("Out of page number(%d), index=%d", pageNum, index);
		return false;
	}

	int cur = 0;
	for (auto it = _pv->_listPageContent->begin(); it != _pv->_listPageContent->end(); it++) {
		if (cur == index) {
			elm_box_unpack(_pv->_box, it->evasObj);
			evas_object_del(it->evasObj);
			_pv->_listPageContent->erase(it);
			return true;
		}
		cur++;
	}
	return false;
}

void WPageSlideViewController::resizePageSlideLayout() {
	_WPageSlideViewControllerImpl::_layoutResizeCallback(this, nullptr, _pv->_scroller, nullptr);
}

bool WPageSlideViewController::setPageContent(int index, WContentViewController* content) {
	if (_pv->_setPageContent(index, content, true) == false) {
		WERROR("Fail to set page content %d!", index);
		return false;
	}

	wcontentviewcontroller_set_container_properties(content, getNaviframeController());
	wcontentviewcontroller_set_container_naviitem_property(content, getNaviItem(), false);
	wcontentviewcontroller_propagate_container_properties_to_childviews(content);

	content->onSetAsPageContent(getNaviItem());

	return true;
}

bool WPageSlideViewController::setPageContent(int index, WViewController* content) {
	if (_pv->_setPageContent(index, content, false) == false) {
		WERROR("Fail to set page content %d!", index);
		return false;
	}
	return true;
}


int WPageSlideViewController::getPageCount() const {
	return _pv->_listPageContent->size();
}

WViewController* WPageSlideViewController::getPageContent(int index) const {
	int count = 0;
	for (_WPageSlideViewControllerImpl::PageContent& it : *_pv->_listPageContent) {
		if (count == index)
			return it.uiObj;
		count++;
	}

	return nullptr;
}

int WPageSlideViewController::getCurrentPageIndex() const {
	return _pv->_currentPage;
}

Evas_Object* WPageSlideViewController::getScroller() const {
	return _pv->_scroller;
}

void WPageSlideViewController::setScrollerShowState(bool show) {
	_pv->_showScroll = show;
}

void WPageSlideViewController::showPage(int index, bool withAnimation) {
	int pageNum = getPageCount();
	if (index < 0 || index >= pageNum) {
		WERROR("Out of page number(%d), index=%d", pageNum, index);
		return;
	}
	_pv->_currentPage = index;
	if (withAnimation)
		elm_scroller_page_bring_in(_pv->_scroller, _pv->_currentPage, 0);
	else
		elm_scroller_page_show(_pv->_scroller, _pv->_currentPage, 0);
}

void WPageSlideViewController::showPage(int index) {
	return showPage(index, false);
}

WViewController* WPageSlideViewController::unsetPageContent(int index) {
	int pageNum = getPageCount();
	if (index < 0 || index >= pageNum) {
		WERROR("Out of page number(%d), index=%d", pageNum, index);
		return nullptr;
	}

	auto it = _pv->_listPageContent->begin();
	for (int i = 0; i < index; i++)
		it++;

	Evas_Object* obj = elm_object_part_content_unset((*it).evasObj, "sw");
	(*it).uiObj = nullptr;
	if (obj == nullptr) {
		WERROR("elm_object_part_content_unset obj is nullptr");
		return nullptr;
	}
	else {
		WViewController* uiObj = nullptr;
		if ((uiObj = (WViewController*)evas_object_data_get(obj, WKEY_VIEW_INSTANCE)) != nullptr) {
			return uiObj;
		}
		return nullptr;
	}
}

void WPageSlideViewController::setOnInitScroller(std::function<void (Evas_Object*)> delegateFunc) {
	_pv->_initScrollerHandler = delegateFunc;
}

Evas_Object* WPageSlideViewController::onCreateView(Evas_Object* parent, void* viewParam) {
	WHIT();

	Evas_Object* layout = onCreateLayout(parent);
	Evas_Object* scroller = nullptr;
	if (layout) {
		scroller = elm_scroller_add(layout);
		evas_object_event_callback_add(layout, EVAS_CALLBACK_RESIZE, _WPageSlideViewControllerImpl::_layoutResizeCallback, this);
	}
	else {
		scroller = elm_scroller_add(parent);
		evas_object_event_callback_add(scroller, EVAS_CALLBACK_RESIZE, _WPageSlideViewControllerImpl::_layoutResizeCallback, this);
	}

	evas_object_smart_callback_add(scroller, "scroll", _WPageSlideViewControllerImpl::_scrollCallback, this);
	evas_object_smart_callback_add(scroller, "scroll,anim,start", _WPageSlideViewControllerImpl::_scrollAnimStartCallback, this);
	evas_object_smart_callback_add(scroller, "scroll,anim,stop", _WPageSlideViewControllerImpl::_scrollAnimStopCallback, this);

	elm_scroller_loop_set(scroller, EINA_FALSE, EINA_FALSE);
	evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_scroller_page_relative_set(scroller, 1.0, 0.0);
	elm_scroller_policy_set(scroller,
							_pv->_showScroll ? ELM_SCROLLER_POLICY_AUTO : ELM_SCROLLER_POLICY_OFF,
							ELM_SCROLLER_POLICY_OFF);
	elm_scroller_page_scroll_limit_set(scroller, 1, 0);
	elm_object_scroll_lock_y_set(scroller, EINA_TRUE);
	elm_scroller_single_direction_set(scroller, ELM_SCROLLER_SINGLE_DIRECTION_HARD);

	if (_pv->_initScrollerHandler)
		_pv->_initScrollerHandler(scroller);
	else
		onInitScroller(scroller);

	if (layout) {
		elm_object_part_content_set(layout, "scroller", scroller);
	}

	Evas_Object* box = elm_box_add(scroller);

	_pv->_scroller = scroller;
	_pv->_box = box;

	elm_box_horizontal_set(box, EINA_TRUE);
	elm_object_content_set(scroller, box);
	evas_object_show(scroller);
	evas_object_show(box);


	if (layout) {
		WTimer::addTimer(0.0, _WPageSlideViewControllerImpl::_onTimer, this, layout);
		return layout;
	}
	else {
		WTimer::addTimer(0.0, _WPageSlideViewControllerImpl::_onTimer, this, scroller);
		return scroller;
	}
}

void WPageSlideViewController::onSetContainerProperties(WNaviframeController* naviframe, Elm_Object_Item* item) {
	for (_WPageSlideViewControllerImpl::PageContent& it : *_pv->_listPageContent) {
		if (it.uiObj && it.isContentView) {
			auto view = (WContentViewController*)wviewcontroller_get_instance_from_evasobj(it.uiObj->getEvasObject());
			wcontentviewcontroller_set_container_properties(view , naviframe);
			wcontentviewcontroller_set_container_naviitem_property(view, item, false);
			wcontentviewcontroller_propagate_container_properties_to_childviews(view);
		}
	}
}

void WPageSlideViewController::onInitScroller(Evas_Object* scroller) {
}

Evas_Object* WPageSlideViewController::onCreateLayout(Evas_Object* parent) {
	WHIT();
	return nullptr;
}


