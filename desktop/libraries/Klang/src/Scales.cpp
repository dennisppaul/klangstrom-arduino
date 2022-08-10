/*
 * Klang – a node+text-based synthesizer library
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

#include "Scales.hpp"

using namespace std;
using namespace klang;

const vector<SCALE_TYPE> Scales::CHROMATIC        = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const vector<SCALE_TYPE> Scales::FIFTH            = {0, 7};
const vector<SCALE_TYPE> Scales::MINOR            = {0, 2, 3, 5, 7, 8, 10};
const vector<SCALE_TYPE> Scales::MAJOR            = {0, 2, 4, 5, 7, 9, 11};
const vector<SCALE_TYPE> Scales::MINOR_CHORD      = {0, 3, 7};
const vector<SCALE_TYPE> Scales::MAJOR_CHORD      = {0, 4, 7};
const vector<SCALE_TYPE> Scales::MINOR_CHORD_7    = {0, 3, 7, 11};
const vector<SCALE_TYPE> Scales::MAJOR_CHORD_7    = {0, 4, 7, 11};
const vector<SCALE_TYPE> Scales::MINOR_PENTATONIC = {0, 3, 5, 7, 10};
const vector<SCALE_TYPE> Scales::MAJOR_PENTATONIC = {0, 4, 5, 7, 11};
const vector<SCALE_TYPE> Scales::OCTAVE           = {0};
const vector<SCALE_TYPE> Scales::DIMINISHED       = {0, 3, 6, 9};
