#pragma once
#include "BaseTask.h"
#include <boost/asio.hpp>

/**
 * Registers the time it takes to connect to a TCP server.
 */
class TCPConnectionTask : public BaseTask {

private:

    const std::string _hostName;
    const unsigned int _portNumber;

    Metrics _tcpConnectionSetupTime;

public:

    TCPConnectionTask(const std::string hostName, const unsigned int& portNumber);
    ~TCPConnectionTask() = default;

    virtual TaskUtilities::TaskReturnData runTask() override;
};

