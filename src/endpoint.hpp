#pragma once

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include "request.hpp"
#include "response.hpp"
#include "config.hpp"

class Endpoint {
public:
    using RestFunction = std::function<Response(const Request&)>;

    Endpoint(std::string&& path, RestFunction&& f);
    virtual ~Endpoint();

    std::string path() const;
    std::string regexPath() const;
    bool match(Request& req);
    Response exec(const Request& req);

private:
    void constructRegex(const std::string& path);

    std::string m_path;
    std::string m_regex_path;
    std::vector<std::string> m_keys;
    std::regex m_regex;
    RestFunction m_f;
};
