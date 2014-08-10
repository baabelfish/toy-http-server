#pragma once

#include "socket.hpp"
#include <string>
#include <map>
#include <unordered_map>
#include <sstream>
#include "config.hpp"

class Request {
public:
    Request(const zmq::Frame& frame);
    virtual ~Request();

    void parse(const zmq::Frame& frame);

    void debug();

    std::string param(std::string key) const;
    std::string& param(std::string key);

    std::string path() const;
    std::string host() const;
    std::string version() const;
    std::string type() const;
    std::string data() const;

    std::string m_type;
    std::string m_path;
    std::string m_host;
    std::string m_version;
    std::map<std::string, std::string> m_headers;
    std::unordered_map<std::string, std::string> m_params;
    std::string m_data;
};
