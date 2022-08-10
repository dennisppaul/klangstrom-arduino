/*
 * Strom – a node+text-based composition library
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2022 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
