/*
 * Klangstrom
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

#ifndef task_scheduler_hpp
#define task_scheduler_hpp

#include <functional>
#include <vector>

using namespace std;

namespace strom {
    class TaskScheduler {
        typedef function<void(void)> callback_function_t;

    public:
        TaskScheduler() : fCurrentTime(0), fTaskID(-1) {}

        void reset(uint32_t pCurrentTime) {
            fCurrentTime = pCurrentTime;
        }

        uint32_t schedule_task(void (*pCallback)(void), uint32_t pPeriod, bool pImmediate = false) {
            const callback_function_t mCallback = pCallback;
            fTaskID++;
            fTasks.push_back(PeriodicTask{pCallback, pPeriod, fTaskID, pImmediate});
            return fTaskID;
        }

        int16_t reset_task(int16_t pID) {
            int16_t mElement = -1;
            for (uint16_t i = 0; i < fTasks.size(); i++) {
                auto &mTask = fTasks[i];
                if (mTask.ID == pID) {
                    mTask.reset();
                    return mElement;
                }
            }
            return mElement;
        }

        int16_t remove_task(int16_t pID) {
            int16_t mElement = -1;
            for (uint16_t i = 0; i < fTasks.size(); i++) {
                auto &mTask = fTasks[i];
                if (mTask.ID == pID) {
                    mElement = i;
                    break;
                }
            }
            if (mElement >= 0) {
                fTasks.erase(fTasks.begin() + mElement);
            }
            return mElement;
        }

        int16_t set_period(int16_t pID, uint32_t pPeriod, bool pImmediate = false) {
            int16_t mElement = -1;
            for (uint16_t i = 0; i < fTasks.size(); i++) {
                auto &mTask = fTasks[i];
                if (mTask.ID == pID) {
                    mTask.set_period(pPeriod, pImmediate);
                    return mElement;
                }
            }
            return mElement;
        }

        void update(uint32_t pTime) {
            const uint32_t mDelta = pTime - fCurrentTime;
            for (auto &mTask : fTasks) {
                mTask.duration += mDelta;
                if (mTask.duration >= mTask.period) {
                    mTask.duration -= mTask.period;
                    mTask.callback_function();
                }
            }
            fCurrentTime = pTime;
        }

    private:
        class PeriodicTask {
        public:
            PeriodicTask(void (*pCallback)(void),
                         uint32_t pPeriod,
                         int16_t  pID,
                         bool     pImmediate = false) : callback_function(pCallback),
                                                    period(pPeriod),
                                                    duration(pImmediate ? pPeriod : 0),
                                                    ID(pID) {}
            void reset() {
                duration = 0;
            }

            void set_period(uint32_t pPeriod, bool pImmediate = false) {
                period = pPeriod;
                if (pImmediate) {
                    duration = pPeriod;
                }
            }

            callback_function_t callback_function;
            uint32_t            period;
            uint32_t            duration;
            int16_t             ID;
        };

        vector<PeriodicTask> fTasks;
        uint32_t             fCurrentTime;
        int16_t              fTaskID;

        // https://stackoverflow.com/questions/2298242/callback-functions-in-c
        // https://www.arduino.cc/reference/en/language/functions/time/micros/
    };
}  // namespace strom

#endif  // task_scheduler_hpp