//
// Created by impodog on 7/28/2023.
//

#include "pups.h"
#include <iostream>

int main(int argc, char **argv) {
    std::cout << "Now executing in " << argv[0] << std::endl;
    pups::Constants constants("test/out/main.con");
    pups::init(constants);

    auto control = std::make_unique<pups::Control>("test/out/main.pups", constants);
    control->run();
    control.reset();

    pups::quit();
    return 0;
}