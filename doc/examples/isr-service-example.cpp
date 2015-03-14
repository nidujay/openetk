#include <iostream>
#include <isr/isr-service.h>

namespace djetk {

// Base class of some peripheral driver. It implements the interrupt
// handler code for that type of device.
// Each instance of this peripheral (e.g. UART1 and UART2) will derive
// from this.
class SomeDriverBase : private IIsrHandler
{
  public:
    SomeDriverBase (IIsrService &isr_service, int id)
        : id_(id)
    {
        isr_service.RegisterHandler(*this);
    }

  private:
    virtual void HandleIsr(bool &task_woken) override
    {
        // Interrupt handler code is common to all the derived
        // classes of this driver
        std::cout << id_ << std::endl;

        // Set to true if an interrupt were to wake a higher priority task
        // (queues return this value)
        task_woken = false;
    }

    int id_;
};

// Drivers for each peripheral instance
class Device1 : public SomeDriverBase {
  public:
    Device1(IIsrService &isr_service)
        : SomeDriverBase(isr_service, 1)
    {
    }
};

class Device2 : public SomeDriverBase {
  public:
    Device2(IIsrService &isr_service)
        : SomeDriverBase(isr_service, 2)
    {
    }
};

// These are the interrupt handler vectors for the two peripherals of the same
// device class
extern "C" void Device1IrqVector()
{
    bool task_woken;
    IsrService<Device1>::ISR(task_woken);
}

extern "C" void Device2IrqVector()
{
    bool task_woken;
    IsrService<Device2>::ISR(task_woken);
}

}    // namespace djetk


int main()
{
    using namespace djetk;

    IsrService<Device1> isr_service1;
    IsrService<Device2> isr_service2;

    Device1 device1(isr_service1);
    Device2 device2(isr_service2);

    Device1IrqVector();
    Device2IrqVector();

    return 0;
}


