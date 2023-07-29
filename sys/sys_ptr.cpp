//
// Created by impodog on 7/28/2023.
//


#include "init.h"

namespace pups_extension::sys {

    pups::ObjectPtr same(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 2) {
            map->throw_error(std::make_shared<pups::library::ArgumentError>("sys.same requires two only arguments."));
        } else {
            auto first = *args.front();
            args.pop_front();
            auto second = *args.front();
            args.pop_front();
            return first.get() == second.get() ? pups::True : pups::False;
        }
        return pups::pending;
    }

    pups::ObjectPtr swap(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 2) {
            map->throw_error(std::make_shared<pups::library::ArgumentError>("sys.swap requires two only arguments."));
        } else {
            auto first = args.front();
            args.pop_front();
            auto second = args.front();
            args.pop_front();
            first->swap(*second);
        }
        return pups::pending;
    }

    pups::ObjectPtr ptr_to_str(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 1) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("sys.ptr_to_str requires one only argument."
                                                                   " pending is returned."));
        } else {
            return std::make_shared<pups::String>(pups::library::to_string(args.front()->get()));
        }
        return pups::pending;
    }

    pups::ObjectPtr is_unique(pups::FunctionArgs &args, pups::Map *map) {
        if (args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("sys.is_unique requires at least one argument."
                                                                   " pending is returned."));
        } else {
            while (!args.empty()) {
                if (!args.front()->unique()) {
                    return pups::False;
                }
                args.pop_front();
            }
            return pups::True;
        }
        return pups::pending;
    }

    pups::ObjectPtr count_same(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 1) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("sys.count_same requires one only argument."
                                                                   " pending is returned."));
        } else {
            return std::make_shared<pups::Int>(args.front()->use_count());
        }
        return pups::pending;
    }

    pups::Id id_same{"", "same"}, id_swap{"", "swap"}, id_ptrToStr{"", "ptr_to_str"},
            id_isUnique{"", "is_unique"}, id_countSame{"", "count_same"};

    void init_ptr(pups::PackageHandler &handler) {
        handler.add(id_same, std::make_shared<pups::Function>(same));
        handler.add(id_swap, std::make_shared<pups::Function>(swap));
        handler.add(id_ptrToStr, std::make_shared<pups::Function>(ptr_to_str));
        handler.add(id_isUnique, std::make_shared<pups::Function>(is_unique));
        handler.add(id_countSame, std::make_shared<pups::Function>(count_same));
    }
}
