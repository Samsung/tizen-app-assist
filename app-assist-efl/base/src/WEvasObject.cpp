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

#include "WEvasObject.h"
#include "WDebugInternal.h"

using namespace app_assist;

namespace app_assist {

class _WEvasObjectImpl : public IWEvasObject {
public:
	_WEvasObjectImpl(Evas_Object* obj) : _evasObj(obj) {}
	_WEvasObjectImpl() = delete;

public:
	virtual Evas_Object* getEvasObject() const { return _evasObj; };

	static void _objDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info) {
		auto p = (_WEvasObjectImpl*)data;
		delete p;
	};

	std::shared_ptr<IWEvasObject> _selfPtr;
	Evas_Object* _evasObj;
};
}

std::weak_ptr<IWEvasObject> app_assist::wevasobject_get_weak_ptr(Evas_Object* obj) {
	auto pImpl = new _WEvasObjectImpl(obj);
	std::shared_ptr<IWEvasObject> sp(pImpl, [](IWEvasObject * p) {});
	pImpl->_selfPtr = sp;

	evas_object_event_callback_add(obj, EVAS_CALLBACK_DEL, _WEvasObjectImpl::_objDelCb, pImpl);

	return std::weak_ptr<IWEvasObject>(pImpl->_selfPtr);
}

