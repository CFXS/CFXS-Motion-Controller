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
#include <new>

static const char* s_TestData = R"(
F400
)";

int main() {
    CFXS_printf("[%s v%s]\n", CFXS_PROJECT_NAME, CFXS_PROJECT_VERSION_STRING);

    char temp[sizeof(CFXS::CNC::G_Man)];
    auto gman = new (temp) CFXS::CNC::G_Man;

    auto dataLen = strlen(s_TestData);
    auto dataPtr = s_TestData;
    for (int i = 0; i < dataLen; i += 13) {
        gman->ProcessCommandDataBlock(dataPtr, 13);
        dataPtr += 13;
    }
    auto remainder = dataLen % 13;
    if (remainder) {
        gman->ProcessCommandDataBlock(dataPtr, remainder);
    }

    return 0;
}