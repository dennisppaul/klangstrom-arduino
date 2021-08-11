//
//  StromEventListener.hpp
//  Strom – a node+text-based library
//
//
//

#ifndef StromEventListener_hpp
#define StromEventListener_hpp

#include <stdint.h>

#include <vector>

#include "Strom.hpp"

namespace strom {
    class StromEventListener {
    public:
        virtual void event(STROM_EVENT pEventType, std::vector<float> pData) = 0;
    };
}  // namespace strom

#endif /* StromEventListener_hpp */
