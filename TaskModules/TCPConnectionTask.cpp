#include <chrono>
#include <boost/asio.hpp>
#include "TCPConnectionTask.h"

using namespace boost::asio::ip;

TCPConnectionTask::TCPConnectionTask(const std::string hostName, const unsigned& portNumber)
    : BaseTask("TCPConnectionTask")
    , _hostName(hostName)
    , _portNumber(portNumber)
    , _tcpConnectionSetupTime() {}

TaskUtilities::TaskReturnData TCPConnectionTask::runTask() {

    boost::asio::io_service
        ioService;

    tcp::resolver
        tcpEndpointResolver(ioService);

    tcp::resolver::query
        resolverQuery(
            _hostName,
            std::to_string(_portNumber),
            tcp::resolver::query::numeric_service
        );

	TaskUtilities::TaskReturnData taskData{};

    try {

        // Resolve DNS name
        tcp::resolver::iterator
            resolverIterator = tcpEndpointResolver.resolve(resolverQuery);

        // Create Client Socket
        tcp::socket
            clientSocket(ioService);


        auto begin = std::chrono::high_resolution_clock::now();

        // Iterate over all endpoints. Try to connecto to one.
        boost::asio::connect(clientSocket, resolverIterator);

        auto end = std::chrono::high_resolution_clock::now();

        // Get the time required to establish a connection to the server.
        _tcpConnectionSetupTime.raw = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

        std::string connectionResult =
            "\n\t   } Succefully connected to host '" + _hostName +
            "' " + "on port '" + std::to_string(_portNumber) + "'" +
            "\n\t   } Time taken to establish connection to host: " + std::to_string(_tcpConnectionSetupTime.raw) + " miliseconds";
        TaskUtilities::logTime(connectionResult);

        updateMetrics(_tcpConnectionSetupTime);
		
		taskData.returnData.push_back({"TCP_Setup_Time_in_ms", _tcpConnectionSetupTime.raw, _tcpConnectionSetupTime.minimum , _tcpConnectionSetupTime.maximum, _tcpConnectionSetupTime.average});

        clientSocket.close();
    }
    catch (boost::system::system_error &systemError) {

        std::stringstream ss;
        ss << "\n\t   } Error code: " << systemError.code()
           << "\n\t   } Erro message: " << systemError.what();

        TaskUtilities::logTime(ss.str());
    }
    return taskData;
}