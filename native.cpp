#include <iostream>

#include "Org.h"
#include "emp/base/vector.hpp"
#include "emp/config/ArgManager.hpp"
#include "emp/math/Random.hpp"
#include "emp/math/random_utils.hpp"
#include "ConfigSetup.h"

#include "World.h"




int main(int argc, char* argv[]) {
    MyConfig config;
    // config.SEED(12345);
    // config.START_POP_SIZE(12);

    auto args = emp::cl::ArgManager(argc, argv);
    if (args.ProcessConfigOptions(config, std::cout, "MySettings.cfg") ==
      false) {
      std::cerr << "There was a problem in processing the options file." << std::endl;
      exit(1);
    }
    if (args.TestUnknown() == false) {
      std::cerr << "Leftover args no good." << std::endl;
      exit(1);
    }

    std::cout << "about to read..." << std::endl;
    config.Read("MySettings.cfg");
    bool success = config.Read("MySettings.cfg");
    if (success) {
        std::cout << "read" << std::endl;
    } else {
        std::cout << "didn't read" << std::endl;
        config.Write("MySettings.cfg");
    }
    emp::Random random(config.SEED());
    std::cout << "seed: " << config.SEED() << std::endl;
    OrgWorld world(random);

    Organism* new_org = new Organism(&random);
    world.Inject(*new_org);

    world.Resize(10, 10);

    for (int update = 0; update < 3; update++) {
        world.Update();
        std::cout << world.GetNumOrgs() << std::endl;

        std::cout << world.size() << std::endl;
    }
}
