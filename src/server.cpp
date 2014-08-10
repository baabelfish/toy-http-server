#include "server.hpp"

Server::Server(std::size_t port):
    m_socket(port),
    m_endpoints() {
}

Server::~Server() {}

void Server::connect(std::string path, RestFunction&& f) { add("CONNECT" + path, std::forward<RestFunction>(f)); }
void Server::get(std::string path, RestFunction&& f) { add("GET" + path, std::forward<RestFunction>(f)); }
void Server::head(std::string path, RestFunction&& f) { add("HEAD" + path, std::forward<RestFunction>(f)); }
void Server::options(std::string path, RestFunction&& f) { add("OPTIONS" + path, std::forward<RestFunction>(f)); }
void Server::patch(std::string path, RestFunction&& f) { add("PATCH" + path, std::forward<RestFunction>(f)); }
void Server::post(std::string path, RestFunction&& f) { add("POST" + path, std::forward<RestFunction>(f)); }
void Server::put(std::string path, RestFunction&& f) { add("PUT" + path, std::forward<RestFunction>(f)); }
void Server::remove(std::string path, RestFunction&& f) { add("DELETE" + path, std::forward<RestFunction>(f)); }
void Server::trace(std::string path, RestFunction&& f) { add("TRACE" + path, std::forward<RestFunction>(f)); }
void Server::custom(std::string type, std::string path, RestFunction&& f) { add(type + path, std::forward<RestFunction>(f)); }

void Server::exec() {
    std::cout
        << "\033[0J"
        << "\033[1m"
        << "\033[30m"
        << "App running on port "
        << "\033[37m"
        << m_socket.port()
        << std::endl;

    while (true) {
        zmq::Frame frame = m_socket.receive();
        Request req(frame);
        // req.debug();
        auto start = std::chrono::steady_clock::now();
        auto res = run(req);
        auto stop = (std::chrono::steady_clock::now() - start).count();
        frame.data = res.construct();
        print(req, res, stop);
        m_socket.respond(frame);
    }
}

Response Server::run(Request req) {
    std::smatch match;
    for (auto& x : m_endpoints) {
        if (x.match(req)) {
            return x.exec(req);
        }
    }
    return Response{ 404 };
}

void Server::add(std::string&& path, RestFunction&& f) {
    m_endpoints.push_back(Endpoint{ std::move(path), std::forward<RestFunction>(f) });
}

void Server::print(const Request& req, const Response& res, int ms) {
    std::cout
        << "\033[0m"
        << "\033[1m"
        << "\033[30m"
        << req.m_type << " "
        << req.m_path << " ";

    auto code = res.code();

    if (code < 200) { std::cout << "\033[1m\033[36m"; }
    else if (200 <= code && code < 300) { std::cout << "\033[1m\033[32m"; }
    else if (300 <= code && code < 400) { std::cout << "\033[1m\033[35m"; }
    else if (400 <= code && code < 500) { std::cout << "\033[1m\033[33m"; }
    else if (500 <= code) { std::cout << "\033[1m\033[31m"; }

    std::cout << code << "\033[1m\033[30m";
    if (ms != 0) { std::cout << " " << ms / 1000000 << "ms"; }
    std::cout << "\033[0m" << std::endl;
}
