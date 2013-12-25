#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <iterator>

#include <boost/bind.hpp>
#include <boost/ref.hpp>

using namespace std;
using namespace boost;

#define PROGRAM_NAME "%(program)"

static string usage_format = 
    "Generate error_codes.{h,cpp} from error_codes.err.\n\n"
    "Format of error_codes.err:\n\n"
    "error_code(\"symbol1\", code1)\n"
    "error_code(\"symbol2\", code2)\n"
    "...\n"
    "(\"class1\", [\"symbol1\", \"symbol2, ...\"])\n\n"
    "Usage:\n"
    "    " PROGRAM_NAME " <path to error_codes.err> <header file path> <source file path>\n";

#define ERROR_CODE_ENUM_DECLARATIONS "%(error_code_enum_declarations)"
#define ERROR_CODE_CLASS_PREDICATE_DECLARATIONS "%(error_code_class_predicate_declarations)"

static string header_format =
    "// AUTO-GENERATED FILE DO NOT EDIT\n"
    "// See utils/generate_error_codes.cpp\n"
    "// This file is generated by \"Max Zhou\" using CMake.\n"
    "/*    Copyright 2012 10gen Inc.\n"
    " *\n"
    " *    Licensed under the Apache License, Version 2.0 (the \"License\");\n"
    " *    you may not use this file except in compliance with the License.\n"
    " *    You may obtain a copy of the License at\n"
    " *\n"
    " *    http://www.apache.org/licenses/LICENSE-2.0\n"
    " *\n"
    " *    Unless required by applicable law or agreed to in writing, software\n"
    " *    distributed under the License is distributed on an \"AS IS\" BASIS,\n"
    " *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
    " *    See the License for the specific language governing permissions and\n"
    " *    limitations under the License.\n"
    " */\n\n"
    "#pragma once\n\n"
    "#include \"mongo/base/string_data.h\"\n"
    "#include \"mongo/client/export_macros.h\"\n\n"
    "namespace mongo {\n\n"
    "    /**\n"
    "     * This is a generated class containing a table of error codes and their corresponding error\n"
    "     * strings. The class is derived from the definitions in src/mongo/base/error_codes.err file.\n"
    "     *\n"
    "     * Do not update this file directly. Update src/mongo/base/error_codes.err instead.\n"
    "     */\n\n"
    "    class MONGO_CLIENT_API ErrorCodes {\n"
    "    public:\n"
    "        enum Error {\n"
    ERROR_CODE_ENUM_DECLARATIONS
    "            MaxError\n"
    "        };\n\n"
    "        static const char* errorString(Error err);\n\n"
    "        /**\n"
    "         * Parse an Error from its \"name\".  Returns UnknownError if \"name\" is unrecognized.\n"
    "         *\n"
    "         * NOTE: Also returns UnknownError for the string \"UnknownError\".\n"
    "         */\n"
    "        static Error fromString(const StringData& name);\n\n"
    "        /**\n"
    "         * Parse an Error from its \"code\".  Returns UnknownError if \"code\" is unrecognized.\n"
    "         *\n"
    "         * NOTE: Also returns UnknownError for the integer code for UnknownError.\n"
    "         */\n"
    "        static Error fromInt(int code);\n\n"
    ERROR_CODE_CLASS_PREDICATE_DECLARATIONS
    "    };\n\n"
    "}  // namespace mongo\n";

#define SYMBOL_TO_STRING_CASES "%(symbol_to_string_cases)"
#define STRING_TO_SYMBOL_CASES "%(string_to_symbol_cases)"
#define INT_TO_SYMBOL_CASES "%(int_to_symbol_cases)"
#define ERROR_CODE_CLASS_PREDICATE_DEFINITIONS "%(error_code_class_predicate_definitions)"

static string source_format =
    "// AUTO-GENERATED FILE DO NOT EDIT\n"
    "// See utils/generate_error_codes.cpp\n"
    "// This file is generated by \"Max Zhou\" using CMake.\n"
    "/*    Copyright 2012 10gen Inc.\n"
    " *\n"
    " *    Licensed under the Apache License, Version 2.0 (the \"License\");\n"
    " *    you may not use this file except in compliance with the License.\n"
    " *    You may obtain a copy of the License at\n"
    " *\n"
    " *    http://www.apache.org/licenses/LICENSE-2.0\n"
    " *\n"
    " *    Unless required by applicable law or agreed to in writing, software\n"
    " *    distributed under the License is distributed on an \"AS IS\" BASIS,\n"
    " *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
    " *    See the License for the specific language governing permissions and\n"
    " *    limitations under the License.\n"
    " */\n\n"
    "#include \"mongo/base/error_codes.h\"\n\n"
    "#include <cstring>\n\n"
    "namespace mongo {\n"
    "    const char* ErrorCodes::errorString(Error err) {\n"
    "        switch (err) {\n"
    SYMBOL_TO_STRING_CASES
    "        default: return \"Unknown error code\";\n"
    "        }\n"
    "    }\n\n"
    "    ErrorCodes::Error ErrorCodes::fromString(const StringData& name) {\n"
    STRING_TO_SYMBOL_CASES
    "        return UnknownError;\n"
    "    }\n\n"
    "    ErrorCodes::Error ErrorCodes::fromInt(int code) {\n"
    "        switch (code) {\n"
    INT_TO_SYMBOL_CASES
    "        default:\n"
    "        return UnknownError;\n"
    "        }\n"
    "    }\n\n"
    ERROR_CODE_CLASS_PREDICATE_DEFINITIONS
    "}  // namespace mongo\n";

#define ERROR_CLASS_NAME "%(error_class_name)"
#define ERROR_CLASS_CODE_CASES "%(error_class_code_cases)"

static string error_code_class_predicate_definition_format =
    "    bool ErrorCodes::is" ERROR_CLASS_NAME "(Error err) {\n"
    "        switch (err) {\n"
    ERROR_CLASS_CODE_CASES
    "            return true;\n"
    "        default:\n"
    "            return false;\n"
    "        }\n"
    "    }\n\n";

string replace_string(string &to_replace, const string &format, const string &replace_with)
{
    string::size_type index = to_replace.find(format);
    to_replace.replace(index, format.length(), replace_with);

    return (to_replace);
}

void usage(const char *program)
{
    cout << replace_string(usage_format, PROGRAM_NAME, program);
}

struct error_code
{
    error_code(string name_, int code_)
        : name(name_)
        , code(code_)
    {
    }

    string name;
    int code;
};

typedef list< ::error_code> error_code_list;

struct error_code_order_by_value
{
    bool operator()(const ::error_code &code1, const ::error_code &code2)
    {
        return (code1.code < code2.code);
    }
};

string parse_name(const string &str)
{
    string::size_type index1 = str.find('\"');
    string::size_type index2 = str.find('\"', index1 + 1);
    if (index1 == string::npos || index2 == string::npos)
    {
        throw runtime_error("Can't parse error_codes file, format error");
    }

    return (str.substr(index1 + 1, index2 - index1 - 1));
}

::error_code parse_error_code(const string &line)
{
    // get name
    string name = parse_name(line);

    // get code
    string::size_type index1 = line.find_first_not_of("\", \t\r\n", line.find_last_of("\""));
    string::size_type index2 = line.find(")", index1 + 1);
    if (index1 == string::npos || index2 == string::npos)
    {
        throw runtime_error("Can't parse error_codes file, error_code format error");
    }
    int code = atoi(line.substr(index1, index2 - index1).c_str());

    return (::error_code(name, code));
}

struct error_class
{
    error_class() {}

    error_class(const string &name_, const list<string> &codes_)
        : name(name_)
        , codes(codes_)
    {
    }

    string name;
    list<string> codes;
};

typedef list<error_class> error_class_list;

list<string> split_string(string &line, char seperator)
{
    list<string> strs;

    while (true)
    {
        string::size_type index = line.find(seperator);
        if (index == string::npos)
        {
            strs.push_back(line);
            break;
        }
        else
        {
            strs.push_back(line.substr(0, index));
            line = line.substr(index + 1);
        }
    }

    return (strs);
}

error_class parse_error_class(const string &line)
{
    string::size_type index1 = line.find('\"');
    string::size_type index2 = line.find('\"', index1 + 1);
    if (index1 == string::npos || index2 == string::npos)
    {
        throw runtime_error("Can't parse error_codes file, error_class format error");
    }

    // get name
    string name = line.substr(index1 + 1, index2 - index1 - 1);

    index1 = line.find("[", index2);
    index2 = line.find("]", index1);
    if (index1 == string::npos || index2 == string::npos)
    {
        throw runtime_error("Can't parse error_codes file, error_class format error");
    }

    // get codes
    list<string> codes;
    string codes_string(line.substr(index1 + 1, index2 - index1 - 1));
    list<string> code_names = split_string(codes_string, ',');
    transform(code_names.begin(), code_names.end(), back_inserter(codes), parse_name);

    return (error_class(name, codes));
}

void parse_file(const char *filename, error_code_list &error_codes, error_class_list &error_classes)
{
    error_codes.clear();
    error_classes.clear();

    ifstream infile;
    infile.open(filename, ios_base::in);

    if (!infile.good())
    {
        throw runtime_error((string(filename) + " does not exists").c_str());
    }

    while (!infile.eof())
    {
        string line;
        getline(infile, line);

        if (line.length() > 0)
        {
            if (line[0] == '#')
            {
                // comment line
            }
            else if (line.find("error_code") == 0)
            {
                // error code
                error_codes.push_back(parse_error_code(line.substr(10)));
            }
            else if (line.find("error_class") == 0)
            {
                // error class
                error_classes.push_back(parse_error_class(line.substr(11)));
            }
        }
    }

    // sort codes
    error_codes.sort(error_code_order_by_value());
}

void generate_error_code_enum_declaration(ostringstream &os, const ::error_code &code)
{
    os << "            " << code.name << " = " << code.code << "," << endl;
}

void generate_error_code_class_predicate_declaration(ostringstream &os, const error_class &class_)
{
    os << "        static bool is" << class_.name << "(Error err);" << endl;
}

void generate_header_file(const char *filename, 
                          const error_code_list &error_codes, 
                          const error_class_list &error_classes)
{
    ostringstream os1;
    for_each(error_codes.begin(), 
             error_codes.end(), 
             bind(generate_error_code_enum_declaration, boost::ref(os1), _1));

    ostringstream os2;
    for_each(error_classes.begin(),
             error_classes.end(),
             bind(generate_error_code_class_predicate_declaration, boost::ref(os2), _1));

    // replace
    string content = replace_string(header_format, ERROR_CODE_ENUM_DECLARATIONS, os1.str());
    content = replace_string(content, ERROR_CODE_CLASS_PREDICATE_DECLARATIONS, os2.str());

    // write to file
    ofstream out(filename, ios_base::out);
    out << content;
    out.close();
}

void generate_symbol_to_string_case(ostringstream &os, const ::error_code &code)
{
    os << "        case " << code.name << ": return \"" << code.name << "\";" << endl;
}

void generate_string_to_symbol_case(ostringstream &os, const ::error_code &code)
{
    os << "        if (name == \"" << code.name << "\") return " << code.name << ";" << endl;
}

void generate_int_to_symbol_case(ostringstream &os, const ::error_code &code)
{
    os << "        case " << code.name << ": return " << code.name << ";" << endl;
}

void generate_error_class_code_case(ostringstream &os, const string &code_name)
{
    os << "        case " << code_name << ":" << endl;
}

void generate_error_code_class_predicate_definition(ostringstream &os, const error_class &class_)
{
    ostringstream os1;
    for_each(class_.codes.begin(),
             class_.codes.end(),
             bind(generate_error_class_code_case, boost::ref(os1), _1));

    string definition(error_code_class_predicate_definition_format);
    definition = replace_string(definition, ERROR_CLASS_NAME, class_.name);
    definition = replace_string(definition, ERROR_CLASS_CODE_CASES, os1.str());

    os << definition;
}

void generate_source_file(const char *filename, 
                          const error_code_list &error_codes, 
                          const error_class_list &error_classes)
{
    ostringstream os1;
    for_each(error_codes.begin(),
             error_codes.end(),
             bind(generate_symbol_to_string_case, boost::ref(os1), _1));

    ostringstream os2;
    for_each(error_codes.begin(),
             error_codes.end(),
             bind(generate_string_to_symbol_case, boost::ref(os2), _1));

    ostringstream os3;
    for_each(error_codes.begin(),
             error_codes.end(),
             bind(generate_int_to_symbol_case, boost::ref(os3), _1));

    ostringstream os4;
    for_each(error_classes.begin(),
             error_classes.end(),
             bind(generate_error_code_class_predicate_definition, boost::ref(os4), _1));

    // replace
    string content = replace_string(source_format, SYMBOL_TO_STRING_CASES, os1.str());
    content = replace_string(content, STRING_TO_SYMBOL_CASES, os2.str());
    content = replace_string(content, INT_TO_SYMBOL_CASES, os3.str());
    content = replace_string(content, ERROR_CODE_CLASS_PREDICATE_DEFINITIONS, os4.str());

    // write file
    ofstream out(filename, ios_base::out);
    out << content;
    out.close();
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        usage(argv[0]);
        return 2;
    }

    try
    {
        error_code_list error_codes;
        error_class_list error_classes;
        parse_file(argv[1], error_codes, error_classes);

        generate_header_file(argv[2], error_codes, error_classes);
        generate_source_file(argv[3], error_codes, error_classes);
    }
    catch (const exception &e)
    {
        cout << "Error occurred: " << e.what() << endl;
        return 1;
    }
}
