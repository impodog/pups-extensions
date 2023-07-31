//
// Created by impodog on 7/31/2023.
//
#include "init.h"

namespace pups_extension::math {
    pups::ObjectPtr itof(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 1) {
            map->throw_error(std::make_shared<pups::library::ArgumentError>(
                    "math.itof requires one only argument. pending is returned."));
        } else {
            auto ptr = pups::cast<pups::Int>(*args.front());
            if (ptr) {
                return std::make_shared<pups::Float>(static_cast<pups_float>(ptr->value));
            } else {
                map->throw_error(std::make_shared<pups::library::TypeError>(
                        "math.itof requires a int instead of " + args.front()->get()->str() +
                        ". pending is returned."));
            }
        }
        return pups::pending;
    }

    pups::ObjectPtr ftoi(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 1) {
            map->throw_error(std::make_shared<pups::library::ArgumentError>(
                    "math.ftoi requires one only argument. pending is returned."));
        } else {
            auto ptr = pups::cast<pups::Float>(*args.front());
            if (ptr) {
                return std::make_shared<pups::Int>(static_cast<pups_int>(ptr->value));
            } else {
                map->throw_error(std::make_shared<pups::library::TypeError>(
                        "math.ftoi requires a float instead of " + args.front()->get()->str() +
                        ". pending is returned."));
            }
        }
        return pups::pending;
    }

    pups::Id id_itof{"", "itof"}, id_ftoi{"", "ftoi"};

    void init_conversion(pups::PackageHandler &handler) {
        handler.add(id_itof, std::make_shared<pups::Function>(itof));
        handler.add(id_ftoi, std::make_shared<pups::Function>(ftoi));
    }
}