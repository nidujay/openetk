/**
    \file
    \brief OS services object interface

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

#ifndef IOS_SERVICES_H
#define IOS_SERVICES_H

namespace djetk {

/**
 * \brief Interface for OS service broker
 */
class IOsServices {
  public:
    /**
     * \brief Start the OS scheduler
     */
    virtual void StartScheduler() = 0;

    /**
     * \brief Stop the OS scheduler
     */
    virtual void StopScheduler() = 0;

    /**
     * \brief Disable global interrupts
     */
    virtual void DisableInterrupts() = 0;

    /**
     * \brief Enable global interrupts
     */
    virtual void EnableInterrupts() = 0;

    virtual ~IOsServices() {}
};

/**
 * \brief Helper class to enable/disable interrupts in scope
 */
class AutoInterruptDisabler {
  public:
    /**
     * \brief Constructor disables interrupts
     * \param[in]   os_services Reference to OS services interface
     */
    explicit AutoInterruptDisabler(IOsServices &os_services)
        : os_services_(os_services)
    {
        os_services_.DisableInterrupts();
    }

    /**
     * \brief Destructor that disables interrupts
     */
    ~AutoInterruptDisabler()
    {
        os_services_.EnableInterrupts();
    }

  private:
    IOsServices &os_services_;
};

}    // namespace djetk

#endif    // IOS_SERVICES_H

