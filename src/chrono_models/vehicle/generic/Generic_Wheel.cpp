// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All right reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban
// =============================================================================
//
// HMMWV wheel subsystem
//
// =============================================================================

#include "chrono_models/vehicle/generic/Generic_Wheel.h"

namespace chrono {
namespace vehicle {
namespace generic {

// -----------------------------------------------------------------------------
// Static variables
// -----------------------------------------------------------------------------

const double Generic_Wheel::m_mass = 45.4;
const ChVector<> Generic_Wheel::m_inertia(0.113, 0.113, 0.113);

const double Generic_Wheel::m_radius = 0.268;
const double Generic_Wheel::m_width = 0.22;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
Generic_Wheel::Generic_Wheel(const std::string& name) : ChWheel(name) {}


}  // end namespace generic
}  // end namespace vehicle
}  // end namespace chrono