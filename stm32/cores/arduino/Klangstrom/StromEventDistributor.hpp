//
//  StromEventDistributor.hpp
//  Strom – a node+text-based library
//
//
//

#ifndef StromEventDistributor_hpp
#define StromEventDistributor_hpp

#include <stdint.h>

#include <vector>

#include "Strom.hpp"
#include "StromEventListener.hpp"

namespace strom {
    class StromEventDistributor {
    public:
        void add_event_listener(StromEventListener* pEventListener) {
            mEventListeners.push_back(pEventListener);
        }

        void clear_listeners() {
            mEventListeners.clear();
        }

        void fire_event(STROM_EVENT pEventType, std::vector<float> pData) {
            for (uint8_t i = 0; i < mEventListeners.size(); i++) {
                if (mEventListeners[i] != nullptr) {
                    mEventListeners[i]->event(pEventType, pData);
                }
            }
        }
    private: 
        std::vector<StromEventListener*> mEventListeners;
    };
}  // namespace strom

#endif /* StromEventDistributor_hpp */
