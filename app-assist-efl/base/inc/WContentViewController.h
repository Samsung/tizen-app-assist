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

#ifndef _APP_ASSIST_WCONTENT_VIEW_CONTROLLER_H_
#define _APP_ASSIST_WCONTENT_VIEW_CONTROLLER_H_

#include <functional>
#include <Evas.h>
#include <Elementary.h>
#include "WViewController.h"
#include "WNaviframeController.h"

namespace app_assist {

/**
* @class	WContentViewController
* @brief	This class means the view represented by an evas object, which can be pushed into naviframe elementary.
*
* The %WContentViewController class keeps an evas object and provides methods to handle the case when the view is pushed into naviframe.
*/
class WAPP_ASSIST_EXPORT WContentViewController : public WViewController {
public:
	/**
	 * This is the default constructor for this class.
	 *
	 */
	WContentViewController();

	/**
	 * Constructor
	 *
	 * @param[in]	name	the instance name to be set
	 */
	explicit WContentViewController(const char* name);

	/**
	 * Gets the class name.
	 *
	 */
	virtual const char* getClassName() const; //override;

public:
	/**
	 * Creates evas object which represents this view.
	 * "onCreate" will be called subsequently.
	 *
	 * @param[in]	parent	parent evas object of the view
	 * @param[in]	viewParam	user data
	 * @see onCreate
	 * @see onCreateView
	 * @see onCreated
	 */
	bool create(Evas_Object* parent, void* param);	//final;

	/**
	 * Destroys the class instance and deletes evas object.
	 * If it has been pushed into naviframe, it will be popped out without onPop method being called.
	 *
	 * @see onDestroy
	 */
	void destroy(); //final;

public:
	/**
	 * Sets a delegate function. It delegates onPushed handling to delegateFunc.
	 * This is simpler alternative to overriding the virtual method onPushed(), allowing direct use of WContentViewController without sub-classing.
	 * If delegation is used, onPushed() will not be called.
	 *
	 * @param[in]	delegateFunc	The pointer to delegate function
	 * @see onPushed()
	 */
	void setOnPushed(std::function<void (Elm_Object_Item* naviItem)> delegateFunc);

protected:
	/**
	 * Called when create method is called.
	 * It creates a default view.
	 *
	 * @param[in]	parent	parent evas object
	 * @param[in]	viewParam	user data
	 *
	 * @return the most parent evas object created in this method. It will be related to the class instance.
	 */
	virtual Evas_Object* onCreateView(Evas_Object* parent, void* viewParam);	//override;

	/**
	 * Called when the container properties are set by Naviframe or other container views.
	 * Container view should override this method in order to set container properties for any views it contains,
	 * so that those page views can access window and naviframe.
	 *
	 * @param[in]	win		IWWindowController instance
	 * @param[in]	naviframe	WNaviframeController instance
	 * @param[in]	item	navi item pointer of container view which is pushed into naviframe.
	*/
	virtual void onSetContainerProperties(WNaviframeController* naviframe, Elm_Object_Item* item);

	/**
	 * Called when this view instance evas object should be pushed into naviframe.
	 * Override this method to do something and push the view into naviframe by itself.
	 *
	 * @param[in]	naviFrame	naviframe evas object
	 *
	 * @return navi item resulted from pushing the view to the naviframe.
	 * @see %WNaviframeController::push()
	 * @see onPushed()
	 */
	virtual Elm_Object_Item* onPushToNaviframe(Evas_Object* naviFrame);

	/**
	 * Called when this view instance evas object is pushed into naviframe.
	 * Override this method to do something with navi item.
	 *
	 * @param[in]	naviItem	navi item resulted from naviframe push
	 * @see %WNaviframeController::push()
	 * @see onPushToNaviframe()
	 */
	virtual void onPushed(Elm_Object_Item* naviItem);

	/**
	 * Called when the view evas object is popped from naviframe.
	 * Override this method to do something before it is popped out.
	 *
	 * @return false, if you want to keep the view instead of being popped out.
	 */
	virtual bool onPop();

	/**
	 * Called when the view evas object will be becoming top item on naviframe.
	 * Override this method to do something before it actually become top item.
	 * It is called sequentially after onPop method of the previous top view being popped.
	 */
	virtual void onBecomeTop();

	/**
	 * Called when this view instance evas object is attached to WPageSlideView.
	 * Override this method to do something with navi item after being attached.
	 *
	 * @param[in]	parentNaviItem	navi item of WPageSlideView object pushed into naviframe
	 * @see onPushed()
	 */
	virtual void onSetAsPageContent(Elm_Object_Item* parentNaviItem);

	/**
	 * Called when setMoreButton method is called to create option menu.
	 * Override this method to create your own option menu.
	 *
	 * @param[in]	naviItem	navi item to add more button
	 * @param[in]	clickedCb	clicked callback function pointer
	 * @param[in]	userData	clicked callback user data parameter
	 * @see onPushed()
	 */
	virtual void onSetMoreButton(Elm_Object_Item* naviItem, Evas_Smart_Cb clickedCb, void* userData);

	/**
	 * Stores the title string of the view. It is used as title string, when this view is pushed into an WNaviframeController object.
	 *
	 * @param[in]	title	string of the title to be stored
	 *
	 */
	void setTitle(const char* title);

	/**
	 * Gets the title string stored by the method setTitle.
	 *
	 * @return title string
	 * @see setTitle
	 */
	const char* getTitle() const;

	/**
	 * Gets the WNaviframeController class instance.
	 *
	 * @return the pointer to the WNaviframeController class instance. If this view instance is not pushed into naviframe, it will be nullptr.
	 */
	WNaviframeController* getNaviframeController() const;

	/**
	 * Gets a navi item.
	 *
	 * @return The pointer to navi item. If this view instance is not pushed into naviframe, it will be nullptr.
	 */
	Elm_Object_Item* getNaviItem() const;

	/**
	 * Pops out this view instance evas object from naviframe.
	 *
	 * @return true, if succeeded
	 */
	bool popOut();

	/**
	 * Create more button on navi item.
	 *
	 * @param[in]	naviItem
	 * @param[in]	clickedCb
	 * @param[in]	userData
	 * @see %WNaviframeController::push()
	 * @see onPushed()
	 */
	bool setMoreButton(Elm_Object_Item* naviItem, Evas_Smart_Cb clickedCb, void* userData);

protected:
	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WContentViewController();

private:
	WDECLARE_PRIVATE_IMPL(WContentViewController);
	WDISABLE_COPY_AND_ASSIGN(WContentViewController);
	friend class WNaviframeController;
	friend class WPageSlideViewController;
};

/**
 * Sets the container class properties to the view.
 * It will be used only to create your own container view class and set its properties to child views, like WPageSlideView.
 *
 * @param[in]	view	WContentViewController instance to set these properties
 * @param[in]	naviframe	WNaviframeController instance
 *
 * @see wcontentviewcontroller_set_container_naviitem_property
 * @see wcontentviewcontroller_propagate_container_properties_to_childviews
 */
WAPP_ASSIST_EXPORT void wcontentviewcontroller_set_container_properties(WContentViewController* view, WNaviframeController* naviframe);

/**
 * Sets the container class navi item property to the view.
 * It will be used only to create your own container view class and set its properties to child views, like WPageSlideView.
 *
 * @param[in]	view	WContentViewController instance to set these properties
 * @param[in]	item	navi item pointer of container view which is pushed into naviframe.
 * @param[in]	isViewNaviItem	If the Elm_Object_Item is for the view itself, set it true.
 *                              Otherwise if it is a container's Elm_Object_Item, set it false.
 *
 * @see wcontentviewcontroller_set_container_properties
 * @see wcontentviewcontroller_propagate_container_properties_to_childviews
 */
WAPP_ASSIST_EXPORT void wcontentviewcontroller_set_container_naviitem_property(WContentViewController* view, Elm_Object_Item* item, bool isViewNaviItem);


/**
 * Propagates container properties (including naviItem property) set on the view to its child views.
 *
 * Whenever wcontentviewcontroller_set_container_properties and/or wcontentviewcontroller_set_container_naviitem_property is called for a view,
 * this function should be called afterwards, so that in case the view is a custom container view,
 * it will get a chance to propagate the properties it just received to its child views.
 *
 * This function invokes virtual method onSetContainerProperties() of the view,
 * implementation of which should, in turn, set properties for each child views.
 *
 * @param[in]	view	WContentViewController instance whose child views to set these properties
 *
 * @see wcontentviewcontroller_set_container_properties
 * @see wcontentviewcontroller_set_container_naviitem_property
 * @see onSetContainerProperties
 */
WAPP_ASSIST_EXPORT void wcontentviewcontroller_propagate_container_properties_to_childviews(WContentViewController* view);


} // namespace


#endif /* _APP_ASSIST_WCONTENT_VIEW_CONTROLLER_H_ */
