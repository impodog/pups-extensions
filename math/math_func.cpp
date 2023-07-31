//
// Created by impodog on 7/31/2023.
//
#include "init.h"

namespace pups_extension::math {
    template<typename Operator, typename Arithmetic>
    concept IsUnary = requires {
        { Operator::template work<Arithmetic>(std::declval<Arithmetic>()) } -> std::same_as<Arithmetic>;
    } && std::is_arithmetic_v<Arithmetic>;

    template<typename Operator, typename Arithmetic>
    concept IsBinary = requires {
        {
        Operator::template work<Arithmetic>(std::declval<Arithmetic>(), std::declval<Arithmetic>())
        } -> std::same_as<Arithmetic>;
    } && std::is_arithmetic_v<Arithmetic>;

    template<typename Operator>
    struct MathFunction {
        template<typename Arithmetic>
        requires IsUnary<Operator, Arithmetic>
        static Arithmetic work(Arithmetic x) {
            return Operator::template work<Arithmetic>(x);
        }

        template<typename Arithmetic>
        requires IsBinary<Operator, Arithmetic>
        static Arithmetic work(Arithmetic x, Arithmetic y) {
            return Operator::template work<Arithmetic>(x, y);
        }

        template<typename Arithmetic>
        requires IsUnary<Operator, Arithmetic>
        static pups::ObjectPtr function(pups::FunctionArgs &args, pups::Map *map) {
            if (args.size() != 1) {
                map->throw_error(std::make_shared<pups::library::ArgumentError>(
                        std::string("math.") + Operator::name + " requires one only argument."
                                                                " pending is returned."));
            } else {
                auto ptr = pups::cast<pups::Number<Arithmetic>>(*args.front());
                if (ptr) {
                    return std::make_shared<pups::Number<Arithmetic>>(work<Arithmetic>(ptr->value));
                } else {
                    map->throw_error(std::make_shared<pups::library::TypeError>(
                            std::string("math.") + Operator::name + " requires a " +
                            pups::library::builtins::numbers::type_name_of_arith<Arithmetic>() +
                            " instead of " + args.front()->get()->str() +
                            +". pending is returned."));
                }
            }
            return pups::pending;
        }

        template<typename Arithmetic>
        requires IsBinary<Operator, Arithmetic>
        static pups::ObjectPtr function(pups::FunctionArgs &args, pups::Map *map) {
            if (args.size() != 2) {
                map->throw_error(std::make_shared<pups::library::ArgumentError>(
                        std::string("math.") + Operator::name + " requires two arguments."
                                                                " pending is returned."));
            } else {
                auto first = pups::cast<pups::Number<Arithmetic>>(*args.front());
                auto second = pups::cast<pups::Number<Arithmetic>>(*args.back());
                if (first && second) {
                    return std::make_shared<pups::Number<Arithmetic>>(work<Arithmetic>(first->value, second->value));
                } else {
                    map->throw_error(std::make_shared<pups::library::TypeError>(
                            std::string("math.") + Operator::name + " requires two " +
                            pups::library::builtins::numbers::type_name_of_arith<Arithmetic>() +
                            " instead of " + args.front()->get()->str() + " and " + args.back()->get()->str() +
                            +" arguments. pending is returned."));
                }
            }
            return pups::pending;
        }
    };

    struct SinType {
        template<typename Arithmetic>
        static Arithmetic work(Arithmetic x) {
            return std::sin(x);
        }

        static constexpr const char *name = "sin";
    };

    struct CosType {
        template<typename Arithmetic>
        static Arithmetic work(Arithmetic x) {
            return std::cos(x);
        }

        static constexpr const char *name = "cos";
    };

    struct TanType {
        template<typename Arithmetic>
        static Arithmetic work(Arithmetic x) {
            return std::tan(x);
        }

        static constexpr const char *name = "tan";
    };

    struct AsinType {
        template<typename Arithmetic>
        static Arithmetic work(Arithmetic x) {
            return std::asin(x);
        }

        static constexpr const char *name = "asin";
    };

    struct AcosType {
        template<typename Arithmetic>
        static Arithmetic work(Arithmetic x) {
            return std::acos(x);
        }

        static constexpr const char *name = "acos";
    };

    struct AtanType {
        template<typename Arithmetic>
        static Arithmetic work(Arithmetic x) {
            return std::atan(x);
        }

        static constexpr const char *name = "atan";
    };

    struct Atan2Type {
        template<typename Arithmetic>
        static Arithmetic work(Arithmetic a, Arithmetic b) {
            return std::atan2(a, b);
        }

        static constexpr const char *name = "atan2";
    };

    struct AbsType {
        template<typename Arithmetic>
        static Arithmetic work(Arithmetic x) {
            return std::abs(x);
        }

        static constexpr const char *name = "abs";
    };

    struct CeilType {
        template<typename Arithmetic>
        static Arithmetic work(Arithmetic x) {
            return std::ceil(x);
        }

        static constexpr const char *name = "ceil";
    };

    struct FloorType {
        template<typename Arithmetic>
        static Arithmetic work(Arithmetic x) {
            return std::floor(x);
        }

        static constexpr const char *name = "floor";
    };

    struct RoundType {
        template<typename Arithmetic>
        static Arithmetic work(Arithmetic x) {
            return std::round(x);
        }

        static constexpr const char *name = "round";
    };

    pups::Id id_sin{"", "sin"}, id_cos{"", "cos"}, id_tan{"", "tan"},
            id_asin{"", "asin"}, id_acos{"", "acos"}, id_atan{"", "atan"}, id_atan2{"", "atan2"},
            id_abs{"", "abs"}, id_fabs{"", "fabs"}, id_ceil{"", "ceil"}, id_floor{"", "floor"},
            id_round{"", "round"};

    void init_func(pups::PackageHandler &handler) {
        handler.add(id_sin, std::make_shared<pups::Function>(MathFunction<SinType>::function<pups_float>));
        handler.add(id_cos, std::make_shared<pups::Function>(MathFunction<CosType>::function<pups_float>));
        handler.add(id_tan, std::make_shared<pups::Function>(MathFunction<TanType>::function<pups_float>));
        handler.add(id_asin, std::make_shared<pups::Function>(MathFunction<AsinType>::function<pups_float>));
        handler.add(id_acos, std::make_shared<pups::Function>(MathFunction<AcosType>::function<pups_float>));
        handler.add(id_atan, std::make_shared<pups::Function>(MathFunction<AtanType>::function<pups_float>));
        handler.add(id_atan2, std::make_shared<pups::Function>(MathFunction<Atan2Type>::function<pups_float>));
        handler.add(id_abs, std::make_shared<pups::Function>(MathFunction<AbsType>::function<pups_int>));
        handler.add(id_fabs, std::make_shared<pups::Function>(MathFunction<AbsType>::function<pups_float>));
        handler.add(id_ceil, std::make_shared<pups::Function>(MathFunction<CeilType>::function<pups_float>));
        handler.add(id_floor, std::make_shared<pups::Function>(MathFunction<FloorType>::function<pups_float>));
        handler.add(id_round, std::make_shared<pups::Function>(MathFunction<RoundType>::function<pups_float>));
    }
}