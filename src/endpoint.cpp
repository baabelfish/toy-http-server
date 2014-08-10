#include "endpoint.hpp"

Endpoint::Endpoint(std::string&& path, RestFunction&& f):
    m_path(path),
    m_regex_path(path),
    m_keys(),
    m_regex(),
    m_f(f) {
    constructRegex(m_path);
}

Endpoint::~Endpoint() {
}

bool Endpoint::match(Request& req) {
    std::smatch match;
    if (!std::regex_match(req.m_type + req.m_path, match, m_regex) && match.size() != m_keys.size() + 1) {
        return false;
    }

    for (std::size_t i = 1; i < match.size(); ++i) {
        std::string key = m_keys[i - 1];
        std::string value(match[i].str().c_str());
        req.m_params[key] = value;
    }
    return true;
}

std::string Endpoint::path() const {
    return m_path;
}

std::string Endpoint::regexPath() const {
    return m_regex_path;
}

Response Endpoint::exec(const Request& req) {
    return std::move(m_f(req));
}

void Endpoint::constructRegex(const std::string& path) {
    std::smatch match;
    std::string str_regex("[a-zA-Z0-9]+");
    std::regex keyregex("(.*)/(:" + str_regex + ").*");
    std::size_t regexed_last = path.size();
    std::regex_match(path, match, keyregex);
    while (match.size() > 2) {
        std::string str = match[2].str();
        m_keys.push_back(str.substr(1));
        regexed_last = m_regex_path.rfind(str, regexed_last);
        m_regex_path.replace(regexed_last, str.size(), "(" + str_regex + ")");
        std::regex_match(match[1].str(), match, keyregex);
    }

    std::reverse(m_keys.begin(), m_keys.end());
    m_regex = std::regex(m_regex_path);
}
