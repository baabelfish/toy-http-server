#pragma once

#include <iostream>
#include <chrono>
#include <regex>
#include <functional>
#include <vector>
#include "socket.hpp"
#include "request.hpp"
#include "response.hpp"
#include "endpoint.hpp"
#include "config.hpp"

class Server {
public:
    using RestFunction = std::function<Response(const Request&)>;

    Server(std::size_t port);
    virtual ~Server();

    void connect(std::string path, RestFunction&& f);
    void get(std::string path, RestFunction&& f);
    void head(std::string path, RestFunction&& f);
    void options(std::string path, RestFunction&& f);
    void patch(std::string path, RestFunction&& f);
    void post(std::string path, RestFunction&& f);
    void put(std::string path, RestFunction&& f);
    void remove(std::string path, RestFunction&& f);
    void trace(std::string path, RestFunction&& f);
    void custom(std::string type, std::string path, RestFunction&& f);

    void exec();

private:
    Response run(Request req);
    void add(std::string&& path, RestFunction&& f);
    void print(const Request& req, const Response& res, int ms);

    zmq::socket m_socket;
    std::vector<Endpoint> m_endpoints;
};
