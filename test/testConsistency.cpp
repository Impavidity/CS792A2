//
// Created by zalsader on 19/10/17.
//

#define BOOST_TEST_MODULE My Test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <iostream>

#define CLIENT_DIR "/tmp/client"
#define WRITES 10

BOOST_AUTO_TEST_CASE(consistency) {
    chdir(CLIENT_DIR);
    std::string fileName = "testConsistency.test";
    std::srand(clock());
    fileName += std::to_string(std::rand());
    std::ofstream os(fileName);
    for (int i = 0; i < WRITES; i++) {
        os << i << std::endl;
        std::cout << i << std::endl;
        sleep(1);
    }
    os.close();
    std::ifstream is(fileName);
    int j;
    for (int i = 0; i < WRITES; i++) {
        is >> j;
        std::cout << i  << " " << j << std::endl;
        BOOST_CHECK_EQUAL(i, j);
    }
    is.close();
    unlink(fileName.c_str());
}
