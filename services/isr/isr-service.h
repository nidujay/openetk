/**
    \file
    \brief ISR Service interfaces and definition

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

#ifndef ISR_SERVICE_H
#define ISR_SERVICE_H

namespace djetk {

/**
 * \brief ISR Handler interface
 *
 * Device drivers that handle interrupts can inherit from this. The
 * handler can then register with an \ref IIsrService to be notified when
 * interrupts fire. The device driver is then able to handle the interrupt.
 *
 * The example below shows how it all ties up together
 * \include isr-service-example.cpp
 */
class IIsrHandler {
  public:
    /**
     * \brief Driver specific ISR handler function
     * \param[out]  task_woken  Rescheduling required due to woken up task
     */
    virtual void HandleIsr(bool &task_woken) = 0;
    virtual ~IIsrHandler() {}
};

/**
 * \brief Definition of ISR Service interface
 *
 * This is a generic interface for any ISR that the device offers. By using
 * an interface, it is possible for a single device driver base class to
 * handle interrupts from multiple sources of the same class. For example
 * a UartDriverBase can handle interrupts originating from the UART1 and
 * UART2 vectors.
 *
 * The example below shows how it all ties up together
 * \include isr-service-example.cpp
 */
class IIsrService {
  public:
    /**
     * \brief Register the Handler for the interface
     * \param[in]   handler Handler class to register
     * return       True if successful
     *
     * The registered handler is notified when an interrupt fires.
     */
    virtual bool RegisterHandler(IIsrHandler &handler) = 0;
    virtual ~IIsrService() {}
};

/**
 * \brief Driver specific ISR Service implementation
 * \param Driver    Class of driver that will register for ISR service events
 *
 * This template defines the registrable hook to an Interrupt Service Routine.
 * An instance per IRQ can be defined. The example below shows how it all ties
 * up together.
 *
 * The example below shows how it all ties up together
 * \include isr-service-example.cpp
 */
template <class Driver>
class IsrService : public IIsrService {
  public:
    /**
     * \brief See IIsrService::RegisterHandler
     */
    virtual bool RegisterHandler(IIsrHandler &handler)
    {
        if (handler_ != nullptr) {
            return false;
        }

        handler_ = &handler;
        return true;
    }

    /**
     * \brief Unregister any registered handlers
     */
    void UnregisterHandler()
    {
        handler_ = nullptr;
    }

    /**
     * \brief Device Driver ISR handler
     * \param[out]  task_woken  Rescheduling required due to woken up task
     *
     * This method is to be invoked by the IRQ vector
     */
    static void ISR(bool &task_woken)
    {
        if (handler_) {
            handler_->HandleIsr(task_woken);
        }
    }

  private:
    static IIsrHandler *handler_;
};

template <class Driver>
IIsrHandler *IsrService<Driver>::handler_;

}    // namespace djetk

#endif    // ISR_SERVICE_H

