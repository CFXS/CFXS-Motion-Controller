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
#include <memory>
#include <CFXS/Base/Debug.hpp>
#include <CFXS/Platform/CPU.hpp>
#include <CFXS/CNC/G_Man.hpp>
#include <TestData/TestData.hpp>
#include <driverlib/sysctl.h>

void main() {
    CFXS_printf("[%s v%s]\n", CFXS_PROJECT_NAME, CFXS_PROJECT_VERSION_STRING);

    auto gman = std::make_unique<CFXS::CNC::G_Man>();

    const char* testData = reinterpret_cast<const char*>(res_TestData);
    char lineTemp[512]; // 512 byte line buffer
    int lineIndex = 0;

    while (testData) {
        lineTemp[0] = 0;
        int chidx;
        for (chidx = 0; chidx < sizeof(lineTemp) - 1; chidx++) {
            if (testData[chidx] == '\n') {
                break;
            } else if (testData[chidx] == 0) {
                testData = nullptr;
                chidx    = 0;
                break;
            } else {
                lineTemp[chidx] = testData[chidx];
            }
        }

        if (testData) {
            testData += chidx + 1; // +1 for \n
        }

        if (chidx) {
            lineIndex++;
            lineTemp[chidx] = 0;

            // clean up \r
            auto len = strlen(lineTemp);
            for (int i = 0; i < len; i++) {
                if (lineTemp[i] == '\r') {
                    for (int j = i; j < len; j++) {
                        lineTemp[j] = lineTemp[j + 1];
                    }
                    len--;
                    i--;
                }
            }

            auto stat = gman->ProcessCommand(lineTemp, strlen(lineTemp));
            printf(" - %s\n", CFXS::ToString(stat));
        }
    }
}