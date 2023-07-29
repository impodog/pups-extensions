//
// Created by impodog on 7/29/2023.
//
#include "init.h"
#include <cstdlib>

namespace pups_extension::os {
    pups::ObjectPtr run(pups::FunctionArgs &args, pups::Map *map) {
        auto cmd = pups::library::builtins::strings::str_of(args);
        int code = std::system(cmd.c_str());
        /* Error handling can be done by returning the code
        if (code) {
            map->throw_error(std::make_shared<OsError>(
                    "os.run got code" + std::to_string(code) + " when running command \"" + cmd + "\"."));
        }
         */
        return std::make_shared<pups::Int>(code);
    }

    pups::Id id_run{"", "run"};

    void init_cmd(pups::PackageHandler &handler) {
        handler.add(id_run, std::make_shared<pups::Function>(run));
    }
}