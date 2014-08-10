#pragma once

#include <string>
#include <unordered_map>
#include "static/status.hpp"

class Response {
public:
    Response(int code, std::string data):
        m_code(code),
        m_data(data),
        m_headers() {
    }

    Response(std::string data, Status code): Response(static_cast<unsigned>(code), data) {}
    Response(const char* data, Status code): Response(static_cast<unsigned>(code), data) {}
    Response(std::string data, unsigned code): Response(code, data) {}
    Response(const char* data, unsigned code): Response(code, data) {}
    Response(std::string data): Response(200, data) {}
    Response(const char* data): Response(200, data) {}
    Response(Status code): Response(static_cast<unsigned>(code), "") {}
    Response(unsigned code): Response(code, "") {}

    virtual ~Response() {}

    std::string getHeader(std::string key) {
        auto it = m_headers.find(key);
        return it != m_headers.end() ? it->second : "";
    }

    void setHeader(std::string key, std::string value) {
        m_headers[key] = value;
    }

    std::string construct() {
        std::string response;
        response += "HTTP/1.0 " + std::to_string(m_code) + " OK\r\n";
        response += "Content-Type: text/plain\r\n";
        for (auto& x : m_headers) { response += x.first + ": " + x.second + "\r\n"; }
        response += "\r\n";
        response += m_data;
        return response;
    }

    int code() const {
        return m_code;
    }

private:
    unsigned m_code;
    std::string m_data;
    std::unordered_map<std::string, std::string> m_headers;
};
