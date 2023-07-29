//
// Created by impodog on 7/29/2023.
//
#include "init.h"
#include <filesystem>
#include <functional>

namespace pups_extension::os {

    class DirEntryInit : public pups::Function {
    public:
        DirEntryInit() : Function([](pups::FunctionArgs &args, pups::Map *map) -> pups::ObjectPtr {
            if (args.size() != 1) {
                map->throw_error(
                        std::make_shared<pups::library::ArgumentError>("os.DirEntry requires one only argument."
                                                                       " pending is returned."));
            } else {
                auto ptr = pups::cast<pups::String>(*args.front());
                if (ptr) {
                    return pups::ObjectPtr(new DirEntry(ptr->data()));
                } else {
                    map->throw_error(std::make_shared<pups::library::TypeError>(
                            "os.DirEntry requires a string argument. pending is returned."));
                }
            }
            return pups::pending;
        }) {}
    };

    pups::ObjectPtr get(DirEntry &entry, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(std::make_shared<pups::library::ArgumentError>("os.DirEntry requires no arguments."));
        }
        return std::make_shared<pups::String>(entry.get());
    }

    pups::ObjectPtr exists(DirEntry &entry, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("os.DirEntry.exists requires no arguments."));
        }
        try {
            return entry.entry.exists() ? pups::True : pups::False;
        } catch (...) {
            map->throw_error(std::make_shared<OsError>("os.DirEntry.exists operation failed."));
        }
        return pups::pending;
    }

    pups::ObjectPtr is_regular(DirEntry &entry, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("os.DirEntry.is_regular requires no arguments."));
        }
        try {
            return entry.entry.is_regular_file() ? pups::True : pups::False;
        } catch (...) {
            map->throw_error(std::make_shared<OsError>("os.DirEntry.is_regular operation failed."));
        }
        return pups::pending;
    }

    pups::ObjectPtr is_character(DirEntry &entry, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("os.DirEntry.is_character requires no arguments."));
        }
        try {
            return entry.entry.is_character_file() ? pups::True : pups::False;
        } catch (...) {
            map->throw_error(std::make_shared<OsError>("os.DirEntry.is_character operation failed."));
        }
        return pups::pending;
    }

    pups::ObjectPtr is_block(DirEntry &entry, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("os.DirEntry.is_block requires no arguments."));
        }
        try {
            return entry.entry.is_block_file() ? pups::True : pups::False;
        } catch (...) {
            map->throw_error(std::make_shared<OsError>("os.DirEntry.is_block operation failed."));
        }
        return pups::pending;
    }

    pups::ObjectPtr is_pipe(DirEntry &entry, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("os.DirEntry.is_pipe requires no arguments."));
        }
        try {
            return entry.entry.is_fifo() ? pups::True : pups::False;
        } catch (...) {
            map->throw_error(std::make_shared<OsError>("os.DirEntry.is_pipe operation failed."));
        }
        return pups::pending;
    }

    pups::ObjectPtr is_socket(DirEntry &entry, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("os.DirEntry.is_socket requires no arguments."));
        }
        try {
            return entry.entry.is_socket() ? pups::True : pups::False;
        } catch (...) {
            map->throw_error(std::make_shared<OsError>("os.DirEntry.is_socket operation failed."));
        }
        return pups::pending;
    }

    pups::ObjectPtr is_symlink(DirEntry &entry, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("os.DirEntry.is_symlink requires no arguments."));
        }
        try {
            return entry.entry.is_symlink() ? pups::True : pups::False;
        } catch (...) {
            map->throw_error(std::make_shared<OsError>("os.DirEntry.is_symlink operation failed."));
        }
        return pups::pending;
    }

    pups::ObjectPtr is_dir(DirEntry &entry, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>("os.DirEntry.is_dir requires no arguments."));
        }
        try {
            return entry.entry.is_directory() ? pups::True : pups::False;
        } catch (...) {
            map->throw_error(std::make_shared<OsError>("os.DirEntry.is_dir operation failed."));
        }
        return pups::pending;
    }

    pups::ObjectPtr get_size(DirEntry &entry, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(
                    std::make_shared<pups::library::ArgumentError>(
                            "os.DirEntry.get_size requires no arguments."));
        }
        try {
            return std::make_shared<pups::Int>(entry.entry.file_size());
        } catch (...) {
            map->throw_error(std::make_shared<OsError>("os.DirEntry.get_size operation failed."));
        }
        return pups::pending;
    }

    using DirEntryFunction = std::function<pups::ObjectPtr(DirEntry &entry, pups::FunctionArgs &args, pups::Map *map)>;
    using DirEntryFunctionMap = pups::library::IdMap<DirEntryFunction>;
    pups::Id id_entry_get{"", "get"}, id_entry_exists{"", "exists"}, id_entry_is_regular{"", "is_regular"},
            id_entry_is_character{"", "is_character"}, id_entry_is_block{"", "is_block"},
            id_entry_is_pipe{"", "is_pipe"}, id_entry_is_socket{"", "is_socket"},
            id_entry_is_symlink{"", "is_symlink"}, id_entry_is_dir{"", "is_dir"},
            id_entry_get_size{"", "get_size"};
    const DirEntryFunctionMap dir_entry_functions = {
            {id_entry_get,            get},
            {id_entry_exists,         exists},
            {id_entry_is_regular,     is_regular},
            {pups::Id{"", "is_file"}, is_regular},
            {id_entry_is_character,   is_character},
            {id_entry_is_block,       is_block},
            {id_entry_is_pipe,        is_pipe},
            {id_entry_is_socket,      is_socket},
            {id_entry_is_symlink,     is_symlink},
            {id_entry_is_dir,         is_dir},
            {id_entry_get_size,       get_size}
    };

    DirEntry::DirEntry(const path &path) : entry(path) {

    }

    DirEntry::DirEntry(directory_entry entry) : entry(std::move(entry)) {

    }

    pups::ObjectPtr DirEntry::put(pups::ObjectPtr &object, pups::Map *map) {
        map->throw_error(
                std::make_shared<pups::library::TypeError>("os.DirEntry object does not support putting."));
        return nullptr;
    }

    pups::FunctionCore DirEntry::get_method(const pups::Id &name) {
        const auto &func = dir_entry_functions.at(name);
        return [this, &func](pups::FunctionArgs &args, pups::Map *map) {
            return func(*this, args, map);
        };
    }

    std::string DirEntry::get() const noexcept {
        return entry.path().string();
    }

    pups::Id id_dirEntryInit{"", "DirEntry"};

    void init_entry(pups::PackageHandler &handler) {
        handler.add(id_dirEntryInit, std::make_shared<DirEntryInit>());
    }
}