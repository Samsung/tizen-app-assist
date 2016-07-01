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

#ifndef _APP_ASSIST_WPAGE_SLIDE_VIEW_CONTROLLER_H_
#define _APP_ASSIST_WPAGE_SLIDE_VIEW_CONTROLLER_H_

#include <Elementary.h>
#include <functional>
#include "WContentViewController.h"

namespace app_assist {

class IWWindowController;
class WNaviframeController;

/**
* @class	WPageSlideViewController
* @brief	This class provides slide view with pages
*
*
* The %WPageSlideViewController is the class to provide slide page view.
*/
class WAPP_ASSIST_EXPORT WPageSlideViewController : public WContentViewController {
public:
	/**
	 * This is the default constructor for this class.
	 *
	 */
	WPageSlideViewController();

	/**
	 * Retuns the class name string
	 *
	 * @return class name
	 */
	virtual const char* getClassName() const; //override;

public:
	/**
	 * Attaches WContentViewController ui object as a page
	 *
	 * @param[in]	view	WContentViewController object to be attached.
	 *
	 * @return true if succeeded.
	 * @see WContentViewController::onSetAsPageContent()
	 */
	bool appendPage(WContentViewController* content);

	/**
	 * Attaches WViewController ui object as a page.
	 *
	 * @param[in]	control	WViewController instance to be attached.
	 *
	 * @return true if succeeded.
	 */
	bool appendPage(WViewController* content);

	/**
	 * Attaches empty ui object as a page
	 *
	 * @return true if succeeded.
	 */
	bool appendPage();

	/**
	 * Remove the page and delete its content.
	 *
	 * @param[in]	index	the index of the page.If it is out of range, it will fail.
	 *
	 * @return true if succeeded.
	 */
	bool removePage(int index);

	/**
	 * Resizes the page slide layout.
	 * Call this API when you append or remove pages by some events after creation is finished.
	 * If it is not called, the screen will not be updated.
	 * At the view creation time, you don't need to call this API.
	 *
	 * @see appendPage(), removePage()
	 */
	void resizePageSlideLayout();

	/**
	 * Sets the page WContentViewController. If the previous page WContentViewController is exist, it returns false.
	 *
	 * @param[in]	index	the index of the page
	 * @param[in]	view	WContentViewController object
	 */
	bool setPageContent(int index, WContentViewController* view);

	/**
	 * Sets the page WViewController. If the previous page WViewController is exist, it returns false.
	 *
	 * @param[in]	index	the index of the page
	 * @param[in]	control	WViewController object
	 */
	bool setPageContent(int index, WViewController* control);

	/**
	 * Returns the number of page.
	 *
	 * @return the number of page
	 */
	int getPageCount() const;

	/**
	 * Gets the page instance.
	 *
	 * @param[in]	index	index of the page
	 *
	 * @return UI object
	 */
	WViewController* getPageContent(int index) const;

	/**
	 * Gets the current page index.
	 *
	 * @return The current page index
	 */
	int getCurrentPageIndex() const;

	/**
	 * Gets the scroller evas object.
	 *
	 *
	 * @return The pointer to the evas object of the scroller
	 */
	Evas_Object* getScroller() const;

	/**
	 * Sets the flag whether the scroller is shown or not.
	 *
	 * @param[in]	show	true if it should be shown.
	 */
	void setScrollerShowState(bool show);

	/**
	 * Changes to the page
	 *
	 * @param[in]	index			the index of the page
	 * @param[in]	withAnimation	flag if animation is shown or not.
	 */
	void showPage(int index, bool withAnimation);

	/**
	 * Changes to the page. The animation effect is off.
	 *
	 * @param[in]	index			the index of the page
	 */
	void showPage(int index);

public:
	/**
	 * Sets a delegate function. It delegates onInitScroller handling to delegateFunc.  it is called when scroller is created.
	 * Use this to set additional property to the scroller.
	 *
	 * @param[in]	delegateFunc	The pointer to delegate function
	 *
	 * @see onInitScroller()
	 */
	void setOnInitScroller(std::function<void (Evas_Object* scroller)> delegateFunc);

	/**
	 * Sets 'scroll animation stop' event handling function.  it is called when scroll animation completed.
	 *
	 * The dir parameter to the handlerFunc tells in which direction the pages moved.
	 * If dir > 0, the page right to the previously shown page is now showing. Page slid from right to left.
	 * if dir < 0, the page left to the previously shown page is now showing. Page slid from left to right.
	 *
	 * @param[in]	handlerFunc	The pointer to handler function
	 */
	void setOnScrollAnimStop(std::function<void(int dir)> handlerFunc);

protected:
	/**
	 * Unsets the page content. The page content is not deleted.
	 * It should be used with setPageContent method to set the page again.
	 *
	 * @param[in]	index	the index of the page
	 *
	 * @see setPageObject
	 */
	WViewController* unsetPageContent(int index);

protected:
	/**
	 * Called when the view is created. It comes from WContentViewController class.
	 *
	 * @param[in]	parent	parent evas object
	 * @param[in]	viewParam	user parameter
	 */
	virtual Evas_Object* onCreateView(Evas_Object* parent, void* viewParam);   //override;

	/**
	 * Called when the container properties are set by Naviframe or other container views.
	 * Sets container properties for any existing page views,
	 * so that those page views can access window and naviframe.
	 *
	 * @param[in]	naviframe	WNaviframe instance
	 * @param[in]	item	navi item pointer of container view which is pushed into naviframe.
	*/
	virtual void onSetContainerProperties(WNaviframeController* naviframe, Elm_Object_Item* item);  //override;

	/**
	 * Called when the layout is created. It comes from WContentViewController class.
	 *
	 * @param[in]	parent	parent evas object
	 *
	 * @return The evas object of layout to be created.
	 */
	virtual Evas_Object* onCreateLayout(Evas_Object* parent);

	/**
	 * Called when the scroller is created.
	 * Override this to set additional property to the scroller.
	 *
	 * @param[in]	scroller	scroller evas object
	 *
	 * @see setOnInitScroller()
	 */
	virtual void onInitScroller(Evas_Object* scroller);

protected:
	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WPageSlideViewController();

private:
	WDECLARE_PRIVATE_IMPL(WPageSlideViewController);
	WDISABLE_COPY_AND_ASSIGN(WPageSlideViewController);
};

} // namespace

#endif /* _APP_ASSIST_WPAGE_SLIDE_VIEW_CONTROLLER_H_ */
