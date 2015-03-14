/**
    \file
    \brief Stub of OS services for testing purposes

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

#ifndef OS_SERVICES_STUB_H
#define OS_SERVICES_STUB_H

#include <os/ios-services.h>

namespace djetk {

/**
 * \brief OS services Stub implementation
 */
class OsServicesStub : public IOsServices {
  public:
    /**
     * \brief See \ref IOsServices::StartScheduler
     */
    virtual void StartScheduler() override {}

    /**
     * \brief See \ref IOsServices::StopScheduler
     */
    virtual void StopScheduler() override {}

    /**
     * \brief See \ref IOsServices::DisableInterrupts
     */
    virtual void DisableInterrupts() override {}

    /**
     * \brief See \ref IOsServices::EnableInterrupts
     */
    virtual void EnableInterrupts() override {}

    ~OsServicesStub() {}

  private:
};

}    // namespace djetk

#endif    // OS_SERVICES_STUB_H

