#ifndef BASE_H
#define BASE_H

/***************************************************************************
Base.h
Comment: This file contains all basic definitions.
***************************************************************************/

#define PI 3.141592653589793

// STL includes.
#include <algorithm>
#include <iostream>
#include <cassert>
#include <vector>
#include <cfloat>
#include <cmath>
#include <queue>
#include <set>

// Local includes.
#include "Point3.h"

#define round(x) floor(x+0.5)

//--------------------------------------------------------------------------
/** draw modes */
enum DrawMode {
    None,
    Wire,
    Smooth,
    Points,
    Hidden,
    Flat,
    FlatWire,
    Radar,
    Patch
};

//---------------------------------------------------------------------------
/** color modes */
enum ColorMode {
    CMNone,
    PerVert,
    PerFace
};

#endif
