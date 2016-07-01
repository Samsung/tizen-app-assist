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

#ifndef _APP_ASSIST_WEVAS_OBJECT_H_
#define _APP_ASSIST_WEVAS_OBJECT_H_

#include <memory>
#include <Evas.h>
#include "WDefine.h"

namespace app_assist {

class IWEvasObject;

typedef std::weak_ptr<IWEvasObject> WEvasObjectPtr;

WAPP_ASSIST_EXPORT WEvasObjectPtr wevasobject_get_weak_ptr(Evas_Object* obj);

class WAPP_ASSIST_EXPORT IWEvasObject {
protected:
	virtual ~IWEvasObject() {};

public:
	virtual Evas_Object* getEvasObject() const = 0;
};

} // namespace

#endif /* _APP_ASSIST_WEVAS_OBJECT_H_ */
