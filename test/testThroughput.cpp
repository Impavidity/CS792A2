//
// Created by zalsader on 19/10/17.
//

#define BOOST_TEST_MODULE My Test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <iostream>

#define CLIENT_DIR "/tmp/client"
#define WRITESIZE 10240
#define TPTWRITES 1024
#define TRIES 10

BOOST_AUTO_TEST_CASE(throughput) {
    std::ifstream ris("/dev/urandom");
    char randomString[WRITESIZE + 10];
    ris.read(randomString, WRITESIZE);
    chdir(CLIENT_DIR);
    std::srand(clock());
    for (int t = 0; t < TRIES; t++) {
        std::string fileName = "testThroughput.test";
        fileName += std::to_string(std::rand());
        clock_t t1 = clock();
        std::ofstream os(fileName);
        for (int i = 0; i < TPTWRITES; i++) {
            os.write(randomString, WRITESIZE);
        }
        os.close();
        std::cout << clock() - t1 << ",";
        usleep(1000);
        std::ifstream is(fileName, std::ios::ate);
        long int size = is.tellg();
        std::cout << size << std::endl;
        //    BOOST_CHECK_EQUAL(size, TPTWRITES*WRITESIZE);
        is.close();
//        unlink(fileName.c_str());
        usleep(500000);
    }
}
