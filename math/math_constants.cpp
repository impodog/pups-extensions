//
// Created by impodog on 7/31/2023.
//
#include "init.h"

namespace pups_extension::math {
    pups::Id id_imax{"", "imax"}, id_imin{"", "imin"}, id_fmax{"", "fmax"}, id_fmin{"", "fmin"},
            id_pi{"", "pi"}, id_e{"", "e"}, id_inf{"", "inf"}, id_nan{"", "nan"};

    void init_constants(pups::PackageHandler &handler) {
        handler.add(id_imax,
                    std::make_shared<pups::Int>(std::numeric_limits<pups::library::pups_int>::max()));
        handler.add(id_imin,
                    std::make_shared<pups::Int>(std::numeric_limits<pups::library::pups_int>::min()));
        handler.add(id_fmax,
                    std::make_shared<pups::Float>(std::numeric_limits<pups::library::pups_float>::max()));
        handler.add(id_fmin,
                    std::make_shared<pups::Float>(std::numeric_limits<pups::library::pups_float>::min()));
        handler.add(id_pi, std::make_shared<pups::Float>(M_PI));
        handler.add(id_e, std::make_shared<pups::Float>(M_E));
        handler.add(id_inf, std::make_shared<pups::Float>(INFINITY));
        handler.add(id_nan, std::make_shared<pups::Float>(NAN));
    }
}