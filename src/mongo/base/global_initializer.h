/*    Copyright 2012 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#pragma once

#include "mongo/client/export_macros.h"

namespace mongo {
    class Initializer;

    /**
     * Get the process-global initializer object.
     *
     * See mongo/base/initializer.h and mongo/base/init.h for information about process
     * initialization in mongo applications.
     */
    MONGO_CLIENT_API Initializer& getGlobalInitializer();

}  // namespace mongo
