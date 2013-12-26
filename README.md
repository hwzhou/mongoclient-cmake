MongoClient CMake
=================

CMake build package for [MongoDB](http://www.mongodb.org) client.

Recently, I learned MongoDB. I also tried the C++ client library for MongoDB. I Found it hard for us to build with different settings. Then I had this idea to port it to use [CMake](http://www.cmake.org) build system. After about a week's hard work. I finally get here. And today is Christmas. Enjoy MongoDB and Merry Christmas!
This client library is based on the official C++ source codes from [github](https://github.com/mongodb/mongo).

Notes:
------

+ Files under *utils* directory. These files are used to generate basic definitions for the C++ client. And the usage are so simple, they just provide basic error checking.

+ About BOOST, the BOOST libraries contained in the official C++ package is 1.49.0. But this version of BOOST doesn't support newest C++ compilers, such as VC12 (VC2013). So I provide both version 1.49.0 (for older C++ compilers, such as VC7) and version 1.55.0 (for new C++ compilers). About BOOST's support for C++ compilers. Please see [BOOST' web site](http://www.boost.org). This package will first find BOOST in the system. If not found, use bundled versions. CMake options relate to BOOST are:

```
Boost_USE_STATIC_LIBS -- Use static BOOST libraries  
MongoClient_BOOST_TIME_UTC_HACK -- Some Linux distributions changed "boost/thread/xtime.hpp". If you encountered errors related to "mongo/bson/bson_extract.cpp", please set it to ON and try again.  
Boost_NO_BOOST_CMAKE -- If you encounter errors related to BOOST, set it to ON are try again.  
```

+ [OpenSSL](http://www.openssl.org)：This package uses *FindOpenSSL.cmake* to find OpenSSL libraries. If found, declares **MONGO_SSL** and uses OpenSSL.
+ class RCString：In *mongo/util/intrusive_counter.h*, this class overloads *operator new()* and *operator delete()*. VC will emit warnings about one argument form of *operator delete()*, and GCC 4.8 will emit errors about two arguments form of *operator delete()*. So I made the following changes.

```
#if defined(_MSC_VER)  
    void operator delete (void* ptr, size_t realSize) ...  
#else  
    void operator delete (void* ptr) ...  
#endif  
```

+ VC warnings: This package disabled the following VC warnings: C4996,C4800,C4244.  When we try to build shared MongoDB client library. VC will emit the following warnings as well: C4251,C4275. The former warning means our codes try to export class members, but those members' classes are not exported, it is disabled in the project settings. The latter means our classes inherited from a non-export class. This warning is also disabled in the header files like this:

```
#if defined(_MSC_VER)  
#pragma warning(disable: 4275)  
#endif  
```

Both these warnings can be ignored if we pay attentions when use those classes. Remember to not to mix /MD,/MDd,/MT,/MTd. Most errors are caused by this.

+ mongo/base/init.cpp：The content of this file is moved to *mongo/base/initializer.cpp*. When build static MongoDB client library, and use it in EXE. VC will optimized it away. This will cause *mongo::client::initialize()* fail, even uses */OPT:NOREF* linker option.

+ The oldest 32-bit Windows system supported by MongoDB client is Windows XP SP3 (**_WIN32_WINNT=0x0501,NTDDI_VERSION=0x05010300**). And 64-bit is Windows 2003 SP3(**_WIN32_WINNT=0x0502,NTDDI_VERSION=0x05020200**). Based on CMake option **MongoClient_ONLY_WIN7_AND_LATER**, this package will define those system related macros as minimum (this will provide maximized compatibilities). Or if set to ON, define them as **_WIN32_WINNT=0x0701,NTDDI_VERSION=0x07010000** (means Windows 7. If you want to use new synchronization objects provided by the new system). Please don't mix them with your MongoDB client libraries and your EXEs that use them. If you encountered weird errors when the program exists. Please check these definitions (normall happens in *mongo/util/currency/simplerwlock.h*).

+ **LIBMONGOCLIENT_CONSUMER**：This macro is defined in *mongo/client/dbclient.h* for client usage. But this is not correct. If we build static MongoDB client library, it is not needed. So I commented it. If you use shared MongoDB client library. Remember to define it before your #include *mongo/client/dbclient.h*.

+ CMake option **MongoClient_BUILD_EXAMPLES**: Should we build examples, or not?

+ Testing compilers and systems:

    + VC10,VC11,VC12, under Windows 7 x64, uses BOOST 1.49.0 & 1.55.0
    + GCC 4.4.7,GCC 4.8.2, under CentOS 6.1, uses BOOST 1.54.0
    + GCC 4.7.2,GCC 4.8.2 under OpenSUSE 12.3, uses BOOST 1.49.0 & 1.54.0


+ The following examples passed test: bsondemo, tutorial, firstExample, secondExample, whereExample, httpClientTest, clientTest. I didn't run the following examples due to environment restrictions: authTest, rsExample. If they both work, please let me know. Thanks.

MongoDB客户端CMake包
====================

前段时间学习了MongoDB，并试了一下编译C++的MongoDB客户端。发现使用上不是特别方便，于是有了做一个CMake生成包的想法，经过几天的努力，终于成功了。希望大家都能从这个包中找到自己需要的。由于时间较紧，难免由疏漏之处，希望大家给予指正。

注意事项：
----------

+ utils目录下的文件用于生成C++客户端所需要的基本定义。用于只是辅助，因此只采用了基本的错误处理。

+ [BOOST](http://www.boost.org)：官方的源码随附的是1.49.0版的BOOST，该版本大多数情况下没有任何问题。但是，如果使用VC12（VC2013）进行编译，自动连接时会要求vc110，这是因为该版本出现时间较早，未提供对VC12的自动连接支持。因此，在该包中，我同时提供了1.49.0以及1.55.0版本的BOOST。1.55.0版本不支持早期的VC以及其他的一些编译器，具体参见BOOST官方网站。该CMake包总是会先搜索系统中自带的BOOST库(1.49.0版本以上)，如果找到则使用系统中的库，未找到则使用包中附带的BOOST库。例外情况，如果系统中带的BOOST库版本不满足C++编译器要求，则仍然使用附带的BOOST库。BOOST相关的CMake选项如下（其他关于BOOST的CMake变量，选项可参考FindBOOST.cmake）：

```
Boost_USE_STATIC_LIBS -- 使用静态连接的BOOST库  
MongoClient_BOOST_TIME_UTC_HACK -- 某些Linux发布（如OpenSUSE 12.3）随附的BOOST库中对于 boost/thread/xtime.hpp 有所改变，如果在编译时遇到 mongo/bson/bson_extract.cpp 错误，可以打开该选择再试试，该CMake宏会定义C++宏 MONGO_BOOST_TIME_UTC_HACK  
Boost_NO_BOOST_CMAKE -- 如果在编译过程中（通常是Unix类系统）遇到某些BOOST相关错误，可以试试添加并打开该选择  
```

+ [OpenSSL](http://www.openssl.org)：该包并未包含OpenSSL，但是会自动搜索系统中的OpenSSL库（具体参见FindOpenSSL.cmake）。如果找到，则会定义**MONGO_SSL**宏以支持SSL。

+ RCString类：在文件*mongo/util/intrusive_counter.h*中，重载了*operator new()*和*operator delete()*函数，由于参数不一致，VC会给出C4291的警告，但是该函数在GCC 4.8中正常（两参数的*operator delete()*在GCC 4.8下会报错）。因此修改为如下：

```
#if defined(_MSC_VER)  
    void operator delete (void* ptr, size_t realSize) ...  
#else  
    void operator delete (void* ptr) ...  
#endif  
```

+ VC警告：在VC下编译，VC会给出这些警告C4996、C4800、C4244，这些警告在包中设置为禁止提示。还有C4251、C4275，这两个警告只在生成DLL时出现。其中，前一个警告表示类中的某些成员的类型并未在任何DLL中导出，这些类通常都是C++标准库中的模板类，该警告也在包中设置为禁止提示；后一个警告表示该类从DLL中导出，但是其某个基类并未在任何DLL中被导出，这些类都是从::boost::noncopyable继承过来的，用于禁止这些类的拷贝，该警告在源代码中采用如下方式来禁止提示：

```
#if defined(_MSC_VER)  
#pragma warning(disable: 4275)  
#endif  
```

这两个警告在于提示在使用时要注意不要混用了不同的运行时间库（即不要混用/MD、/MDd、/MT、/MTd，则不会有任何问题）。

+ mongo/base/init.cpp：该文件中的内容移到了文件*mongo/base/initializer.cpp*中，因为在编译VC静态库时，VC优化器会在连接到EXE中时将其优化掉，这会导致*mongo::client::initialize()*失败，即使增加/OPT:NOREF连接选项也不行。

+ VC系统相关宏，MongoDB支持的32位最低Windows版本为Windows XP SP3（**_WIN32_WINNT=0x0501，NTDDI_VERSION=0x05010300**）；64位最低Windows版本为Windows 2003 SP3（**_WIN32_WINNT=0x0502，NTDDI_VERSION=0x05020200**）。通常这不需要我们来定义（MongoDB客户端代码在*mongo/platform/windows_basic.h*），但是这有可能引起同步对象的不一致，该包中定义了这两个宏，根据CMake选项**MongoClient_ONLY_WIN7_AND_LATER**来指定不同的值，关闭则使用最低版本支持，可以支持较多的系统，打开则为Windows 7（**_WIN32_WINNT=0x0701，NTDDI_VERSION=0x07010000**），这将会使用一些新的同步对象，如果知道自己的程序将只在Windows 7即以后的版本中执行，可以考虑打开该选项来使用系统新特性。如果在执行EXE时，其他一切都正常，在程序退出时出现错误，请检查这两个宏的定义是否一致（主要出现在*mongo/util/currency/simplerwlock.h*）。

+ **LIBMONGOCLIENT_CONSUMER**宏：MongoDB客户端在*mongo/client/dbclient.h*中定义了该宏，表示由客户端代码使用，但这是不正确的，如果是生成静态库而不是DLL，则不需要（可参见*mongo/client/export_macros.h*）。如果在编译自己的EXE并使用DLL时，请记得在包含*mongo/client/dbclient.h*文件之前定义LIBMONGOCLIENT_CONSUMER宏。

+ CMake选项**MongoClient_BUILD_EXAMPLES**：用于指定是否生成MongoDB客户端例子。

+ 该包通过了如下编译器和系统：

    + VC10、VC11和VC12，Windows 7 x64，BOOST 1.49.0 & 1.55.0
    + GCC 4.4.7、GCC 4.8.2，CentOS 6.1，BOOST 1.54.0
    + GCC 4.7.2、GCC 4.8.2，OpenSUSE 12.3，BOOST 1.49.0 & 1.54.0


+ 通过了测试的例子：bsondemo、tutorial、firstExample、secondExample、whereExample、httpClientTest以及clientTest。由于测试系统限制，authTest和rsExample例子并未执行过。
