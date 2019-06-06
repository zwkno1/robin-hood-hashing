#include <robin_hood.h>

#include <app/CtorDtorVerifier.h>
#include <app/checksum.h>
#include <app/doctest.h>
#include <app/sfc64.h>

#include <iostream>

TYPE_TO_STRING(robin_hood::unordered_flat_map<CtorDtorVerifier, CtorDtorVerifier>);
TYPE_TO_STRING(robin_hood::unordered_node_map<CtorDtorVerifier, CtorDtorVerifier>);

template <typename M>
void testMapsOfMaps() {

    sfc64 rng;
    for (size_t trial = 0; trial < 4; ++trial) {
        {
            M maps;
            for (size_t i = 0; i < 100; ++i) {
                auto a = rng.uniform<uint64_t>(20);
                auto b = rng.uniform<uint64_t>(20);
                auto x = rng();
                // std::cout << i << ": map[" << a << "][" << b << "] = " << x << std::endl;
                maps[a][b] = x;
            }

            M mapsCopied;
            mapsCopied = maps;
            REQUIRE(checksum::mapmap(mapsCopied) == checksum::mapmap(maps));
            REQUIRE(mapsCopied == maps);

            // TODO
            // M mapsMoved
        }
        REQUIRE(CtorDtorVerifier::mapSize() == static_cast<size_t>(0));
    }
}

TEST_CASE_TEMPLATE("mapmap", Map,
                   robin_hood::unordered_node_map<CtorDtorVerifier, CtorDtorVerifier>,
                   robin_hood::unordered_flat_map<CtorDtorVerifier, CtorDtorVerifier>) {

    testMapsOfMaps<robin_hood::unordered_flat_map<CtorDtorVerifier, Map>>();
    testMapsOfMaps<robin_hood::unordered_node_map<CtorDtorVerifier, Map>>();
}
