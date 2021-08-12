//
//  KlangEventListener.hpp
//  Klang – a node+text-based synthesizer library
//
//
//

#ifndef KlangEventListener_hpp
#define KlangEventListener_hpp

#include <stdint.h>

#include <vector>

#include "Klang.hpp"

namespace klang {
    class KlangEventListener {
    public:
        virtual void event(KLANG_EVENT pEventType, std::vector<float> pData) = 0;
    };
}  // namespace strom

#endif /* KlangEventListener_hpp */
