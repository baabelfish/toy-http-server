#include "socket.hpp"
#include <thread>

namespace zmq {

socket::socket(std::size_t port):
   m_context(zmq_ctx_new()),
   m_socket(zmq_socket(m_context, ZMQ_STREAM)),
   m_address("tcp://127.0.0.1:" + std::to_string(port)),
   m_port(port) {
   zmq_ctx_set(m_context, ZMQ_IO_THREADS, std::thread::hardware_concurrency());
   zmq_bind(m_socket, m_address.c_str());
}

socket::~socket() {
    zmq_close(m_socket);
    zmq_ctx_destroy(m_context);
}

Frame socket::receive() {
   static const int BUFFER_SIZE = 8192;
   uint8_t id[256];
   int id_size = 256;
   char buffer[BUFFER_SIZE];
   int buffer_size = BUFFER_SIZE;

   id_size = zmq_recv(m_socket, id, id_size, 0);
   buffer_size = zmq_recv(m_socket, buffer, sizeof(buffer), 0);

   return Frame{ std::string((char*)id, id_size), std::string(buffer, buffer_size) };
}

void socket::respond(Frame frame) {
   zmq_send(m_socket, frame.id.c_str(), frame.id.size(), ZMQ_SNDMORE);
   zmq_send(m_socket, frame.data.c_str(), frame.data.size(), ZMQ_SNDMORE);
   zmq_send(m_socket, frame.id.c_str(), frame.id.size(), ZMQ_SNDMORE);
   zmq_send(m_socket, 0, 0, ZMQ_SNDMORE);
}

std::string socket::address() const {
   return m_address;
}

unsigned socket::port() const {
   return m_port;
}

} // namespace zmq
