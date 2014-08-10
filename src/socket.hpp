#pragma once

#include <zmq.h>
#include <iostream>
#include <string>

namespace zmq {

struct Frame {
    std::string id;
    std::string data;
};

class socket {
public:
    socket(std::size_t port);
    virtual ~socket();

    Frame receive();
    void respond(Frame frame);
    std::string address() const;
    unsigned port() const;

private:
    void* m_context;
    void* m_socket;
    std::string m_address;
    unsigned m_port;
};

} // namespace zmq
