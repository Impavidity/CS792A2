//
// Created by zalsader on 19/10/17.
//

#define BOOST_TEST_MODULE My Test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <iostream>

#define CLIENT_DIR "/tmp/client"
#define WRITESIZE 1000
#define TPTWRITES 10

BOOST_AUTO_TEST_CASE(throughput) {
    chdir(CLIENT_DIR);
    std::ifstream ris("/dev/random");
    char randomString[WRITESIZE + 10];
    ris.read(randomString, WRITESIZE);
    std::string fileName = "testThroughput.test";
    fileName += std::rand();
    time_t t1 = time(0);
    std::ofstream os(fileName);
    for (int i = 0; i < TPTWRITES; i++) {
        os.write(randomString, WRITESIZE);
    }
    os.close();
    std::cout << t1-time(0);
    std::ifstream is(fileName, std::ios::ate);
    long int size = is.tellg();
    BOOST_CHECK_EQUAL(size, TPTWRITES*WRITESIZE);
    is.close();
    unlink(fileName.c_str());
}
