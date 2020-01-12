
// Let Catch provide main()
#define CATCH_CONFIG_MAIN

// C++ includes
#include <string>
// Raspberry classes
#include "../server/raspberryserver.h"
//#include "../server/raspberry.h"
//#include "../server/connectclient.h"
// External classes/headers
#include "catch.hpp"

// ============
//  Test cases
// ============

SCENARIO("Test RaspberryServer when given correct file")
{
    GIVEN("A new object all correct in debug mode") {
        RaspberryServer server(
            "test_server",  // Name of the server
            2,              // Raspberry model
            "test_good.txt",// Setup file
            true,           // Debug mode server
            true            // Debug mode GPIO
        );

        WHEN("Get the server name") {
            std::string name = server.getServerName();

            THEN("Compare with test_server") {
                REQUIRE(name == "test_server");
            }
        }

        WHEN("Reading the file") {   
            std::string msg{""};
            int port{server.getServerPort()};
            std::string name{server.getServerName()};
            try {
                server.start(5050);
                server.closeConnection();
            }
            catch(const std::exception& e) {
                msg = e.what();
            }

            THEN("No error") {
                REQUIRE(msg == "");
            }

            THEN("Get the server port") {
                REQUIRE(port == 5050);
            }

            THEN("Get server name") {
                REQUIRE(name == "test_server");
            }
        }
    }
}

SCENARIO("Test RasberryServer when given bad file")
{
    GIVEN("A new object error in read file") {
        std::string msg{};
        RaspberryServer server(
            "test_server",  // Name of the server
            2,              // Raspberry model
            "test_bad.txt",// Setup file
            true,           // Debug mode server
            true            // Debug mode GPIO
        );

        WHEN("Reading the file") {
            try {
                server.start(5050);
            }
            catch(const std::exception& e) {
                msg = e.what();
            }


            THEN("Found error") {

            }
        }

        server.closeConnection();
    }
}