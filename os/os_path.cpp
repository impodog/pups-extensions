//
// Created by impodog on 7/29/2023.
//
#include "init.h"
#include <filesystem>

namespace pups_extension::os {
    pups::ObjectPtr make_dir(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 1) {
            map->throw_error(std::make_shared<pups::library::ArgumentError>("os.make_dir requires one only argument."));
        } else {
            auto ptr = pups::cast<pups::String>(*args.front());
            if (ptr) {
                if (!std::filesystem::create_directory(ptr->data())) {
                    map->throw_error(std::make_shared<OsError>(
                            "os.make_dir failed to create directory \"" + ptr->data() + "\"."));
                }
            } else {
                map->throw_error(std::make_shared<pups::library::TypeError>(
                        "os.make_dir requires a string argument."));
            }
        }
        return pups::pending;
    }

    pups::ObjectPtr remove(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 1) {
            map->throw_error(std::make_shared<pups::library::ArgumentError>("os.remove requires one only argument."));
        } else {
            auto ptr = pups::cast<pups::String>(*args.front());
            if (ptr) {
                if (!std::filesystem::remove(ptr->data())) {
                    map->throw_error(std::make_shared<OsError>(
                            "os.remove failed to remove path \"" + ptr->data() + "\"."));
                }
            } else {
                map->throw_error(std::make_shared<pups::library::TypeError>(
                        "os.remove requires a string argument."));
            }
        }
        return pups::pending;
    }

    pups::ObjectPtr remove_all(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 1) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("os.remove_all requires one only argument."));
        } else {
            auto ptr = pups::cast<pups::String>(*args.front());
            if (ptr) {
                if (!std::filesystem::remove_all(ptr->data())) {
                    map->throw_error(std::make_shared<OsError>(
                            "os.remove_all failed to remove path \"" + ptr->data() + "\"."));
                }
            } else {
                map->throw_error(std::make_shared<pups::library::TypeError>(
                        "os.remove_all requires a string argument."));
            }
        }
        return pups::pending;
    }

    pups::ObjectPtr rename(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 2) {
            map->throw_error(std::make_shared<pups::library::ArgumentError>("os.rename requires two arguments."));
        } else {
            auto first = pups::cast<pups::String>(*args.front());
            auto second = pups::cast<pups::String>(*args.back());
            if (first && second) {
                try {
                    std::filesystem::rename(first->data(), second->data());
                } catch (const std::filesystem::filesystem_error &) {
                    map->throw_error(std::make_shared<OsError>(
                            "os.rename failed to rename \"" + first->data() + "\" to \"" + second->data() + "\"."));
                }
            } else {
                map->throw_error(std::make_shared<pups::library::TypeError>(
                        "os.rename requires two string arguments."));
            }
        }
        return pups::pending;
    }

    pups::ObjectPtr abs_path(pups::FunctionArgs &args, pups::Map *map) {
        if (args.size() != 1) {
            map->throw_error(std::make_shared<pups::library::ArgumentError>("os.abspath requires one only argument."
                                                                            " pending is returned"));
        } else {
            auto ptr = pups::cast<pups::String>(*args.front());
            if (ptr) {
                return std::make_shared<pups::String>(std::filesystem::absolute(ptr->data()).string());
            } else {
                map->throw_error(std::make_shared<pups::library::TypeError>(
                        "os.abs_path requires a string argument. pending is returned"));
            }
        }
        return pups::pending;
    }

    pups::ObjectPtr join_path(pups::FunctionArgs &args, pups::Map *map) {
        if (args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("os.join_path requires at least one argument."
                                                                   " pending is returned"));
        } else {
            std::filesystem::path path;
            for (auto &arg: args) {
                auto ptr = pups::cast<pups::String>(*arg);
                if (ptr) {
                    path /= ptr->data();
                } else {
                    map->throw_error(std::make_shared<pups::library::TypeError>(
                            "os.join_path requires string arguments. Argument \"" + arg->get()->repr() +
                            "\" ignored."));
                }
            }
            return std::make_shared<pups::String>(path.string());
        }
        return pups::pending;
    }

    pups::ObjectPtr my_path(pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("os.my_path requires no arguments."));
        }
        return std::make_shared<pups::String>(map->get_path().string());
    }

    pups::Id id_makeDir{"", "make_dir"}, id_remove{"", "remove"}, id_removeAll{"", "remove_all"},
            id_rename{"", "rename"},
            id_absPath{"", "abs_path"}, id_joinPath{"", "join_path"}, id_myPath{"", "my_path"};

    void init_path(pups::PackageHandler &handler) {
        handler.add(id_makeDir, std::make_shared<pups::Function>(make_dir));
        handler.add(id_remove, std::make_shared<pups::Function>(remove));
        handler.add(id_removeAll, std::make_shared<pups::Function>(remove_all));
        handler.add(id_rename, std::make_shared<pups::Function>(rename));
        handler.add(id_absPath, std::make_shared<pups::Function>(abs_path));
        handler.add(id_joinPath, std::make_shared<pups::Function>(join_path));
        handler.add(id_myPath, std::make_shared<pups::Function>(my_path));
    }
}