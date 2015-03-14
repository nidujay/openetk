/**
    \file
    \brief Interface for a system tick client

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

#ifndef ITICK_CLIENT_H
#define ITICK_CLIENT_H

namespace djetk {

/**
 * \brief Tick Client interface
 *
 * This interface provides a means to register an object to a
 * system tick source.
 */
class ITickClient {
  public:
    /**
     * \brief Callback to indicate a tick event
     * \param[out]  task_woken  Indicates if a thread reschedule is required
     * \return true on success
     */
    virtual bool OnTickFromIsr(bool &task_woken) = 0;

    virtual ~ITickClient() {}
};

}    // namespace djetk

#endif    // ITICK_CLIENT_H

