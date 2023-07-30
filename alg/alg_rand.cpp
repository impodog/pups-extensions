//
// Created by impodog on 7/30/2023.
//

#include "init.h"

namespace pups_extension::alg {
    std::random_device rd;
    std::mt19937 gen(rd());

    using IntDis = std::uniform_int_distribution<pups::library::pups_int>;
    using FloatDis = std::uniform_real_distribution<pups::library::pups_float>;

    pups::ObjectPtr rand_int(pups::FunctionArgs &args, pups::Map *map) {
        switch (args.size()) {
            case 0:
                return std::make_shared<pups::Int>(IntDis()(gen));
            case 1: {
                auto ptr = std::dynamic_pointer_cast<pups::Int>(*args.front());
                if (ptr) {
                    return std::make_shared<pups::Int>(IntDis(0, ptr->value)(gen));
                } else {
                    map->throw_error(
                            std::make_shared<pups::library::TypeError>(
                                    "alg.rd_int with one argument must get an integer."
                                    " pending is returned."));
                }
                return pups::pending;
            }
            case 2: {
                auto first = std::dynamic_pointer_cast<pups::Int>(*args.front());
                args.pop_front();
                auto second = std::dynamic_pointer_cast<pups::Int>(*args.front());
                args.pop_front();
                if (first && second) {
                    return std::make_shared<pups::Int>(IntDis(first->value, second->value)(gen));
                } else {
                    map->throw_error(
                            std::make_shared<pups::library::TypeError>(
                                    "alg.rd_int with two arguments must get two integers."
                                    " pending is returned."));
                }
                return pups::pending;
            }
            default:
                map->throw_error(
                        std::make_shared<pups::library::ArgumentError>(
                                "alg.rd_int requires 0, 1 or 2 arguments."
                                " pending is returned."));
                return pups::pending;
        }
    }

    pups::ObjectPtr rand_float(pups::FunctionArgs &args, pups::Map *map) {
        switch (args.size()) {
            case 0:
                return std::make_shared<pups::Float>(FloatDis()(gen));
            case 1: {
                auto ptr = std::dynamic_pointer_cast<pups::Float>(*args.front());
                if (ptr) {
                    return std::make_shared<pups::Float>(FloatDis(0, ptr->value)(gen));
                } else {
                    map->throw_error(
                            std::make_shared<pups::library::TypeError>(
                                    "alg.rd_float with one argument must get a floating number."
                                    " pending is returned."));
                }
                return pups::pending;
            }
            case 2: {
                auto first = std::dynamic_pointer_cast<pups::Float>(*args.front());
                args.pop_front();
                auto second = std::dynamic_pointer_cast<pups::Float>(*args.front());
                args.pop_front();
                if (first && second) {
                    return std::make_shared<pups::Float>(FloatDis(first->value, second->value)(gen));
                } else {
                    map->throw_error(
                            std::make_shared<pups::library::TypeError>(
                                    "alg.rd_float with two arguments must get two floating numbers."
                                    " pending is returned."));
                }
                return pups::pending;
            }
            default:
                map->throw_error(
                        std::make_shared<pups::library::ArgumentError>(
                                "alg.rd_float requires 0, 1 or 2 arguments."
                                " pending is returned."));
                return pups::pending;
        }
    }

    template<typename HasData>
    struct ShuffleType {
        using type = HasData;

        static pups::ObjectPtr work(type &obj, pups::FunctionArgs &args, pups::Map *map) noexcept {
            auto &data = pups::library::utility::get_data(obj);
            std::shuffle(data.begin(), data.end(), gen);
            return pups::pending;
        }
    };

    pups::ObjectPtr rand_shuffle(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 1) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>(
                            "alg.shuffle requires one argument."
                            " No operation is done."));
        }
        try {
            return pups::library::utility::work_with_iter
                    <ShuffleType<pups::String>, ShuffleType<pups::Array>, ShuffleType<pups::Deque>>
                    (args, map);
        } catch (const std::invalid_argument &) {
            map->throw_error(
                    std::make_shared<pups::library::TypeError>(
                            "alg.shuffle requires a supported container rather than " + args.front()->get()->repr() +
                            " No operation is done."));
        }
        return pups::pending;
    }

    pups::ObjectPtr rand_choose(pups::FunctionArgs &args, pups::Map *map) {
        if (args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>(
                            "alg.choose requires at least one argument."
                            " pending is returned."));
        } else {
            size_t pos = IntDis(0, args.size() - 1)(gen);
            return *args.at(pos);
        }
        return pups::pending;
    }

    pups::Id id_randInt{"", "rd_int"}, id_randFloat{"", "rd_float"},
            id_shuffle{"", "shuffle"}, id_choose{"", "choose"};

    void init_rand(pups::PackageHandler &handler) {
        handler.add(id_randInt, std::make_shared<pups::Function>(rand_int));
        handler.add(id_randFloat, std::make_shared<pups::Function>(rand_float));
        handler.add(id_shuffle, std::make_shared<pups::Function>(rand_shuffle));
        handler.add(id_choose, std::make_shared<pups::Function>(rand_choose));
    }
}