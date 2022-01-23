// ---------------------------------------------------------------------
// CFXS Motion Controller <https://github.com/CFXS/CFXS-Motion-Controller>
// Copyright (C) 2022 | CFXS / Rihards Veips
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
// ---------------------------------------------------------------------
// [CFXS] //
#include <CFXS/Base/Debug.hpp>
#include <CFXS/CNC/G_Man.hpp>
#include <memory>

static const char* s_TestData[] = {
    "G1 F400",
    "M02",
};

void main() {
    CFXS_printf("[%s v%s]\n", CFXS_PROJECT_NAME, CFXS_PROJECT_VERSION_STRING);

    auto gman = std::make_unique<CFXS::CNC::G_Man>();

    for (auto line : s_TestData) {
        auto stat = gman->ProcessCommand(line, strlen(line));
        printf(" - %s\n", CFXS::ToString(stat));
    }
}