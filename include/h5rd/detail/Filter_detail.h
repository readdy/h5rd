/********************************************************************
 * Copyright © 2017 Computational Molecular Biology Group,          * 
 *                  Freie Universität Berlin (GER)                  *
 *                                                                  *
 * This file is part of ReaDDy.                                     *
 *                                                                  *
 * ReaDDy is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU Lesser General Public License as   *
 * published by the Free Software Foundation, either version 3 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU Lesser General Public License for more details.              *
 *                                                                  *
 * You should have received a copy of the GNU Lesser General        *
 * Public License along with this program. If not, see              *
 * <http://www.gnu.org/licenses/>.                                  *
 ********************************************************************/


/**
 * << detailed description >>
 *
 * @file Filter_detail.h
 * @brief << brief description >>
 * @author clonker
 * @date 06.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <H5Zpublic.h>

#include "../File.h"
#include "../Filter.h"
#include "../PropertyList.h"

namespace h5rd {

inline SZIPFilter::SZIPFilter(SZIPFilter::CodingMethod method, unsigned int pixelsPerBlock)
        : codingMethod(method), pixelsPerBlock(pixelsPerBlock) {}

inline bool SZIPFilter::available() const {
    return H5Zfilter_avail(H5Z_FILTER_SZIP) > 0;
}

inline void SZIPFilter::registerFilter() {}

inline void SZIPFilter::activate(PropertyList &plist) {
    if (available()) {
        switch (codingMethod) {
            case Entropy: {
                H5Pset_szip(plist.id(), H5_SZIP_EC_OPTION_MASK, pixelsPerBlock);
                break;
            }
            case NearestNeighbor: {
                H5Pset_szip(plist.id(), H5_SZIP_NN_OPTION_MASK, pixelsPerBlock);
                break;
            }
        }
    } else {
        throw std::runtime_error("Tried activating szip filter even though it was not available");
    }
}

inline bool NBITFilter::available() const {
    return H5Zfilter_avail(H5Z_FILTER_NBIT) > 0;
}

inline void NBITFilter::activate(PropertyList &plist) {
    if (!available()) {
        throw std::runtime_error("Tried activating nbit filter even though it was not available");
    }
    H5Pset_nbit(plist.id());
}

inline void NBITFilter::registerFilter() {}

inline ScaleOffsetFilter::ScaleOffsetFilter(ScaleType scaleType, int scaleFactor)
        : scaleType(scaleType), scaleFactor(scaleFactor) {}

inline bool ScaleOffsetFilter::available() const {
    return H5Zfilter_avail(H5Z_FILTER_SCALEOFFSET) > 0;
}

inline void ScaleOffsetFilter::registerFilter() {}

inline void ScaleOffsetFilter::activate(PropertyList &plist) {
    switch (scaleType) {
        case FloatingPointVariableMinBits: {
            H5Pset_scaleoffset(plist.id(), H5Z_SO_FLOAT_DSCALE, scaleFactor);
            break;
        }
        case FloatingPointFixedMinBits: {
            H5Pset_scaleoffset(plist.id(), H5Z_SO_FLOAT_ESCALE, scaleFactor);
            break;
        }
        case IntegerType: {
            H5Pset_scaleoffset(plist.id(), H5Z_SO_INT, scaleFactor);
            break;
        }
    }
}

inline bool ShuffleFilter::available() const {
    return H5Zfilter_avail(H5Z_FILTER_SHUFFLE) > 0;
}

inline void ShuffleFilter::activate(PropertyList &plist) {
    H5Pset_shuffle(plist.id());
}

inline void ShuffleFilter::registerFilter() {}

inline bool Fletcher32Filter::available() const {
    return H5Zfilter_avail(H5Z_FILTER_FLETCHER32) > 0;
}

inline void Fletcher32Filter::activate(PropertyList &plist) {
    H5Pset_fletcher32(plist.id());
}

inline void Fletcher32Filter::registerFilter() {}


}
