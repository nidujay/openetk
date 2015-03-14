/**
    \file
    \brief

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

#ifndef CRITICAL_ERROR_HANDLER_STUB_H
#define CRITICAL_ERROR_HANDLER_STUB_H

#include <errors/icritical-error-handler.h>

namespace djetk {

/**
 * \brief Critical Error Handler implementation for unit tests
 */
class CriticalErrorHandlerStub : public ICriticalErrorHandler {
  public:
    CriticalErrorHandlerStub()
       : is_critical_error(false),
       last_error_code(0)
    {
    }

    /**
     * \brief See \ref ICriticalErrorHandler::NotifyCriticalError
     */
    virtual void NotifyCriticalError(uint32_t error_code,
            const char *filename, uint32_t line) override
    {
        // Disable warnings regarding unused variables. This might
        // not work outside of gcc
        (void)filename;
        (void)line;
        is_critical_error = true;
        last_error_code = error_code;
    }

    /**
     * \brief Flag to indicate a critical error has occurred
     */
    bool is_critical_error;

    /**
     * \brief Last error code when a critical error occurred
     */
    uint32_t last_error_code;
};

}    // namespace djetk

#endif    // CRITICAL_ERROR_HANDLER_STUB_H

