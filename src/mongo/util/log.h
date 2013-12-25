// @file log.h

/*    Copyright 2009 10gen Inc.
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

#include "mongo/base/status.h"
#include "mongo/bson/util/builder.h"
#include "mongo/logger/logger.h"
#include "mongo/logger/logstream_builder.h"
#include "mongo/logger/tee.h"
#include "mongo/util/concurrency/thread_name.h"

namespace mongo {

namespace logger {
    typedef void (*ExtraLogContextFn)(BufBuilder& builder);
    MONGO_CLIENT_API Status registerExtraLogContextFn(ExtraLogContextFn contextFn);

}  // namespace logger

    using logger::LogstreamBuilder;
    using logger::LabeledLevel;
    using logger::Tee;

    /**
     * Returns a LogstreamBuilder for logging a message with LogSeverity::Severe().
     */
    inline LogstreamBuilder severe() {
        return LogstreamBuilder(logger::globalLogDomain(),
                                getThreadName(),
                                logger::LogSeverity::Severe());
    }

    /**
     * Returns a LogstreamBuilder for logging a message with LogSeverity::Error().
     */
    inline LogstreamBuilder error() {
        return LogstreamBuilder(logger::globalLogDomain(),
                                getThreadName(),
                                logger::LogSeverity::Error());
    }

    /**
     * Returns a LogstreamBuilder for logging a message with LogSeverity::Warning().
     */
    inline LogstreamBuilder warning() {
        return LogstreamBuilder(logger::globalLogDomain(),
                                getThreadName(),
                                logger::LogSeverity::Warning());
    }

    /**
     * Returns a LogstreamBuilder for logging a message with LogSeverity::Log().
     */
    inline LogstreamBuilder log() {
        return LogstreamBuilder(logger::globalLogDomain(),
                                getThreadName(),
                                logger::LogSeverity::Log());
    }


    /**
     * Synonym for log().
     */
    inline LogstreamBuilder out() { return log(); }

    /**
     * For logging which we may not want during unit tests (dbtests) runs.  Set tlogLevel to -1 to
     * suppress MONGO_TLOG() output in a test program.
     */
    extern MONGO_CLIENT_API int tlogLevel;

#define MONGO_LOG(DLEVEL) \
    if (!(::mongo::logger::globalLogDomain())->shouldLog(::mongo::LogstreamBuilder::severityCast(DLEVEL))) {} \
    else LogstreamBuilder(::mongo::logger::globalLogDomain(), getThreadName(), ::mongo::LogstreamBuilder::severityCast(DLEVEL))

#define LOG MONGO_LOG

#define MONGO_DLOG(DLEVEL) \
    if (!(DEBUG_BUILD) && !::mongo::logger::globalLogDomain()->shouldLog(::mongo::LogstreamBuilder::severityCast(DLEVEL))) {} \
    else LogstreamBuilder(::mongo::logger::globalLogDomain(), getThreadName(), ::mongo::LogstreamBuilder::severityCast(DLEVEL))

#define MONGO_TLOG(DLEVEL) \
    if ((!::mongo::debug && ((DLEVEL) > tlogLevel)) || !::mongo::logger::globalLogDomain()->shouldLog(::mongo::LogstreamBuilder::severityCast(DLEVEL))) {} \
    else LogstreamBuilder(::mongo::logger::globalLogDomain(), getThreadName(), ::mongo::LogstreamBuilder::severityCast(DLEVEL))

    /* default impl returns "" -- mongod overrides */
    extern MONGO_CLIENT_API const char * (*getcurns)();

    inline LogstreamBuilder problem() {
        std::string curns = getcurns();
        return log().setBaseMessage(curns);
    }

    /**
     * Rotates the log files.  Returns true if all logs rotate successfully.
     */
    MONGO_CLIENT_API bool rotateLogs();

    /** output the error # and error message with prefix.
        handy for use as parm in uassert/massert.
        */
    MONGO_CLIENT_API string errnoWithPrefix( const char * prefix );

    // Guard that alters the indentation level used by log messages on the current thread.
    // Used only by mongodump (mongo/tools/dump.cpp).  Do not introduce new uses.
    struct MONGO_CLIENT_API LogIndentLevel {
        LogIndentLevel();
        ~LogIndentLevel();
    };

    extern MONGO_CLIENT_API Tee* const warnings; // Things put here go in serverStatus
    extern MONGO_CLIENT_API Tee* const startupWarningsLog; // Things put here get reported in MMS

    string MONGO_CLIENT_API errnoWithDescription(int errorcode = -1);
    void MONGO_CLIENT_API rawOut( const StringData &s );

    /*
     * Redirects the output of "rawOut" to stderr.  The default is stdout.
     *
     * NOTE: This needs to be here because the tools such as mongoexport and mongodump sometimes
     * send data to stdout and share this code, so they need to be able to redirect output to
     * stderr.  Eventually rawOut should be replaced with something better and our tools should not
     * need to call internal server shutdown functions.
     *
     * NOTE: This function is not thread safe and should not be called from a multithreaded context.
     */
    void MONGO_CLIENT_API setRawOutToStderr();

    /**
     * Write the current context (backtrace), along with the optional "msg".
     */
    void MONGO_CLIENT_API logContext(const char *msg = NULL);

} // namespace mongo
