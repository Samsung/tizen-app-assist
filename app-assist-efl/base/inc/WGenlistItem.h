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

#ifndef _APP_ASSIST_WGENLIST_ITEM_H_
#define _APP_ASSIST_WGENLIST_ITEM_H_

#include <Evas.h>
#include <Elementary.h>
#include "WDefine.h"

namespace app_assist {

class WViewController;
/**
* @class	WGenlistItem
* @brief	This class is
*
*
* The %WGenlistItem is
*/
class WAPP_ASSIST_EXPORT WGenlistItem {
public:
	/**
	 * This is the default constructor for this class.
	 *
	 */
	WGenlistItem();

	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WGenlistItem();

public:
	/**
	 * Override this method to create Elm_Genlist_Item_Class structure.
	 * It is used to append item to the genlist. Define static Elm_Getnlist_Item_Class variable and return it.
	 *
	 * @return	the pointer of Elm_Genlist_Item_Class instantiated as static variable.
	 * 			It is not needed to be free.
	 * @see getItemClassNew()
	 */
	virtual Elm_Genlist_Item_Class* getItemClassStatic() const;

	/**
	 * Override this method to create Elm_Genlist_Item_Class structure.
	 * It is used to append item to the genlist.
	 *
	 * @return	the pointer of Elm_Genlist_Item_Class created by elm_genlist_item_class_new.
	 * 			It should be free by elm_genlist_item_class_free if it is not used anymore.
	 * @see getItemClassStatic()
	 */
	virtual Elm_Genlist_Item_Class* getItemClassNew() const;

	/**
	 * Gets Elm_Object_Item of this item instance after it is appened to genlist.
	 *
	 * @return	the Elm_Object_Item
	 * @see setElmObjectItem()
	 */
	Elm_Object_Item* getElmObjectItem() const;

	/**
	 * Sets Elm_Object_Item, which is returned when this item is appended to genlist.
	 * If this Elm_Object_Item is required in your genlist item implementation, set it by this method.
	 * Then you can get it by getElmObjectItem method.
	 * After calling this method, consequently virtual method onSetElmObjectItem will be called.
	 *
	 * @param[in]	objItem	The pointer of Elm_Object_Item
	 *
	 * @see getElmObjectItem()
	 * @see onSetElmObjectItem()
	 */
	void setElmObjectItem(Elm_Object_Item* objItem);

	/**
	 * Gets static callback function to be used to append item to the genlist.
	 * Then callback will call onSelect( Elm_Object_Item* item ) method when callback is invoked.
	 *
	 * @see Evas_Smart_Cb
	 * @see onSelect( Elm_Object_Item* item )
	 */
	static Evas_Smart_Cb getSelectCb();

protected:
	/**
	 * Override this method to do something when genlist is selected.
	 * Before this, the function pointer returned by getSelectCb method, should be set as select callback of genlist when this item is appended.
	 *
	 * @param[in]	objItem	Elm_Object_Item of this instance
	 *
	 * @see getSelectCb()
	 */
	virtual void onSelect(Elm_Object_Item* objItem);

	/**
	 * Override this method to set more properties to Elm_Object_Item of this item.
	 * It is called when setElmObjectItem method is called to set the Elm_Object_Item pointer of this item.
	 *
	 * @param[in]	objItem	Elm_Object_Item of this instance
	 *
	 * @see setElmObjectItem()
	 */
	virtual void onSetElmObjectItem(Elm_Object_Item* objItem);

	/**
	 * Attaches the pop-up to the view, which is the container view of this item.
	 *
	 * @param[in]	popup	The pointer to the pop-up instance to be attached
	 *
	 * @return	true the view is found and it is attached successfully.
	 */
	bool attachToParentViewController(WViewController* popup);

private:
	WDECLARE_PRIVATE_IMPL(WGenlistItem);
	WDISABLE_COPY_AND_ASSIGN(WGenlistItem);
};

#define WGENLIST_ITEM_DEL() [](void* data, Evas_Object* obj) \
		{ \
			WGenlistItem* item = static_cast<WGenlistItem*>(data); \
			delete item; \
		}
/**
 * This is a helper function.
 * Appends genlist item to the genlist. It helps to add genlist item using WGenlistItem class to genlist.
 * Internally setElmObjectItem method of the WGenlistItem is called to set Elm_Object_Item pointer.
 * The virtual method onSelect of WGenlistItem class will be called when the item is selected.
 *
 * @param[in]	genlist	genlist Evas Object
 * @param[in]	itc		item class
 * @param[in]	item	WGenlistItem type object pointer to be appended
 * @param[in]	type	genlist item type
 *
 * @see genlist_item_append
 */
WAPP_ASSIST_EXPORT Elm_Object_Item* wgenlist_item_append(Evas_Object* genlist, const Elm_Genlist_Item_Class* itc, WGenlistItem* item, Elm_Genlist_Item_Type type);

/**
 * This is a helper function.
 * Appends genlist item to the genlist. It helps to add genlist item using WGenlistItem class to genlist.
 * Internally setElmObjectItem method of the WGenlistItem is called to set Elm_Object_Item pointer.
 * When the item is selected, selectCb will be called instead of onSelect virtual method of the WGenlistItem class.
 *
 * @param[in]	genlist	genlist Evas Object
 * @param[in]	itc		item class
 * @param[in]	item	WGenlistItem type object pointer to be appended
 * @param[in]	type	genlist item type
 * @param[in]	selectCb	the callback function to be called when the item is selected
 * @param[in]	userData	select callback function user data pointer
 *
 * @see genlist_item_append
 * @see wgenlist_item_append
 */
WAPP_ASSIST_EXPORT Elm_Object_Item* wgenlist_item_append_with_callback(Evas_Object* genlist, const Elm_Genlist_Item_Class* itc, WGenlistItem* item, Elm_Genlist_Item_Type type, Evas_Smart_Cb selectCb, const void* userData);

} // namespace

#endif /* _APP_ASSIST_WGENLIST_ITEM_H_ */
