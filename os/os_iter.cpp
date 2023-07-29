//
// Created by impodog on 7/29/2023.
//
#include "init.h"
#include <filesystem>
#include <functional>
#include <memory>
#include <ranges>

namespace pups_extension::os {
    template<bool is_recursive>
    struct IterType;

    template<>
    struct IterType<true> {
        using type = recursive_directory_iterator;
    };

    template<>
    struct IterType<false> {
        using type = directory_iterator;
    };
    template<bool is_recursive>
    using IterType_t = typename IterType<is_recursive>::type;

    struct Iter {
        const bool is_recursive;
        IterType_t<true> recursive, recur_current;
        IterType_t<false> non_recursive, non_recur_current;

        Iter(const path &path, bool is_recursive) : is_recursive(is_recursive) {
            if (is_recursive) {
                recursive = IterType_t<true>(path);
            } else {
                non_recursive = IterType_t<false>(path);
            }
        }
    };

    class DirIter : public pups::HasMethods {
    public:
        Iter iter;

        DirIter(const path &path, bool is_recursive) : iter(path, is_recursive) {

        }

        pups::ObjectPtr put(pups::ObjectPtr &object, pups::Map *map) override {
            map->throw_error(std::make_shared<pups::library::TypeError>("os.DirIter object does not support putting."));
            return nullptr;
        }

        pups::FunctionCore get_method(const pups::Id &name) override;
    };

    template<bool is_recursive>
    class DirIterInit : public pups::Function {
    public:
        DirIterInit() : Function([](pups::FunctionArgs &args, pups::Map *map) -> pups::ObjectPtr {
            if (args.size() != 1) {
                map->throw_error(
                        std::make_shared<pups::library::ArgumentError>("os.DirIter requires one only argument."
                                                                       " pending is returned."));
            } else {
                auto ptr = pups::cast<pups::String>(*args.front());
                if (ptr) {
                    return pups::ObjectPtr(new DirIter(ptr->data(), is_recursive));
                } else {
                    map->throw_error(std::make_shared<pups::library::TypeError>(
                            "os.DirIter requires a string argument. pending is returned."));
                }
            }
            return pups::pending;
        }) {}
    };

    pups::ObjectPtr iter_beg(DirIter &iter, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(std::make_shared<pups::library::ArgumentError>("os.DirIter.beg takes no arguments."));
        }
        if (iter.iter.is_recursive) {
            iter.iter.recur_current = begin(iter.iter.recursive);
            return std::make_shared<DirEntry>(*iter.iter.recur_current);
        } else {
            iter.iter.non_recur_current = begin(iter.iter.non_recursive);
            return std::make_shared<DirEntry>(*iter.iter.non_recur_current);
        }
    }

    pups::ObjectPtr iter_next(DirIter &iter, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(std::make_shared<pups::library::ArgumentError>("os.DirIter.next takes no arguments."));
        }
        if (iter.iter.is_recursive) {
            ++iter.iter.recur_current;
            if (iter.iter.recur_current == end(iter.iter.recursive))
                return pups::pending;
            return std::make_shared<DirEntry>((*iter.iter.recur_current));
        } else {
            ++iter.iter.non_recur_current;
            if (iter.iter.non_recur_current == end(iter.iter.non_recursive))
                return pups::pending;
            return std::make_shared<DirEntry>((*iter.iter.non_recur_current));
        }
    }

    pups::ObjectPtr iter_cond(DirIter &iter, pups::FunctionArgs &args, pups::Map *map) {
        if (!args.empty()) {
            map->throw_error(std::make_shared<pups::library::ArgumentError>("os.DirIter.cond takes no arguments."));
        }
        if (iter.iter.is_recursive) {
            return iter.iter.recur_current == end(iter.iter.recursive) ? pups::False : pups::True;
        } else {
            return iter.iter.non_recur_current == end(iter.iter.non_recursive) ? pups::False : pups::True;
        }
    }

    using DirIterFunction = std::function<pups::ObjectPtr(DirIter &iter, pups::FunctionArgs &args, pups::Map *map)>;
    using DirIterFunctionMap = pups::library::IdMap<DirIterFunction>;
    DirIterFunctionMap dir_iter_functions = {
            {pups::Id{"", "beg"}, iter_beg},
            {pups::Id{"", "next"}, iter_next},
            {pups::Id{"", "cond"}, iter_cond}
    };

    pups::FunctionCore DirIter::get_method(const pups::Id &name) {
        const auto &func = dir_iter_functions.at(name);
        return [this, &func](pups::FunctionArgs &args, pups::Map *map) -> pups::ObjectPtr {
            return func(*this, args, map);
        };
    }

    pups::Id id_dirIterInit_Recursive{"", "DirRecurIter"}, id_dirIterInit{"", "DirIter"};

    void init_iter(pups::PackageHandler &handler) {
        handler.add(id_dirIterInit_Recursive, std::make_shared<DirIterInit<true>>());
        handler.add(id_dirIterInit, std::make_shared<DirIterInit<false>>());
    }
}