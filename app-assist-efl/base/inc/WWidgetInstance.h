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

#ifndef _APP_ASSIST_WWIDGET_INSTANCE_H_
#define _APP_ASSIST_WWIDGET_INSTANCE_H_

#include <widget_app.h>
#include "WDefine.h"
#include "WWidgetWindowController.h"

namespace app_assist {

class WWidget;
/**
 * @class	WWidgetInstance
 * @brief	This class is the base class of a widget instance
 *
 * The %WWidgetInstance class is the base class of a widget instance. A widget instance class must be inherited from this class
 */
class WAPP_ASSIST_EXPORT WWidgetInstance {
public:
	/**
	 * This is the default constructor for this class.
	 */
	WWidgetInstance();

	/**
	 * This is the destructor for this class.
	 */
	virtual ~WWidgetInstance();

protected:
	/**
	 * Called when the widget instance created.
	 * At least, you have to override this method to create an widget instance.
	 *
	 * @param[in]	context	The context of widget instance
	 * @param[in]	content	The data set for the previous status
	 * @param[in]	width	The pixel value for widget width
	 * @param[in]	height	The pixel value for widget height
	 *
	 * @return	true if it is succeeded, else false
	 */
	virtual bool onCreate(widget_context_h context, bundle* content, int width, int height) = 0;

	/**
	 * Called when the widget instance is destroyed.
	 *
	 * @param[in]	context	The context of widget instance
	 * @param[in]	reason	The reason for destruction
	 * @param[in,out]	content	The data set to save
	 *
	 * @return	true if it is succeeded, else false
	 */
	virtual bool onDestroy(widget_context_h context, widget_app_destroy_type_e reason, bundle* content) { return true; };

	/**
	 * Called when the widget instance is paused.
	 *
	 * @param[in]	context	The context of widget instance
	 *
	 * @return	true if it is succeeded, else false
	 */
	virtual bool onPause(widget_context_h context) { return true; };

	/**
	 * Called when the widget instance is resumed.
	 *
	 * @param[in]	context	The context of widget instance
	 *
	 * @return	true if it is succeeded, else false
	 */
	virtual bool onResume(widget_context_h context) { return true; };

	/**
	 * Called when the widget instance is updated.
	 *
	 * @param[in]	context	The context of widget instance.
	 * @param[in]	content	The data set for updating this widget. It will be provided by requester.
	 *				Requester can use widget_service_trigger_update().
	 * @param[in]	isForced	Although the widget is paused, if it is true, the widget can be updated.
	 *
	 * @return	true if it is succeeded, else false
	 */
	virtual bool onUpdate(widget_context_h context, bundle* content, bool isForced) { return true; };

	/**
	 * Called when the widget instance is resized.
	 *
	 * @param[in]	context	The context of widget instance
	 * @param[in]	width	The pixel value for widget width
	 * @param[in]	height	The pixel value for widget height
	 *
	 * @return	true if it is succeeded, else false
	 */
	virtual bool onResize(widget_context_h context, int width, int height) { return true; };

	/**
	 * Gets the window class instance of the widget.
	 *
	 * @return	The pointer to the window
	 */
	WWidgetWindowController* getWindowController() const;

	/**
	 * Gets the widget class instance.
	 *
	 * @return	The pointer to the widget instance
	 */
	WWidget* getWidget() const;

	/**
	 * Gets the width of this widget.
	 *
	 * @return	The width
	 */
	int getWidth() const;

	/**
	 * Gets the height of this widget.
	 *
	 * @return	The height
	 */
	int getHeight() const;

	/**
	 * Attaches a window to the widget.
	 *
	 * @param[in]	context	The context of widget instance
	 * @param[in]	win	A pointer to the window to attach
	 *
	 * @return	true if the window attached successfully, else false
	 */
	bool attachWindowController(widget_context_h context, WWidgetWindowController* win);

private:
	WDECLARE_PRIVATE_IMPL(WWidgetInstance);
	WDISABLE_COPY_AND_ASSIGN(WWidgetInstance);
};

} // namespace

#endif // _APP_ASSIST_WWIDGET_INSTANCE_H_
