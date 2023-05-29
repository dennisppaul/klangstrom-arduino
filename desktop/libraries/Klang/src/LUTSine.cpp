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

#include "LUTSine.hpp"

using namespace klang;

const float LUTSine::WAVETABLE[WAVETABLE_LENGTH + 1] = {0.0, 0.0030679568, 0.0061358847, 0.009203754, 0.012271538, 0.015339207, 0.01840673, 0.021474082, 0.024541229, 0.027608145, 0.030674804, 0.033741176, 0.036807224, 0.03987293, 0.04293826, 0.04600318, 0.049067676, 0.05213171, 0.055195246, 0.058258265, 0.06132074, 0.064382635, 0.06744392, 0.070504576, 0.07356457, 0.076623864, 0.07968244, 0.08274027, 0.08579732, 0.08885355, 0.091908954, 0.0949635, 0.09801714, 0.10106987, 0.10412164, 0.10717243, 0.110222206, 0.11327095, 0.116318636, 0.119365215, 0.12241068, 0.12545498, 0.12849812, 0.13154003, 0.13458072, 0.13762012, 0.14065824, 0.14369504, 0.14673047, 0.14976454, 0.15279719, 0.15582842, 0.15885815, 0.1618864, 0.16491313, 0.16793829, 0.1709619, 0.17398387, 0.17700422, 0.18002291, 0.18303989, 0.18605515, 0.18906866, 0.19208041, 0.19509032, 0.1980984, 0.20110464, 0.20410897, 0.20711139, 0.21011184, 0.21311033, 0.2161068, 0.21910124, 0.22209363, 0.22508392, 0.22807209, 0.23105812, 0.23404196, 0.2370236, 0.24000302, 0.2429802, 0.24595505, 0.24892761, 0.2518978, 0.25486568, 0.2578311, 0.26079413, 0.2637547, 0.26671278, 0.2696683, 0.27262136, 0.27557182, 0.2785197, 0.28146493, 0.28440756, 0.28734747, 0.29028466, 0.29321918, 0.2961509, 0.29907984, 0.30200595, 0.30492926, 0.30784968, 0.31076714, 0.31368175, 0.3165934, 0.31950203, 0.3224077, 0.32531032, 0.32820985, 0.3311063, 0.33399966, 0.33688986, 0.33977687, 0.34266073, 0.34554133, 0.34841868, 0.35129276, 0.35416353, 0.357031, 0.35989505, 0.36275575, 0.365613, 0.36846682, 0.3713172, 0.37416407, 0.37700742, 0.3798472, 0.38268346, 0.38551608, 0.38834503, 0.39117038, 0.39399207, 0.39681, 0.3996242, 0.40243468, 0.40524134, 0.40804416, 0.4108432, 0.41363832, 0.41642958, 0.4192169, 0.4220003, 0.4247797, 0.42755508, 0.4303265, 0.43309385, 0.43585712, 0.43861625, 0.4413713, 0.44412217, 0.44686884, 0.44961134, 0.4523496, 0.45508358, 0.45781332, 0.46053872, 0.46325982, 0.4659765, 0.46868885, 0.47139674, 0.4741002, 0.47679925, 0.47949374, 0.48218375, 0.48486924, 0.48755017, 0.4902265, 0.49289823, 0.4955653, 0.49822766, 0.50088537, 0.50353837, 0.50618666, 0.5088302, 0.5114689, 0.51410276, 0.5167318, 0.519356, 0.5219753, 0.5245897, 0.52719915, 0.52980363, 0.5324032, 0.53499764, 0.5375871, 0.5401715, 0.5427508, 0.545325, 0.54789406, 0.550458, 0.5530167, 0.55557024, 0.5581185, 0.5606616, 0.56319934, 0.5657318, 0.568259, 0.57078075, 0.57329714, 0.5758082, 0.5783138, 0.580814, 0.5833087, 0.5857979, 0.5882816, 0.5907597, 0.59323233, 0.5956993, 0.59816074, 0.6006165, 0.6030666, 0.605511, 0.6079498, 0.6103828, 0.6128101, 0.61523163, 0.61764735, 0.6200572, 0.62246126, 0.6248595, 0.6272518, 0.62963825, 0.63201874, 0.63439333, 0.63676184, 0.63912445, 0.64148104, 0.64383155, 0.64617604, 0.64851445, 0.6508467, 0.65317285, 0.65549284, 0.6578067, 0.66011435, 0.6624158, 0.664711, 0.66699994, 0.66928256, 0.671559, 0.673829, 0.67609274, 0.6783501, 0.680601, 0.6828456, 0.6850837, 0.68731534, 0.68954057, 0.6917593, 0.69397146, 0.6961772, 0.6983763, 0.7005688, 0.70275474, 0.70493406, 0.70710677, 0.70927286, 0.7114322, 0.7135849, 0.71573085, 0.71787006, 0.72000253, 0.7221282, 0.7242471, 0.7263592, 0.7284644, 0.73056275, 0.7326543, 0.7347389, 0.7368166, 0.73888737, 0.7409512, 0.74300796, 0.74505776, 0.7471006, 0.7491364, 0.75116515, 0.7531868, 0.7552014, 0.7572089, 0.7592092, 0.7612024, 0.7631884, 0.7651673, 0.76713896, 0.76910335, 0.7710606, 0.77301043, 0.7749531, 0.7768885, 0.7788165, 0.7807373, 0.78265065, 0.7845566, 0.7864552, 0.7883464, 0.7902302, 0.79210657, 0.7939755, 0.7958369, 0.79769087, 0.7995373, 0.80137616, 0.8032075, 0.80503136, 0.8068476, 0.8086562, 0.81045717, 0.8122506, 0.8140363, 0.81581444, 0.8175848, 0.81934756, 0.82110256, 0.8228498, 0.8245893, 0.82632107, 0.82804507, 0.82976127, 0.83146966, 0.8331702, 0.8348629, 0.83654773, 0.8382247, 0.8398938, 0.84155494, 0.84320825, 0.8448536, 0.8464909, 0.84812033, 0.84974176, 0.8513552, 0.85296065, 0.854558, 0.85614735, 0.85772866, 0.85930187, 0.86086696, 0.862424, 0.86397284, 0.8655136, 0.86704624, 0.8685707, 0.87008697, 0.8715951, 0.873095, 0.87458664, 0.87607014, 0.8775453, 0.8790123, 0.88047093, 0.8819213, 0.8833633, 0.8847971, 0.88622254, 0.88763964, 0.88904834, 0.89044875, 0.8918407, 0.8932243, 0.8945995, 0.8959663, 0.8973246, 0.8986745, 0.90001595, 0.9013489, 0.9026733, 0.9039893, 0.90529674, 0.9065957, 0.90788615, 0.909168, 0.9104413, 0.91170603, 0.9129622, 0.9142098, 0.9154487, 0.9166791, 0.9179008, 0.9191139, 0.92031825, 0.92151403, 0.9227011, 0.9238795, 0.92504925, 0.9262102, 0.92736256, 0.9285061, 0.9296409, 0.930767, 0.9318843, 0.9329928, 0.9340926, 0.9351835, 0.93626565, 0.937339, 0.93840355, 0.9394592, 0.9405061, 0.94154406, 0.9425732, 0.94359344, 0.9446049, 0.94560736, 0.9466009, 0.94758564, 0.9485614, 0.9495282, 0.95048606, 0.951435, 0.952375, 0.953306, 0.9542281, 0.9551412, 0.95604527, 0.95694035, 0.95782644, 0.9587035, 0.95957154, 0.96043056, 0.9612805, 0.9621214, 0.96295327, 0.96377605, 0.9645898, 0.96539444, 0.96619, 0.96697646, 0.9677538, 0.96852213, 0.96928126, 0.97003126, 0.97077215, 0.9715039, 0.9722265, 0.97293997, 0.97364426, 0.97433937, 0.97502536, 0.9757021, 0.97636974, 0.97702813, 0.97767735, 0.9783174, 0.9789482, 0.9795698, 0.9801822, 0.9807853, 0.9813792, 0.9819639, 0.9825393, 0.9831055, 0.9836624, 0.9842101, 0.9847485, 0.98527765, 0.9857975, 0.9863081, 0.98680943, 0.9873014, 0.98778415, 0.9882576, 0.9887217, 0.9891765, 0.989622, 0.9900582, 0.9904851, 0.99090266, 0.99131083, 0.99170977, 0.99209934, 0.99247956, 0.9928504, 0.993212, 0.9935641, 0.993907, 0.99424046, 0.9945646, 0.9948793, 0.9951847, 0.9954808, 0.9957674, 0.9960447, 0.9963126, 0.9965711, 0.99682033, 0.99706006, 0.99729043, 0.99751145, 0.99772304, 0.9979253, 0.9981181, 0.99830157, 0.99847555, 0.99864024, 0.99879545, 0.9989413, 0.99907774, 0.99920475, 0.9993224, 0.9994306, 0.9995294, 0.9996188, 0.9996988, 0.9997694, 0.9998306, 0.99988234, 0.9999247, 0.9999576, 0.99998116, 0.9999953, 1.0, 0.9999953, 0.99998116, 0.9999576, 0.9999247, 0.99988234, 0.9998306, 0.9997694, 0.9996988, 0.9996188, 0.9995294, 0.9994306, 0.99932235, 0.99920475, 0.99907774, 0.9989413, 0.99879545, 0.99864024, 0.99847555, 0.99830157, 0.9981181, 0.9979253, 0.99772304, 0.99751145, 0.99729043, 0.99706006, 0.9968203, 0.9965711, 0.9963126, 0.9960447, 0.9957674, 0.9954808, 0.9951847, 0.9948793, 0.99456453, 0.99424046, 0.993907, 0.9935641, 0.9932119, 0.9928504, 0.9924795, 0.9920993, 0.99170977, 0.99131083, 0.9909026, 0.9904851, 0.9900582, 0.989622, 0.9891765, 0.98872167, 0.9882576, 0.98778415, 0.9873014, 0.9868094, 0.9863081, 0.9857975, 0.98527765, 0.9847485, 0.9842101, 0.9836624, 0.9831055, 0.9825393, 0.9819639, 0.9813792, 0.98078525, 0.9801821, 0.97956973, 0.9789482, 0.9783174, 0.97767735, 0.97702813, 0.97636974, 0.9757021, 0.97502536, 0.97433937, 0.97364426, 0.9729399, 0.9722265, 0.9715039, 0.97077215, 0.97003126, 0.96928126, 0.9685221, 0.9677538, 0.96697646, 0.96619, 0.96539444, 0.9645898, 0.96377605, 0.9629532, 0.9621214, 0.96128047, 0.9604305, 0.9595715, 0.95870346, 0.9578264, 0.9569403, 0.9560452, 0.9551411, 0.9542281, 0.953306, 0.952375, 0.95143497, 0.95048606, 0.94952816, 0.9485614, 0.9475856, 0.9466009, 0.9456073, 0.9446048, 0.94359344, 0.9425732, 0.94154406, 0.94050604, 0.9394592, 0.9384035, 0.93733895, 0.93626565, 0.9351835, 0.9340925, 0.9329928, 0.9318842, 0.93076694, 0.9296409, 0.9285061, 0.9273625, 0.9262102, 0.9250492, 0.9238795, 0.9227011, 0.921514, 0.92031825, 0.9191139, 0.9179008, 0.916679, 0.9154487, 0.9142097, 0.9129622, 0.91170603, 0.9104413, 0.909168, 0.90788615, 0.9065957, 0.90529674, 0.9039893, 0.9026733, 0.9013488, 0.9000159, 0.8986744, 0.89732456, 0.89596623, 0.89459944, 0.8932243, 0.8918407, 0.8904487, 0.88904834, 0.8876396, 0.8862225, 0.88479704, 0.8833633, 0.88192123, 0.8804708, 0.87901217, 0.87754524, 0.87607, 0.8745866, 0.8730949, 0.8715951, 0.870087, 0.86857074, 0.86704624, 0.8655136, 0.86397284, 0.86242396, 0.86086696, 0.8593018, 0.8577286, 0.8561473, 0.854558, 0.8529606, 0.85135514, 0.84974176, 0.84812033, 0.8464909, 0.8448535, 0.8432082, 0.84155494, 0.83989376, 0.83822465, 0.8365477, 0.83486277, 0.83317006, 0.83146954, 0.82976115, 0.82804507, 0.82632107, 0.8245893, 0.8228498, 0.8211025, 0.8193475, 0.8175848, 0.81581444, 0.8140363, 0.81225055, 0.81045717, 0.80865616, 0.8068475, 0.8050313, 0.8032075, 0.8013761, 0.79953724, 0.7976908, 0.79583687, 0.7939754, 0.7921065, 0.79023015, 0.78834635, 0.78645515, 0.7845565, 0.7826505, 0.7807371, 0.7788164, 0.7768885, 0.7749531, 0.7730105, 0.7710605, 0.76910335, 0.7671389, 0.76516724, 0.7631884, 0.7612024, 0.75920916, 0.7572088, 0.75520134, 0.75318676, 0.7511651, 0.7491363, 0.74710053, 0.7450577, 0.7430079, 0.74095106, 0.73888725, 0.73681647, 0.73473877, 0.73265415, 0.7305627, 0.7284643, 0.72635907, 0.724247, 0.7221281, 0.72000253, 0.71787006, 0.71573085, 0.7135849, 0.7114322, 0.7092728, 0.70710677, 0.70493406, 0.70275474, 0.7005688, 0.69837624, 0.6961771, 0.6939714, 0.6917592, 0.6895405, 0.6873153, 0.6850836, 0.6828455, 0.68060094, 0.67835, 0.6760926, 0.6738289, 0.67155886, 0.6692825, 0.6669998, 0.6647109, 0.6624156, 0.6601144, 0.6578067, 0.6554929, 0.65317285, 0.6508467, 0.6485144, 0.64617604, 0.64383155, 0.641481, 0.6391244, 0.63676184, 0.6343933, 0.6320187, 0.6296382, 0.62725174, 0.62485945, 0.6224612, 0.6200571, 0.61764723, 0.6152315, 0.61280996, 0.6103827, 0.6079497, 0.60551095, 0.60306644, 0.60061634, 0.59816056, 0.59569913, 0.59323233, 0.59075975, 0.5882816, 0.58579785, 0.58330864, 0.58081394, 0.57831377, 0.57580817, 0.57329714, 0.5707807, 0.56825894, 0.56573176, 0.5631993, 0.5606615, 0.55811846, 0.5555702, 0.5530166, 0.5504579, 0.54789394, 0.54532486, 0.54275066, 0.5401714, 0.5375869, 0.53499746, 0.532403, 0.52980345, 0.527199, 0.52458954, 0.52197534, 0.519356, 0.5167318, 0.51410276, 0.5114688, 0.50883013, 0.5061866, 0.50353837, 0.50088537, 0.49822763, 0.4955652, 0.49289814, 0.49022642, 0.4875501, 0.48486918, 0.4821837, 0.47949368, 0.47679913, 0.4741001, 0.47139663, 0.4686887, 0.46597636, 0.46325964, 0.46053857, 0.45781314, 0.45508343, 0.45234942, 0.44961137, 0.44686887, 0.44412217, 0.4413713, 0.43861625, 0.4358571, 0.43309382, 0.43032646, 0.42755505, 0.42477965, 0.42200023, 0.41921684, 0.4164295, 0.41363823, 0.4108431, 0.40804407, 0.40524122, 0.40243456, 0.3996241, 0.39680988, 0.39399192, 0.39117026, 0.3883449, 0.3855159, 0.38268328, 0.37984705, 0.37700725, 0.3741639, 0.37131724, 0.36846685, 0.365613, 0.36275575, 0.35989505, 0.35703096, 0.3541635, 0.35129273, 0.34841865, 0.3455413, 0.34266067, 0.33977684, 0.3368898, 0.33399957, 0.33110622, 0.32820976, 0.3253102, 0.32240757, 0.3195019, 0.31659326, 0.3136816, 0.31076702, 0.3078495, 0.30492908, 0.3020058, 0.29907966, 0.2961507, 0.29321897, 0.29028472, 0.2873475, 0.28440756, 0.28146493, 0.2785197, 0.27557182, 0.27262133, 0.2696683, 0.26671273, 0.26375464, 0.26079407, 0.25783104, 0.2548656, 0.25189775, 0.24892752, 0.24595496, 0.24298008, 0.24000292, 0.23702349, 0.23404182, 0.23105797, 0.22807194, 0.22508375, 0.22209346, 0.21910107, 0.21610662, 0.21311013, 0.21011187, 0.2071114, 0.20410898, 0.20110464, 0.1980984, 0.19509031, 0.19208038, 0.18906865, 0.18605512, 0.18303984, 0.18002285, 0.17700416, 0.17398381, 0.17096181, 0.16793822, 0.16491303, 0.16188629, 0.15885803, 0.15582828, 0.15279706, 0.1497644, 0.14673033, 0.14369488, 0.14065808, 0.13761996, 0.13458052, 0.13153984, 0.12849791, 0.125455, 0.1224107, 0.11936523, 0.116318636, 0.113270946, 0.1102222, 0.10717241, 0.1041216, 0.10106982, 0.0980171, 0.09496344, 0.091908894, 0.08885348, 0.08579723, 0.08274017, 0.079682335, 0.07662375, 0.07356445, 0.07050445, 0.06744379, 0.064382486, 0.061320584, 0.058258105, 0.055195075, 0.052131526, 0.049067486, 0.046002988, 0.042938054, 0.03987295, 0.03680724, 0.03374118, 0.030674802, 0.027608138, 0.02454121, 0.021474054, 0.018406695, 0.015339162, 0.012271485, 0.009203694, 0.006135815, 0.003067878, -8.742278E-8, -0.003068053, -0.0061359894, -0.009203868, -0.01227166, -0.015339337, -0.01840687, -0.02147423, -0.024541385, -0.02760831, -0.030674977, -0.033741355, -0.036807414, -0.039873127, -0.04293823, -0.046003163, -0.04906766, -0.0521317, -0.05519525, -0.05825828, -0.06132076, -0.064382665, -0.06744396, -0.07050462, -0.07356462, -0.076623924, -0.079682514, -0.08274035, -0.08579741, -0.08885365, -0.091909066, -0.09496361, -0.09801727, -0.10107, -0.104121774, -0.10717258, -0.11022237, -0.113271125, -0.11631881, -0.1193654, -0.12241087, -0.12545519, -0.12849809, -0.13154002, -0.1345807, -0.13762012, -0.14065824, -0.14369506, -0.1467305, -0.14976457, -0.15279724, -0.15582845, -0.15885821, -0.16188647, -0.16491319, -0.16793838, -0.17096199, -0.17398398, -0.17700434, -0.18002303, -0.18304002, -0.18605529, -0.18906881, -0.19208056, -0.19509049, -0.19809859, -0.20110482, -0.20410916, -0.20711157, -0.21011205, -0.2131103, -0.21610679, -0.21910124, -0.22209363, -0.22508393, -0.2280721, -0.23105814, -0.234042, -0.23702365, -0.24000308, -0.24298024, -0.24595512, -0.24892768, -0.2518979, -0.25486577, -0.25783122, -0.26079422, -0.2637548, -0.2667129, -0.26966846, -0.2726215, -0.27557197, -0.27851987, -0.2814651, -0.2844077, -0.28734764, -0.29028487, -0.29321915, -0.29615086, -0.29907984, -0.30200595, -0.30492926, -0.30784965, -0.31076717, -0.31368178, -0.3165934, -0.3195021, -0.32240775, -0.32531035, -0.3282099, -0.3311064, -0.33399975, -0.33688995, -0.339777, -0.34266084, -0.34554145, -0.3484188, -0.3512929, -0.35416368, -0.3570311, -0.3598952, -0.3627559, -0.3656132, -0.36846703, -0.3713174, -0.37416404, -0.3770074, -0.3798472, -0.38268343, -0.38551608, -0.38834506, -0.3911704, -0.39399207, -0.39681002, -0.39962426, -0.4024347, -0.4052414, -0.40804425, -0.41084325, -0.4136384, -0.41642967, -0.419217, -0.42200038, -0.4247798, -0.42755523, -0.4303266, -0.43309397, -0.43585724, -0.4386164, -0.44137144, -0.4441223, -0.44686902, -0.4496115, -0.45234957, -0.45508358, -0.45781332, -0.46053872, -0.46325982, -0.46597654, -0.46868885, -0.47139677, -0.47410026, -0.47679928, -0.47949383, -0.48218384, -0.48486933, -0.48755026, -0.49022657, -0.4928983, -0.49556538, -0.49822778, -0.5008855, -0.5035385, -0.5061868, -0.5088303, -0.511469, -0.5141029, -0.516732, -0.5193562, -0.52197546, -0.52458966, -0.52719915, -0.52980363, -0.5324031, -0.53499764, -0.5375871, -0.5401715, -0.54275084, -0.54532504, -0.5478941, -0.550458, -0.5530168, -0.5555703, -0.55811864, -0.5606617, -0.56319946, -0.5657319, -0.56825906, -0.5707809, -0.57329726, -0.57580835, -0.57831395, -0.5808141, -0.5833088, -0.585798, -0.5882817, -0.5907599, -0.59323245, -0.5956993, -0.5981607, -0.6006165, -0.6030666, -0.60551107, -0.6079498, -0.61038285, -0.61281013, -0.61523163, -0.61764735, -0.6200573, -0.6224613, -0.6248596, -0.6272519, -0.6296383, -0.6320188, -0.6343934, -0.63676196, -0.6391246, -0.64148116, -0.6438317, -0.64617616, -0.6485145, -0.65084684, -0.65317297, -0.655493, -0.6578069, -0.6601145, -0.6624158, -0.664711, -0.66699994, -0.6692826, -0.671559, -0.673829, -0.67609274, -0.6783501, -0.68060106, -0.6828456, -0.68508375, -0.6873154, -0.6895406, -0.69175935, -0.6939716, -0.69617724, -0.69837636, -0.7005689, -0.70275486, -0.7049342, -0.7071069, -0.709273, -0.71143234, -0.713585, -0.71573097, -0.7178702, -0.72000265, -0.7221282, -0.7242471, -0.7263592, -0.7284644, -0.7305628, -0.7326543, -0.7347389, -0.7368166, -0.73888737, -0.7409512, -0.743008, -0.7450578, -0.74710065, -0.74913645, -0.7511652, -0.7531869, -0.75520146, -0.75720876, -0.75920916, -0.76120234, -0.76318836, -0.76516724, -0.7671389, -0.7691033, -0.7710605, -0.77301043, -0.77495307, -0.77688843, -0.7788165, -0.7807372, -0.7826506, -0.7845566, -0.7864552, -0.78834647, -0.7902303, -0.7921066, -0.79397553, -0.7958369, -0.79769087, -0.7995373, -0.8013762, -0.8032076, -0.8050314, -0.80684763, -0.8086563, -0.8104573, -0.8122507, -0.8140364, -0.8158145, -0.81758493, -0.8193476, -0.8211026, -0.82284987, -0.82458943, -0.8263212, -0.8280452, -0.8297614, -0.8314698, -0.8331703, -0.834863, -0.83654785, -0.8382249, -0.83989394, -0.8415551, -0.84320843, -0.84485376, -0.8464911, -0.8481205, -0.84974194, -0.8513554, -0.8529605, -0.85455793, -0.8561473, -0.85772854, -0.8593018, -0.8608669, -0.8624239, -0.86397284, -0.8655136, -0.86704624, -0.8685707, -0.87008697, -0.8715951, -0.873095, -0.87458664, -0.8760701, -0.8775453, -0.8790122, -0.88047093, -0.8819213, -0.88336337, -0.88479716, -0.88622254, -0.88763964, -0.8890484, -0.89044875, -0.89184076, -0.89322436, -0.89459956, -0.8959663, -0.8973246, -0.89867455, -0.90001595, -0.90134895, -0.9026734, -0.9039894, -0.90529686, -0.90659577, -0.9078862, -0.90916806, -0.9104414, -0.91170615, -0.9129623, -0.91420984, -0.91544884, -0.91667914, -0.91790086, -0.919114, -0.9203184, -0.92151415, -0.92270124, -0.9238797, -0.92504936, -0.92621034, -0.9273627, -0.928506, -0.9296408, -0.93076694, -0.9318842, -0.93299276, -0.9340925, -0.93518347, -0.93626565, -0.937339, -0.93840355, -0.9394592, -0.94050604, -0.94154406, -0.9425732, -0.94359344, -0.9446048, -0.9456073, -0.9466009, -0.9475856, -0.9485614, -0.9495282, -0.9504861, -0.951435, -0.95237505, -0.9533061, -0.9542281, -0.9551412, -0.95604527, -0.95694035, -0.95782644, -0.9587035, -0.95957154, -0.96043056, -0.9612805, -0.9621214, -0.9629533, -0.9637761, -0.96458983, -0.9653945, -0.96619004, -0.9669765, -0.9677539, -0.96852213, -0.9692813, -0.9700313, -0.9707722, -0.971504, -0.97222656, -0.97294, -0.9736443, -0.9743394, -0.9750254, -0.9757022, -0.9763698, -0.9770282, -0.97767735, -0.9783173, -0.9789482, -0.97956973, -0.9801821, -0.98078525, -0.9813792, -0.9819639, -0.9825393, -0.9831055, -0.9836624, -0.9842101, -0.9847485, -0.98527765, -0.9857975, -0.9863081, -0.98680943, -0.9873014, -0.98778415, -0.9882576, -0.9887217, -0.9891765, -0.98962206, -0.99005824, -0.9904851, -0.99090266, -0.9913109, -0.99170977, -0.99209934, -0.99247956, -0.9928504, -0.993212, -0.9935641, -0.993907, -0.99424046, -0.9945646, -0.99487936, -0.9951847, -0.9954808, -0.9957674, -0.9960447, -0.9963126, -0.9965712, -0.99682033, -0.9970601, -0.9972905, -0.9975115, -0.9977231, -0.9979253, -0.9981181, -0.99830157, -0.9984756, -0.99864024, -0.99879545, -0.9989413, -0.99907774, -0.99920475, -0.99932235, -0.9994306, -0.9995294, -0.9996188, -0.9996988, -0.9997694, -0.9998306, -0.99988234, -0.9999247, -0.9999576, -0.99998116, -0.9999953, -1.0, -0.9999953, -0.99998116, -0.9999576, -0.9999247, -0.99988234, -0.9998306, -0.9997694, -0.9996988, -0.9996188, -0.9995294, -0.9994306, -0.99932235, -0.99920475, -0.99907774, -0.9989413, -0.99879545, -0.99864024, -0.99847555, -0.9983015, -0.9981181, -0.9979253, -0.99772304, -0.99751145, -0.99729043, -0.99706006, -0.9968203, -0.9965711, -0.9963126, -0.9960447, -0.9957674, -0.9954807, -0.9951847, -0.9948793, -0.99456453, -0.9942404, -0.9939069, -0.99356407, -0.9932119, -0.99285036, -0.9924795, -0.9920993, -0.99170977, -0.9913109, -0.99090266, -0.9904851, -0.99005824, -0.989622, -0.9891765, -0.98872167, -0.9882576, -0.98778415, -0.9873014, -0.9868094, -0.9863081, -0.9857975, -0.98527765, -0.9847485, -0.9842101, -0.9836624, -0.9831055, -0.9825393, -0.9819639, -0.98137915, -0.98078525, -0.9801821, -0.97956973, -0.9789482, -0.9783173, -0.97767735, -0.97702813, -0.9763697, -0.9757021, -0.9750253, -0.97433937, -0.9736442, -0.9729399, -0.97222644, -0.97150385, -0.9707721, -0.9700312, -0.9692812, -0.968522, -0.96775377, -0.9669764, -0.9661899, -0.9653944, -0.9645897, -0.963776, -0.9629532, -0.9621213, -0.9612804, -0.96043044, -0.9595714, -0.9587034, -0.9578263, -0.95694023, -0.95604527, -0.9551412, -0.9542281, -0.9533061, -0.95237505, -0.951435, -0.95048606, -0.9495282, -0.9485614, -0.9475856, -0.9466009, -0.9456073, -0.9446048, -0.94359344, -0.9425732, -0.94154406, -0.94050604, -0.9394592, -0.9384035, -0.937339, -0.93626565, -0.93518347, -0.9340925, -0.93299276, -0.9318842, -0.93076694, -0.9296408, -0.928506, -0.92736244, -0.92621017, -0.9250492, -0.92387944, -0.92270106, -0.921514, -0.9203182, -0.91911376, -0.9179007, -0.91667897, -0.9154486, -0.91420966, -0.9129621, -0.9117059, -0.91044116, -0.9091679, -0.90788597, -0.9065956, -0.9052966, -0.90398914, -0.9026732, -0.9013487, -0.9000158, -0.8986743, -0.89732444, -0.8959661, -0.8945993, -0.8932241, -0.89184076, -0.89044875, -0.8890484, -0.88763964, -0.88622254, -0.8847971, -0.88336337, -0.8819213, -0.8804709, -0.8790122, -0.8775453, -0.8760701, -0.87458664, -0.873095, -0.8715951, -0.87008697, -0.8685707, -0.86704624, -0.86551356, -0.86397284, -0.8624239, -0.8608669, -0.85930175, -0.85772854, -0.8561473, -0.85455793, -0.8529605, -0.85135514, -0.8497417, -0.8481203, -0.84649086, -0.84485346, -0.84320813, -0.8415549, -0.8398937, -0.8382246, -0.8365476, -0.83486277, -0.83317006, -0.8314695, -0.8297611, -0.8280449, -0.8263209, -0.82458913, -0.82284963, -0.8211023, -0.8193473, -0.81758463, -0.8158142, -0.81403613, -0.8122504, -0.810457, -0.808656, -0.80684733, -0.8050311, -0.8032076, -0.8013762, -0.7995373, -0.79769087, -0.7958369, -0.79397553, -0.79210657, -0.7902302, -0.7883464, -0.7864552, -0.78455657, -0.7826506, -0.7807372, -0.7788165, -0.77688843, -0.77495307, -0.77301043, -0.77106047, -0.7691033, -0.76713884, -0.76516724, -0.76318836, -0.76120234, -0.7592091, -0.75720876, -0.7552013, -0.7531867, -0.75116503, -0.74913627, -0.7471005, -0.74505764, -0.74300784, -0.740951, -0.7388872, -0.7368164, -0.7347387, -0.7326541, -0.7305626, -0.72846425, -0.726359, -0.7242469, -0.72212803, -0.7200023, -0.7178698, -0.7157306, -0.71358466, -0.711432, -0.7092726, -0.70710653, -0.7049338, -0.7027545, -0.70056856, -0.698376, -0.6961769, -0.69397116, -0.69175935, -0.6895406, -0.6873154, -0.6850837, -0.6828456, -0.680601, -0.6783501, -0.67609274, -0.673829, -0.671559, -0.6692826, -0.66699994, -0.66471094, -0.66241574, -0.6601143, -0.65780663, -0.6554928, -0.6531728, -0.6508466, -0.64851433, -0.6461759, -0.6438315, -0.6414809, -0.63912433, -0.6367618, -0.63439316, -0.6320186, -0.62963814, -0.6272517, -0.62485933, -0.62246114, -0.62005705, -0.6176472, -0.61523145, -0.6128099, -0.6103826, -0.6079496, -0.60551083, -0.6030664, -0.6006163, -0.5981605, -0.5956991, -0.5932321, -0.59075946, -0.58828133, -0.5857976, -0.5833084, -0.5808137, -0.5783135, -0.5758079, -0.5732969, -0.57078046, -0.56825864, -0.5657315, -0.56319904, -0.56066126, -0.5581186, -0.5555703, -0.5530168, -0.550458, -0.5478941, -0.54532504, -0.5427508, -0.5401715, -0.5375871, -0.53499764, -0.5324031, -0.52980363, -0.5271991, -0.52458966, -0.5219753, -0.51935595, -0.51673174, -0.5141027, -0.51146877, -0.5088301, -0.50618654, -0.5035383, -0.5008853, -0.49822757, -0.49556515, -0.49289808, -0.49022636, -0.48755002, -0.4848691, -0.4821836, -0.4794936, -0.47679907, -0.47410002, -0.47139654, -0.46868864, -0.4659763, -0.46325958, -0.46053848, -0.45781308, -0.45508334, -0.45234933, -0.44961107, -0.44686857, -0.44412187, -0.441371, -0.43861595, -0.4358568, -0.43309352, -0.43032616, -0.4275548, -0.42477936, -0.42199993, -0.41921654, -0.41642922, -0.41363797, -0.41084322, -0.40804422, -0.40524137, -0.4024347, -0.39962423, -0.39681002, -0.39399207, -0.39117038, -0.38834506, -0.38551605, -0.38268343, -0.3798472, -0.3770074, -0.37416402, -0.37131715, -0.36846676, -0.36561292, -0.36275566, -0.35989496, -0.35703087, -0.35416344, -0.35129264, -0.34841856, -0.3455412, -0.34266058, -0.33977675, -0.3368897, -0.33399948, -0.33110616, -0.32820967, -0.3253101, -0.32240748, -0.31950185, -0.31659317, -0.31368154, -0.31076694, -0.3078494, -0.304929, -0.3020057, -0.29907957, -0.29615062, -0.29321888, -0.2902844, -0.28734717, -0.28440723, -0.28146464, -0.27851936, -0.2755715, -0.27262104, -0.26966798, -0.2667124, -0.2637543, -0.26079375, -0.25783074, -0.2548653, -0.25189787, -0.24892767, -0.2459551, -0.24298023, -0.24000306, -0.23702364, -0.23404197, -0.23105812, -0.22807208, -0.2250839, -0.22209361, -0.21910122, -0.21610676, -0.21311028, -0.21011178, -0.20711131, -0.2041089, -0.20110455, -0.19809833, -0.19509023, -0.19208029, -0.18906856, -0.18605503, -0.18303975, -0.18002276, -0.17700407, -0.17398372, -0.17096172, -0.16793813, -0.16491294, -0.1618862, -0.15885794, -0.1558282, -0.15279697, -0.14976431, -0.14673024, -0.14369479, -0.14065799, -0.13761987, -0.13458045, -0.13153975, -0.12849782, -0.1254547, -0.12241037, -0.1193649, -0.116318315, -0.113270625, -0.11022187, -0.10717208, -0.10412128, -0.1010695, -0.09801677, -0.09496312, -0.09190857, -0.08885316, -0.08579691, -0.08274032, -0.07968249, -0.0766239, -0.0735646, -0.0705046, -0.06744394, -0.064382635, -0.061320737, -0.058258254, -0.055195227, -0.05213168, -0.04906764, -0.046003137, -0.042938203, -0.039872866, -0.036807153, -0.033741094, -0.030674715, -0.02760805, -0.024541123, -0.021473967, -0.018406607, -0.015339075, -0.012271399, -0.009203606, -0.0061357273, -0.0030677905, 0.0};
