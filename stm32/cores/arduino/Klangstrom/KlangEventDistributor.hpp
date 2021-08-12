//
//  KlangEventDistributor.hpp
//  Klang – a node+text-based library
//
//
//

#ifndef KlangEventDistributor_hpp
#define KlangEventDistributor_hpp

#include <stdint.h>

#include <vector>

#include "Klang.hpp"
#include "KlangEventListener.hpp"

namespace klang {
    class KlangEventDistributor {
    public:
        void add_event_listener(KlangEventListener* pEventListener) {
            mEventListeners.push_back(pEventListener);
        }

        void clear_listeners() {
            mEventListeners.clear();
        }

        void fire_event(KLANG_EVENT pEventType, std::vector<float> pData) {
            for (uint8_t i = 0; i < mEventListeners.size(); i++) {
                if (mEventListeners[i] != nullptr) {
                    mEventListeners[i]->event(pEventType, pData);
                }
            }
        }

    private:
        std::vector<KlangEventListener*> mEventListeners;
    };
}  // namespace klang

#endif /* KlangEventDistributor_hpp */
