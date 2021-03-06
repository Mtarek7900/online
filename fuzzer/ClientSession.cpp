#include <iostream>

#include "ClientSession.hpp"

bool DoInitialization()
{
    LOOLWSD::ChildRoot = "/fuzz/child-root";
    return true;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    static bool initialized = DoInitialization();
    (void)initialized;

    std::string uri;
    Poco::URI uriPublic;
    std::string docKey = "/fuzz/fuzz.odt";
    auto docBroker = std::make_shared<DocumentBroker>(uri, uriPublic, docKey);

    std::string id;
    bool isReadOnly = false;
    const std::string hostNoTrust;
    auto session
        = std::make_shared<ClientSession>(id, docBroker, uriPublic, isReadOnly, hostNoTrust);

    bool fin = false;
    WSOpCode code = WSOpCode::Text;
    std::string input(reinterpret_cast<const char*>(data), size);
    std::stringstream ss(input);
    std::string line;
    while (std::getline(ss, line, '\n'))
    {
        std::vector<char> lineVector(line.data(), line.data() + line.size());
        session->handleMessage(fin, code, lineVector);
    }

    // Make sure SocketPoll::_newCallbacks does not grow forever, leading to OOM.
    Admin::instance().poll(SocketPoll::DefaultPollTimeoutMs);
    return 0;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
