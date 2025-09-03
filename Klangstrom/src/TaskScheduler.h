/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2023 Dennis P Paul.
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

#ifndef _TASK_SCHEDULER_H_
#define _TASK_SCHEDULER_H_

#include <vector>

using namespace std;

namespace klangstrom {
    class TaskScheduler {
        typedef void (*TaskCallbackType)(void);

    public:
        TaskScheduler() {
            fInterrupt = false;
        }

        ~TaskScheduler() {}

        void schedule_priority_task(TaskCallbackType callback) {
            fPriorityTasks.push_back(callback);
        }

        void remove_priority_task(TaskCallbackType callback) {
            fPriorityTasks.erase(remove(fPriorityTasks.begin(), fPriorityTasks.end(), callback), fPriorityTasks.end());
        }

        void schedule_task(TaskCallbackType callback) {
            fTasks.push_back(callback);
        }

        void remove_task(TaskCallbackType callback) {
            fTasks.erase(remove(fTasks.begin(), fTasks.end(), callback), fTasks.end());
        }

        void schedule_repeated_task(TaskCallbackType callback) {
            fRepeatedTasks.push_back(callback);
        }

        void remove_repeated_task(TaskCallbackType callback) {
            fRepeatedTasks.erase(remove(fRepeatedTasks.begin(), fRepeatedTasks.end(), callback), fRepeatedTasks.end());
        }

        void process_priority_tasks() {
            for (auto it = fPriorityTasks.begin(); it != fPriorityTasks.end();) {
                (*it)();
                it = fPriorityTasks.erase(it);
            }
        }

        bool process_tasks() {
            for (auto it = fTasks.begin(); it != fTasks.end();) {
                if (fInterrupt) {
                    return false;
                }
                it = fTasks.erase(it);
                (*it)();
            }
            return true;
        }

        bool process_repeated_tasks() {
            for (auto it = fRepeatedTasks.begin(); it != fRepeatedTasks.end();) {
                if (fInterrupt) {
                    return false;
                }
                (*it)();
            }
            return true;
        }

        bool process() {
            // priority tasks ( uninterrupted, removed after execution )
            process_priority_tasks();
            // tasks ( interruption possible after each task, remove task after execution )
            bool mTasksFinished = process_tasks();
            // repeated tasks ( interruption possible after each task )
            bool mRepeatedTasksFinished = process_repeated_tasks();
            // interruption: either by flag or by scheduled priority task
            fInterrupt = false;
            // returns true if all tasks are finished
            return mTasksFinished && mRepeatedTasksFinished;
        }

        void schedule_interrupt() {
            fInterrupt = true;
        }

    private:
        vector<TaskCallbackType> fPriorityTasks;
        vector<TaskCallbackType> fTasks;
        vector<TaskCallbackType> fRepeatedTasks;
        bool                     fInterrupt;
    };
} // namespace klangstrom

#endif