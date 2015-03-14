/**
    \file
    \brief Tick Source Interface

    \copyright
    Copyright (C) 2015 Dushara Jayasinghe

    This file is part of DJETK

    DJETK is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DJETK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with DJETK.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ITICK_SOURCE_H
#define ITICK_SOURCE_H

#include <cstdint>
#include <timing/itick-client.h>

namespace djetk {

/**
 * \brief Tick Source Interface
 *
 * Tick provides a mechanism to register for timer events. The
 * tick source may be an interrupt or some other mechanism. The registered
 * tick clients are notified on every tick.
 */
class ITickSource {
  public:
    /**
     * \brief Register a Tick Client
     * \param[in]   client  Reference to a tick client
     * \return true if successful
     */
    virtual bool RegisterTickClient(ITickClient &client) = 0;

    /**
     * \brief Convert from milliseconds to ticks
     * \param[in]   milliseconds    Value to convert to ticks
     */
    virtual uint32_t MsToTicks(uint32_t milliseconds) = 0;

    virtual ~ITickSource() {}
};

}    // namespace djetk

#endif    // ITICK_SOURCE_H

