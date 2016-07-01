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

#ifndef _APP_ASSIST_WSIGNAL_H_
#define _APP_ASSIST_WSIGNAL_H_

#include <memory>
#include <list>
#include <map>
#include <functional>
#include <Evas.h>
#include "WDefine.h"
#include "WDebugBase.h"
#include "WSignalIdGenerator.h"

namespace app_assist {

const unsigned int WSIGNAL_INVALID_ID = 0;

typedef unsigned int WSignal_ListenerId;

/**
* @class	WSignal
* @brief	WSignal<Func> is a base template class. WSignal class is to provide the implementation of observer or publish-subscribe patterns.
*
*/
template< typename Func >
class WSignal {
};

/**
* @class	WSignal
* @brief	WSignal< Ret(Args...) > is template class when a callback function has return type and variadic arguments.
*           Return type and arguments types and the number of arguments are decided by template parameters.
*/
template< typename Ret, typename... Args >
class WSignal<Ret(Args...)> {
public:
	/**
	 * This is the default constructor for this class.
	 *
	 */
	WSignal() : _guardFlag(false) {};

	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WSignal() { removeAllListeners(); };

	/**
	 * Adds a listener function object to this signal object to get a signal.
	 *
	 * @param[in]	listener	callback function object
	 * @param[in]	evasObj		optional. evas object related to listener function object.
	 *                          When this evas object is deleted, automatically this listener will be removed from the signal object.
	 *                          Then you don't need to call removeListener method.
	 *                          If it is not set, do not miss call removeListener by yourself.
	 *
	 * @return the added listener id
	 *
	 * @see removeListener
	 */
	WSignal_ListenerId addListener(std::function< Ret(Args...) > listener, Evas_Object* evasObj) {
		WSignal_ListenerId id = WSignalIdGenerator::generate();

		auto l = app_assist::make_unique<ListenerInfo>();
		l->cb = listener;
		l->evasObj = evasObj;
		l->id = id;
		l->signal = this;

		if (_guardFlag)
			_listAddedListeners.push_back(std::move(l));
		else {
			_mapListeners[id] = std::move(l);
			if (evasObj)
				evas_object_event_callback_add(evasObj, EVAS_CALLBACK_DEL, _listenerEvasObjectDelCb, _mapListeners[id].get());
		}

		return id;
	};

	WSignal_ListenerId addListener(std::function< Ret(Args...) > listener) { return addListener(listener, nullptr); };

	/**
	 * Removes a listener function object from this signal object.
	 *
	 * @param[in]	id    listener id which is returned from addListener
	 *
	 * @see addListener
	 */
	void removeListener(WSignal_ListenerId& id) {
		if (id == WSIGNAL_INVALID_ID)
			return;

		auto it = _mapListeners.find(id);
		if (it != _mapListeners.end()) {
			if (it->second->evasObj)
				evas_object_event_callback_del_full(it->second->evasObj, EVAS_CALLBACK_DEL, _listenerEvasObjectDelCb, it->second.get());
			if (_guardFlag)
				it->second->removed = true;
			else
				_mapListeners.erase(id);
		}
		else
			WERROR("Already removed listener! =%d", id);

		id = WSIGNAL_INVALID_ID;
	};

	/**
	 * Checks the validity of listener id.
	 *
	 * @param[in]	id    the id of a listener
	 *
	 * @return true, if it is valid.
	 *
	 * @see addListener
	 */
	bool isValid(WSignal_ListenerId id) const {
		if (id == WSIGNAL_INVALID_ID)
			return false;

		auto it = _mapListeners.find(id);
		if (it != _mapListeners.end())
			return true;

		return false;
	};

	/**
	 * Emits a signal to all added listeners.
	 *
	 * @param[in]	args    variadic arguments to be given to listeners
	 *
	 */
	void emit(Args... args) {
		_guardFlag = true;
		onEmit(args...);
		_guardFlag = false;

		for (auto it = _mapListeners.begin(); it != _mapListeners.end();) {
			if (it->second->removed)
				_mapListeners.erase(it++);
			else
				it++;
		}
		if (_listAddedListeners.empty() == false) {
			for (auto& elem : _listAddedListeners) {
				WSignal_ListenerId id = elem->id;
				_mapListeners[id] = std::move(elem);
				if (_mapListeners[id]->evasObj)
					evas_object_event_callback_add(_mapListeners[id]->evasObj, EVAS_CALLBACK_DEL, _listenerEvasObjectDelCb, _mapListeners[id].get());
			}
			_listAddedListeners.clear();
		}
	};

	/**
	 * Gets the number of added listeners.
	 *
	 * @return the number of listeners
	 */
	int getListenerCount() const { return _mapListeners.size(); };

	/**
	 * Removes all added listeners
	 *
	 */
	void removeAllListeners() {
		for (auto& it : _mapListeners) {
			if (it.second->evasObj)
				evas_object_event_callback_del_full(it.second->evasObj, EVAS_CALLBACK_DEL, _listenerEvasObjectDelCb, it.second.get());
			if (_guardFlag)
				it.second->removed = true;
		}

		if (_guardFlag == false)
			_mapListeners.clear();
	};

protected:
	/**
	 * Overrides this method and set how to emit signals to listeners and how to handle the return value from listeners.
	 *
	 * @param[in]	args    variadic arguments to be given to listeners
	 */
	virtual void onEmit(Args... args) = 0;

	struct ListenerInfo {
		ListenerInfo() : evasObj(nullptr), removed(false), id(0), signal(nullptr) {};
		~ListenerInfo() {};

		std::function< Ret(Args...)> cb;
		Evas_Object* evasObj;
		bool removed;
		WSignal_ListenerId id;
		WSignal<Ret(Args...)>* signal;
	};
	std::map< WSignal_ListenerId, std::unique_ptr<ListenerInfo> > _mapListeners;

private:
	static void _listenerEvasObjectDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info) {
		auto li = static_cast<ListenerInfo*>(data);
		li->evasObj = nullptr;

		if (li->signal->_guardFlag) {
			li->removed = true;
			return;
		}

		li->signal->removeListener(li->id);
	};

	bool _guardFlag;
	std::list< std::unique_ptr<ListenerInfo> > _listAddedListeners;

	WDISABLE_COPY_AND_ASSIGN(WSignal<Ret(Args...)>);
};

/**
* @class	WSignal
* @brief	WSignal< void(Args...) > is template class when a callback function has no return type and variadic arguments.
*           Arguments types and the number of arguments are decided by template parameters.
*
*/
template< typename... Args >
class WSignal<void (Args...)> {
public:
	/**
	 * This is the default constructor for this class.
	 *
	 */
	WSignal() : _guardFlag(false) {};

	/**
	 * This is the destructor for this class.
	 *
	 */
	virtual ~WSignal() { removeAllListeners(); };

	/**
	 * Adds a listener function object to this signal object to get a signal.
	 *
	 * @param[in]	listener	callback function object
	 * @param[in]	evasObj		optional. evas object related to listener function object.
	 *                          When this evas object is deleted, automatically this listener will be removed from the signal object.
	 *                          Then you don't need to call removeListener method.
	 *                          If it is not set, do not miss call removeListener by yourself.
	 *
	 * @return the added listener id
	 *
	 * @see removeListener
	 */
	WSignal_ListenerId addListener(std::function< void (Args...) > listener, Evas_Object* evasObj) {
		WSignal_ListenerId id = WSignalIdGenerator::generate();

		auto l = app_assist::make_unique<ListenerInfo>();
		l->cb = listener;
		l->evasObj = evasObj;
		l->id = id;
		l->signal = this;

		if (_guardFlag)
			_listAddedListeners.push_back(std::move(l));
		else {
			_mapListeners[id] = std::move(l);
			if (evasObj)
				evas_object_event_callback_add(evasObj, EVAS_CALLBACK_DEL, _listenerEvasObjectDelCb, _mapListeners[id].get());
		}

		return id;
	};

	WSignal_ListenerId addListener(std::function< void (Args...) > listener) { return addListener(listener, nullptr); };

	/**
	 * Removes a listener function object from this signal object.
	 *
	 * @param[in]	id    listener id which is returned from addListener
	 *
	 * @see addListener
	 */
	void removeListener(WSignal_ListenerId& id) {
		if (id == WSIGNAL_INVALID_ID)
			return;

		auto it = _mapListeners.find(id);
		if (it != _mapListeners.end()) {
			if (it->second->evasObj)
				evas_object_event_callback_del_full(it->second->evasObj, EVAS_CALLBACK_DEL, _listenerEvasObjectDelCb, it->second.get());
			if (_guardFlag)
				it->second->removed = true;
			else
				_mapListeners.erase(id);
		}
		else
			WERROR("Already removed listener! =%d", id);

		id = WSIGNAL_INVALID_ID;
	};

	/**
	 * Checks the validity of listener id.
	 *
	 * @param[in]	id    the id of a listener
	 *
	 * @return true, if it is valid.
	 *
	 * @see addListener
	 */
	bool isValid(WSignal_ListenerId id) const {
		if (id == WSIGNAL_INVALID_ID)
			return false;

		auto it = _mapListeners.find(id);
		if (it != _mapListeners.end())
			return true;

		return false;
	};

	/**
	 * Emits a signal to all added listeners.
	 *
	 * @param[in]	args    variadic arguments to be given to listeners
	 *
	 */
	void emit(Args... args) {
		_guardFlag = true;
		onEmit(args...);
		_guardFlag = false;

		for (auto it = _mapListeners.begin(); it != _mapListeners.end();) {
			if (it->second->removed)
				_mapListeners.erase(it++);
			else
				it++;
		}
		if (_listAddedListeners.empty() == false) {
			for (auto& elem : _listAddedListeners) {
				WSignal_ListenerId id = elem->id;
				_mapListeners[id] = std::move(elem);
				if (_mapListeners[id]->evasObj)
					evas_object_event_callback_add(_mapListeners[id]->evasObj, EVAS_CALLBACK_DEL, _listenerEvasObjectDelCb, _mapListeners[id].get());
			}
			_listAddedListeners.clear();
		}
	};

	/**
	 * Gets the number of added listeners.
	 *
	 * @return the number of listeners
	 */
	int getListenerCount() const { return _mapListeners.size(); };

	/**
	 * Removes all added listeners
	 *
	 */
	void removeAllListeners() {
		for (auto& it : _mapListeners) {
			if (it.second->evasObj)
				evas_object_event_callback_del_full(it.second->evasObj, EVAS_CALLBACK_DEL, _listenerEvasObjectDelCb, it.second.get());
			if (_guardFlag)
				it.second->removed = true;
		}

		if (_guardFlag == false)
			_mapListeners.clear();
	};

protected:
	/**
	 * Overrides this method to change how to send signals to listeners.
	 * The default implementation is to call listeners' callback sequentially.
	 *
	 * @param[in]	args    variadic arguments to be given to listeners
	 */
	virtual void onEmit(Args... args) {
		for (auto& it : _mapListeners) {
			if (it.second->removed)
				continue;

			it.second->cb(args...);
		}
	};

private:
	static void _listenerEvasObjectDelCb(void* data, Evas* evas, Evas_Object* obj, void* event_info) {
		auto li = static_cast<ListenerInfo*>(data);
		li->evasObj = nullptr;

		if (li->signal->_guardFlag) {
			li->removed = true;
			return;
		}

		li->signal->removeListener(li->id);
	};

	struct ListenerInfo {
		ListenerInfo() : evasObj(nullptr), removed(false), id(0), signal(nullptr) {};
		~ListenerInfo() {};

		std::function< void (Args...)> cb;
		Evas_Object* evasObj;
		bool removed;
		WSignal_ListenerId id;
		WSignal<void (Args...)>* signal;
	};

	bool _guardFlag;
	std::list< std::unique_ptr<ListenerInfo> > _listAddedListeners;
	std::map< WSignal_ListenerId, std::unique_ptr<ListenerInfo> > _mapListeners;

	WDISABLE_COPY_AND_ASSIGN(WSignal<void (Args...)>);
};

} // namespace

#endif /* _APP_ASSIST_WSIGNAL_H_ */
