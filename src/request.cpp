#include "request.hpp"

Request::Request(const zmq::Frame& frame):
    m_type(),
    m_path(),
    m_host(),
    m_version(),
    m_headers(),
    m_data() {
    parse(frame);
}

Request::~Request() {
}

void Request::parse(const zmq::Frame& frame) {
    std::string line;
    std::stringstream ss(frame.data);

    // Parse path
    ss >> m_type;
    ss >> m_path;
    ss >> m_version;
    std::getline(ss, line);

    // Parse headers
    while (std::getline(ss, line) && line.size() > 1) {
        auto split = line.find(":");
        m_headers[line.substr(0, split)] = line.substr(split + 2);
    }

    // Parse data
    m_data.clear();
    while (std::getline(ss, line)) {
        m_data += line;
    }
}

void Request::debug() {
    std::cout << "Type:    " << m_type << std::endl;
    std::cout << "Path:    " << m_path << std::endl;
    std::cout << "Version: " << m_version << std::endl;
    std::cout << std::endl;

    std::cout << "Headers:" << std::endl;
    for (auto& x : m_headers) { std::cout << x.first << ": " << x.second << std::endl; }
    std::cout << std::endl;

    std::cout << "Data: " << std::endl;
    std::cout << m_data << std::endl;
}

std::string Request::param(std::string key) const {
    return m_params.at(key);
}

std::string& Request::param(std::string key) {
    return m_params.at(key);
}

std::string Request::path() const { return m_path; }
std::string Request::host() const { return m_host; }
std::string Request::version() const { return m_version; }
std::string Request::type() const { return m_type; }
std::string Request::data() const { return m_data; }
