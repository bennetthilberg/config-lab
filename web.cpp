#include "emp/web/web.hpp"
#include "ConfigSetup.h"
#include "Org.h"
#include "World.h"
#include "emp/config/ArgManager.hpp"
#include "emp/math/Random.hpp"
#include "emp/prefab/ConfigPanel.hpp"
#include "emp/web/Animate.hpp"
#include "emp/web/Document.hpp"
#include "emp/web/UrlParams.hpp"

emp::web::Document doc("target");
emp::web::Document settings("settings");
MyConfig config;

class AEAnimator : public emp::web::Animate {

    // arena width and height
    const int num_h_boxes = 10;
    const int num_w_boxes = 10;
    const double RECT_SIDE = 10;
    const double width{num_w_boxes * RECT_SIDE};
    const double height{num_h_boxes * RECT_SIDE};
    emp::Random random{5};
    OrgWorld world{random};

    emp::web::Canvas canvas{width, height, "canvas"};

  public:
    AEAnimator() {

        // apply configuration query params and config files to config
        auto specs = emp::ArgManager::make_builtin_specs(&config);
        emp::ArgManager am(emp::web::GetUrlParams(), specs);
        am.UseCallbacks();
        if (am.HasUnused())
            std::exit(EXIT_FAILURE);
        

        // setup configuration panel
        emp::prefab::ConfigPanel config_panel(config);
        config_panel.ExcludeSetting("SEED");
        //doc << config_panel;
        settings << config_panel;

        // shove canvas into the div
        // along with a control button

        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        Organism* new_org = new Organism(&random);
        world.SetPopStruct_Grid(num_w_boxes, num_h_boxes);
        world.Resize(num_h_boxes, num_w_boxes);
        world.Inject(*new_org);

        new_org = new Organism(&random);
        world.Inject(*new_org);
    }

    void DoFrame() override {
        canvas.Clear();
        world.Update();

        int org_num = 0;
        for (int x = 0; x < num_w_boxes; x++) {
            for (int y = 0; y < num_h_boxes; y++) {
                if (world.IsOccupied(org_num)) {
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE,
                                RECT_SIDE, "black", "black");
                } else {
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE,
                                RECT_SIDE, "white", "black");
                }
                org_num++;
            }
        }
    }
};

AEAnimator animator;

int main(int argc, char* argv[]) {
    //
    auto args = emp::cl::ArgManager(argc, argv);
    if (args.ProcessConfigOptions(config, std::cout, "MySettings.cfg") ==
        false) {
        std::cerr << "There was a problem in processing the options file."
                  << std::endl;
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
    //
    animator.Step();
}