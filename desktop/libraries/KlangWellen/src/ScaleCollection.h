/*
 * KlangWellen
 *
 * This file is part of the *KlangWellen* library (https://github.com/dennisppaul/klangwellen).
 * Copyright (c) 2023 Dennis P Paul
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

#pragma once

#include "Scale.h"

namespace klangwellen {
    class ScaleCollection {
    public:
        static inline const Scale CHROMATIC{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        static inline const Scale FIFTH{0, 7};
        static inline const Scale MINOR{0, 2, 3, 5, 7, 8, 10};
        static inline const Scale MAJOR{0, 2, 4, 5, 7, 9, 11};
        static inline const Scale MINOR_CHORD{0, 3, 7};
        static inline const Scale MAJOR_CHORD{0, 4, 7};
        static inline const Scale MINOR_CHORD_7{0, 3, 7, 11};
        static inline const Scale MAJOR_CHORD_7{0, 4, 7, 11};
        static inline const Scale MINOR_PENTATONIC{0, 3, 5, 7, 10};
        static inline const Scale MAJOR_PENTATONIC{0, 4, 5, 7, 11};
        static inline const Scale OCTAVE{0};
        static inline const Scale DIMINISHED{0, 3, 6, 9};
    };
}  // namespace klangwellen
