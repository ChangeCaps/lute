// Copyright (C) 2024  Hjalte C. Nannestad
// See end of file for license information.

#include "target.h"

Target *select_target(Build *build, const char *name) {
    if (!name && build->targets.len > 0) {
        return build->targets.data[0];
    }

    for (size_t i = 0; i < build->targets.len; i++) {
        Target *target = build->targets.data[i];

        if (strcmp(target->name, name) == 0) {
            return target;
        }
    }

    return NULL;
}

// This file is part of Lute.
// Copyright (C) 2024  Hjalte C. Nannestad
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
