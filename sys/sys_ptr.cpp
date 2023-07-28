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

    pups::Id id_same{"", "same"}, id_swap{"", "swap"};

    void init_ptr(pups::PackageHandler &handler) {
        handler.add(id_same, std::make_shared<pups::Function>(same));
        handler.add(id_swap, std::make_shared<pups::Function>(swap));
    }
}
