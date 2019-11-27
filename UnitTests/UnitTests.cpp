#include "UnitTests.h"
#include "../TaskModules/TCPConnectionTask.h"
#include "../TaskModules/MemoryMonitorTask.h"
#include "../DataBase/DataBaseInterface.h"

inline constexpr char* boolToString(bool booleanValue) noexcept{
    return booleanValue ? "Passed" : "Failed";
}

bool UnitTests::tcpTaskDnsResolveFailureTest() {
    
    TCPConnectionTask tcpConnectionTask("googler.com", 42);

    auto returnValue = tcpConnectionTask.runTask();
    auto testResult = (returnValue.returnData.size() == 0) ? true : false;
    return testResult;
}

bool UnitTests::tcpTaskDnsResolveSuccessTest() {

    TCPConnectionTask tcpConnectionTask("google.com", 80);

    auto returnValue = tcpConnectionTask.runTask();
    auto testResult = (returnValue.returnData.size() != 0) ? true : false;
    return testResult;
}

bool UnitTests::memoryTaskTest() {

    MemoryMonitorTask memoryTask;

    auto returnValue = memoryTask.runTask();
    auto testResult = (returnValue.returnData.size() != 0) ? true : false;
    return testResult;
}

bool UnitTests::sqliteConnectTest() {
    
    DataBaseInterface testDataBase(TaskUtilities::dataBase);
    return testDataBase.connect();

}
bool UnitTests::sqliteDisconnectTest() {

    DataBaseInterface testDataBase(TaskUtilities::dataBase);
    if (testDataBase.connect()) {
        return testDataBase.disconnect();
    }
    
    return false;
}

void UnitTests::runAllUnitTests() {
    
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "---------------[Start of Unit Tests]---------------" << std::endl;
    std::cout << "---------------------------------------------------\n" << std::endl;

    bool testResult = false;

    testResult = UnitTests::tcpTaskDnsResolveFailureTest();
    std::cout << "\ntcpTaskDnsResolveFailureTest: " << boolToString(testResult) << "\n" << std::endl;
    
    testResult = UnitTests::tcpTaskDnsResolveSuccessTest();
    std::cout << "\ntcpTaskDnsResolveFailureTest: " << boolToString(testResult) << "\n" << std::endl;

    testResult = UnitTests::memoryTaskTest();
    std::cout << "\nmemoryTaskTest: " << boolToString(testResult) << "\n" << std::endl;

    testResult = UnitTests::sqliteConnectTest();
    std::cout << "\nsqliteConnectTest: " << boolToString(testResult) << "\n" << std::endl;

    testResult = UnitTests::sqliteDisconnectTest();
    std::cout << "\nsqliteDisconnectTest: " << boolToString(testResult) << "\n" << std::endl;

    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "----------------[End of Unit Tests]----------------" << std::endl;
    std::cout << "---------------------------------------------------\n\n" << std::endl;
}
