#pragma once

/**
 * Container class for all the unit tests used to assert the behaviour of the software.
 * 
 * TODO: Used a free testing framework like CppUnit or GoogleTest instead of trying to reinvent the wheel...
 */
class UnitTests {

public:
   
    UnitTests() = default;
    ~UnitTests() = default;

    static bool tcpTaskDnsResolveFailureTest();
    static bool tcpTaskDnsResolveSuccessTest();

    static bool memoryTaskTest();

    static bool sqliteConnectTest();
    static bool sqliteDisconnectTest();

    static void runAllUnitTests();
};

