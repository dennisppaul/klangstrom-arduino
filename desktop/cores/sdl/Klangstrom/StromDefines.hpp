//
//  StromDefines.hpp
//  Strom – a node+text-based library
//
//
//

#ifndef StromDefines_hpp
#define StromDefines_hpp

#include <limits>

#define TRUE     1
#define FALSE    0
#define STROM_NL "\n"

#ifndef STROM_DEBUG_LEVEL
#define STROM_DEBUG_LEVEL 2
#endif

#if (STROM_DEBUG_LEVEL == 0)
#define STROM_LOG_ERR(...)
#define STROM_LOG(...)
#define STROM_LOG_SIM(...)
#elif (STROM_DEBUG_LEVEL == 1)
#define STROM_LOG_ERR(...) \
    printf(__VA_ARGS__);   \
    printf(STROM_NL);
#define STROM_LOG(...)
#define STROM_LOG_SIM(...)
#elif (STROM_DEBUG_LEVEL == 2)
#define STROM_LOG_ERR(...) \
    printf(__VA_ARGS__);   \
    printf(STROM_NL);
#define STROM_LOG(...)   \
    printf(__VA_ARGS__); \
    printf(STROM_NL);
#define STROM_LOG_SIM(...)
#elif (STROM_DEBUG_LEVEL >= 3)
#define STROM_LOG_ERR(...) \
    printf(__VA_ARGS__);   \
    printf(STROM_NL);
#define STROM_LOG(...)   \
    printf(__VA_ARGS__); \
    printf(STROM_NL);
#define STROM_LOG_SIM(...) \
    printf(__VA_ARGS__);   \
    printf(STROM_NL);
#endif

namespace strom {
    typedef float                  STROM_SIGNAL_TYPE;
    typedef int16_t                STROM_NODE_ID;
    typedef int16_t                STROM_CHANNEL_ID;
    const static STROM_SIGNAL_TYPE STROM_SIGNAL_MIN = std::numeric_limits<STROM_SIGNAL_TYPE>::min();
    const static STROM_SIGNAL_TYPE STROM_SIGNAL_MAX = std::numeric_limits<STROM_SIGNAL_TYPE>::max();
    const static STROM_SIGNAL_TYPE STROM_NO_VALUE   = std::numeric_limits<STROM_SIGNAL_TYPE>::infinity();

    enum STROM_EVENT : uint8_t {
        RESERVED_00,
        RESERVED_01,
        NOTE_ON,
        NOTE_OFF,
    };
};  // namespace strom

#endif /* StromDefines_hpp */
