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

#ifndef _APP_ASSIST_WPOPUP_CONTROLLER_H_
#define _APP_ASSIST_WPOPUP_CONTROLLER_H_

#include "WViewController.h"
#include <functional>

namespace app_assist {
/**
* @class	WPopupController
* @brief	This class is the pop-up class to make easy to create pop-up.
*
*
* The %WPopupController is the general pop-up class.
*/
class WAPP_ASSIST_EXPORT WPopupController : public WViewController {
public:
	/**
	 * This is the default constructor for this class.
	 *
	 */
	WPopupController();

	/**
	 * This is the constructor with title and content string.
	 *
	 */
	WPopupController(const char* title, const char* textContent);

public:
	/**
	 * Sets the title string of the pop-up.
	 *
	 * @param[in]	title	title text string
	 *
	 * @see setContent()
	 */
	void setTitle(const char* title);

	/**
	 * Sets the content string of the pop-up.
	 *
	 * @param[in]	text	content text string
	 *
	 * @see other setContent()
	 */
	void setContent(const char* text);

	/**
	 * Sets the content of the pop-up with WViewController instance.
	 * This instance life-time will be attached to WPopupController instance.
	 *
	 * @param[in]	control	WViewController instance to be set
	 *
	 * @see other setContent()
	 */
	void setContent(WViewController* control);

	/**
	 * Sets the creation function of the content Evas Object of the pop-up.
	 *
	 * @param[in]	contentCreator	content creation function object. At this function, create the content evas object and return it.
	 *
	 * @see other setContent()
	 */
	void setContent(std::function<Evas_Object* (Evas_Object* parent)> contentCreator);


	/**
	 * Sets the string ID's domain name to be automatically translated on language change.
	 * The text content and title text should be set with string ID.
	 *
	 * @see setTitle( const char* title), setContent( const char* text)
	 */
	void setTextTranslatable();

	/**
	 * Sets the string ID's domain name to be automatically translated on language change. The text content and title text should be set with string ID.
	 * If you want to use other text domain, use this API exceptionally.
	 *
	 * @param[in]	textDomain	text domain name
	 *
	 * @see setTitle( const char* title), setContent( const char* text)
	 */
	void setTextTranslatable(const char* textDomain);

	/**
	 * Add buttons with button text string and button pressed callback function.
	 *
	 * @param[in]	buttonText	text string of button
	 * @param[in]	buttonCb	callback function object to be called when button is pressed.
	 *                          At this function, if it returns true, pop-up will be deleted. If not, pop-up will not.
	 * @param[in]	setPropertyCb callback function object.
	 *                            Through this function object, additional properties can be set to the button evas object.
	 *
	 * @return true if succeeded
	 */
	bool addButton(const char* buttonText, std::function<void (bool* destroyPopup)> buttonCb, std::function<void (Evas_Object* button)> setPropertyCb);
	bool addButton(const char* buttonText, std::function<void (bool* destroyPopup)> buttonCb);

	/**
	 * Add handler function when back button is pressed.
	 * If it is not set, pop-up will be closed.
	 *
	 * @param[in]	backCb	call back function object.
	 *                      If it returns true, pop-up will be closed, if not, remained.
	 *
	 */
	void setOnBack(std::function<void (bool* destroyPopup)> backCb);

	/**
	 * Add handler function to set more properties of pop-up.
	 * It will be called just after pop-up evas object is created in onCreate method.
	 *
	 * @param[in]	addPropertiesCb	callback function object
	 *
	 * @return true if succeeded
	 */
	void setOnAddProperties(std::function<void(Evas_Object* popup)> addPropertiesCb);

protected:
	/**
	 * Override it, if you want to create your own pop-up. WPopupController::onCreate will be called at the end of that function.
	 * This method comes from WViewController class.
	 *
	 * @param[in]	parent	parent evas object
	 * @param[in]	param	user data
	 *
	 * @return Evas object created
	 */
	virtual Evas_Object* onCreateView(Evas_Object* parent, void* param); //override;

	/**
	 * It is called when pop-up instance is deleted. This method comes from WViewController class.
	 *
	 * @see onCreate()
	 */
	virtual void onDestroy(); //override;

protected:
	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WPopupController();

private:
	WDECLARE_PRIVATE_IMPL(WPopupController);
	WDISABLE_COPY_AND_ASSIGN(WPopupController);
};

} // namespace

#endif /* _APP_ASSIST_WPOPUP_CONTROLLER_H_ */
