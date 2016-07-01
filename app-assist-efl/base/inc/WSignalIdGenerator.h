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

#ifndef _APP_ASSIST_WSIGNAL_ID_GENERATOR_H_
#define _APP_ASSIST_WSIGNAL_ID_GENERATOR_H_

#include "WDefine.h"

namespace app_assist {

class WAPP_ASSIST_EXPORT WSignalIdGenerator {
public:
	/**
	 * Gets the unique id for WSignal listener.
	 * WSignal class internal use only
	 *
	 * @return the id
	 *
	 * @see addListener
	 */
	static unsigned int generate();
};

} // namespace

#endif /* _APP_ASSIST_WSIGNAL_ID_GENERATOR_H_ */
