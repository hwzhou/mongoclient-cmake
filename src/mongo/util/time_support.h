// @file time_support.h

/*    Copyright 2010 10gen Inc.
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

#include <ctime>
#include <string>
#include <boost/thread/xtime.hpp>
#include <boost/version.hpp>

#include "mongo/client/export_macros.h"

namespace mongo {

    MONGO_CLIENT_API void time_t_to_Struct(time_t t, struct tm * buf , bool local = false );
    MONGO_CLIENT_API std::string time_t_to_String(time_t t);
    MONGO_CLIENT_API std::string time_t_to_String_short(time_t t);

    struct MONGO_CLIENT_API Date_t {
        // TODO: make signed (and look for related TODO's)
        unsigned long long millis;
        Date_t(): millis(0) {}
        Date_t(unsigned long long m): millis(m) {}
        operator unsigned long long&() { return millis; }
        operator const unsigned long long&() const { return millis; }
        void toTm (tm *buf);
        std::string toString() const;
        time_t toTimeT() const;
        int64_t asInt64() const {
            return static_cast<int64_t>(millis);
        }
    };

    // uses ISO 8601 dates without trailing Z
    // colonsOk should be false when creating filenames
    MONGO_CLIENT_API std::string terseCurrentTime(bool colonsOk=true);

    /**
     * Formats "time" according to the ISO 8601 extended form standard, including date,
     * and time, in the UTC timezone.
     *
     * Sample format: "2013-07-23T18:42:14Z"
     */
    MONGO_CLIENT_API std::string timeToISOString(time_t time);

    /**
     * Formats "date" according to the ISO 8601 extended form standard, including date,
     * and time with milliseconds decimal component, in the UTC timezone.
     *
     * Sample format: "2013-07-23T18:42:14.072Z"
     */
    MONGO_CLIENT_API std::string dateToISOStringUTC(Date_t date);

    /**
     * Formats "date" according to the ISO 8601 extended form standard, including date,
     * and time with milliseconds decimal component, in the local timezone.
     *
     * Sample format: "2013-07-23T18:42:14.072-05:00"
     */
    MONGO_CLIENT_API std::string dateToISOStringLocal(Date_t date);

    /**
     * Formats "date" in fixed width in the local time zone.
     *
     * Sample format: "Wed Oct 31 13:34:47.996"
     */
    MONGO_CLIENT_API std::string dateToCtimeString(Date_t date);

    MONGO_CLIENT_API boost::gregorian::date currentDate();

    // parses time of day in "hh:mm" format assuming 'hh' is 00-23
    MONGO_CLIENT_API bool toPointInTime( const std::string& str , boost::posix_time::ptime* timeOfDay );

    MONGO_CLIENT_API void sleepsecs(int s);
    MONGO_CLIENT_API void sleepmillis(long long ms);
    MONGO_CLIENT_API void sleepmicros(long long micros);

    class MONGO_CLIENT_API Backoff {
    public:

        Backoff( int maxSleepMillis, int resetAfter ) :
            _maxSleepMillis( maxSleepMillis ),
            _resetAfterMillis( maxSleepMillis + resetAfter ), // Don't reset < the max sleep
            _lastSleepMillis( 0 ),
            _lastErrorTimeMillis( 0 )
        {}

        void nextSleepMillis();

        /**
         * testing-only function. used in dbtests/basictests.cpp
         */
        int getNextSleepMillis(int lastSleepMillis, unsigned long long currTimeMillis,
                               unsigned long long lastErrorTimeMillis) const;

    private:

        // Parameters
        int _maxSleepMillis;
        int _resetAfterMillis;

        // Last sleep information
        int _lastSleepMillis;
        unsigned long long _lastErrorTimeMillis;
    };

    // DO NOT TOUCH except for testing
    MONGO_CLIENT_API void jsTimeVirtualSkew( long long skew );

    MONGO_CLIENT_API void jsTimeVirtualThreadSkew( long long skew );
    MONGO_CLIENT_API long long getJSTimeVirtualThreadSkew();

    /** Date_t is milliseconds since epoch */
    MONGO_CLIENT_API Date_t jsTime();

    /** warning this will wrap */
    MONGO_CLIENT_API unsigned curTimeMicros();
    MONGO_CLIENT_API unsigned long long curTimeMicros64();
    MONGO_CLIENT_API unsigned long long curTimeMillis64();

    // these are so that if you use one of them compilation will fail
    char *asctime(const struct tm *tm);
    char *ctime(const time_t *timep);
    struct tm *gmtime(const time_t *timep);
    struct tm *localtime(const time_t *timep);

#if defined(MONGO_BOOST_TIME_UTC_HACK) || (BOOST_VERSION >= 105000)
#define MONGO_BOOST_TIME_UTC boost::TIME_UTC_
#else
#define MONGO_BOOST_TIME_UTC boost::TIME_UTC
#endif

}  // namespace mongo

