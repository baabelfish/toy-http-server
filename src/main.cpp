#include "server.hpp"
#include "request.hpp"
#include "response.hpp"

int main() {
    Server app(8080);
    std::unordered_map<std::string, std::string> datas;

    app.get("/", [&](const Request&) {
        return Status::OK;
    });

    app.get("/derp/:id", [&](const Request& r) {
        return "Herp: " + datas[r.param("id")];
    });

    app.get("/derp/:id/moro/:nimi", [&](const Request& r) {
        return "Herp derp with an id of " + r.param("id") + " wants to say hi to " + r.param("nimi");
    });

    app.post("/derp/:id", [&](const Request& r) {
        datas[r.param("id")] = r.data();
        return "Thank you!";
    });

    app.exec();
    return 0;
}
