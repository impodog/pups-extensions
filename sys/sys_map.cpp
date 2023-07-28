//
// Created by impodog on 7/28/2023.
//


#include "init.h"

namespace pups_extension::sys {
    pups::ObjectPtr is_toplevel(pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("sys.is_toplevel requires no arguments.")
            );
        }
        return map->get_parent() == map->get_global()
               ? pups::True
               : pups::False;
    }

    pups::ObjectPtr format(pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("sys.format requires no arguments.")
            );
        }
        std::string result;
        for (const auto &item: map->get_all_objects()) {
            result.append(item.first.str() + ": " + item.second->str() + "\n");
        }
        result = result.substr(0, result.size() - 1);
        return std::make_shared<pups::String>(result);
    }

    pups::ObjectPtr find(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 1) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>(
                            "sys.find requires one only argument. pending is returned.")
            );
        } else {
            auto ptr = pups::cast<pups::String>(*args.front());
            if (ptr) {
                auto result = map->find(pups::Id{ptr->data()}, map);
                if (result)
                    return result;
                else
                    return pups::False;
            } else {
                map->throw_error(
                        std::make_shared<pups::library::TypeError>(
                                "sys.find requires a String argument. pending is returned.")
                );
            }
        }
        return pups::pending;
    }

    pups::Id id_isToplevel{"", "is_toplevel"}, id_format{"", "format"}, id_find{"", "find"};

    void init_map(pups::PackageHandler &handler) {
        handler.add(id_isToplevel, std::make_shared<pups::Function>(is_toplevel));
        handler.add(id_format, std::make_shared<pups::Function>(format));
        handler.add(id_find, std::make_shared<pups::Function>(find));
    }
}
