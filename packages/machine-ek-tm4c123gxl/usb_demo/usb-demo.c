#include "boilerplate.h"
#include "rtos-kochab.h"
#include "driverlib/rom_map.h"
#include "driverlib/interrupt.h"
#include "usblib/usblib.h"
#include "usblib/usbcdc.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdcdc.h"
#include "utils/ustdlib.h"
#include "usb-serial-structs.h"

#define SYSTICKS_PER_SECOND     100

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3
#define ALL_LEDS (RED_LED|BLUE_LED|GREEN_LED)

#define USB_IRQ (44 + 16)

#define PRIVILEGED_ACTION(x) \
    rtos_internal_elevate_privileges(); \
    (x); \
    rtos_internal_drop_privileges();

extern void USBDeviceIntHandlerInternal(uint32_t ui32Index, uint32_t ui32Status);
extern void rtos_internal_elevate_privileges();
extern void rtos_internal_drop_privileges();

// USB Device state
bool usb_ready = 0;
uint32_t usb_device_driver_status = 0;

// LED State
int led_on = 0;

// UART State
int uart_on = 0;

bool tick_irq(void) {
    rtos_timer_tick();
    return true;
}

int usb_device_irq(void) {
    // Get the controller interrupt status.
    usb_device_driver_status = MAP_USBIntStatusControl(USB0_BASE);

    // Disable the USB interrupt, otherwise it will constantly fire
    // until handled - which we're about to do.
    ROM_IntDisable(USB_IRQ);

    // Wake up the USB device driver task
    rtos_interrupt_event_raise(RTOS_INTERRUPT_EVENT_ID_USB_DEVICE);

    return true;
}

void task_usb_device_driver_fn(void) {
    while(1) {
        rtos_signal_wait(RTOS_SIGNAL_ID_USB_DEVICE_INTERRUPT);
        // Call the internal handler.
        USBDeviceIntHandlerInternal(0, usb_device_driver_status);

        PRIVILEGED_ACTION(IntEnable(USB_IRQ));
    }
}

#define USB_SERIAL_BUFFER_SIZE 128

typedef struct _usb_write_buffer_t {
    uint8_t buffer[USB_SERIAL_BUFFER_SIZE];
    uint32_t index;
} usb_write_buffer_t;

typedef struct _usb_read_buffer_t {
    uint8_t buffer[USB_SERIAL_BUFFER_SIZE];
    uint32_t index;
} usb_read_buffer_t;

usb_write_buffer_t usb_write_buffer = {{0}, 0};
usb_read_buffer_t usb_read_buffer = {{0}, 0};

void task_usb_writer_fn(void) {
    while(1) {
        rtos_signal_wait( RTOS_SIGNAL_ID_USB_WRITE );

        rtos_mutex_lock( RTOS_MUTEX_ID_WRITER_LOCK );

        // Ignore any writes that happen while the USB device isn't ready
        if(usb_ready) {
            uint32_t space = USBBufferSpaceAvailable((tUSBBuffer *)&g_sTxBuffer);
            // If we somehow overflowed the USB buffer, ignore the write
            if( space > usb_write_buffer.index ) {
                USBBufferWrite((tUSBBuffer *)&g_sTxBuffer,
                                usb_write_buffer.buffer,
                                usb_write_buffer.index);
            }
        }
        usb_write_buffer.index = 0;

        rtos_mutex_unlock( RTOS_MUTEX_ID_WRITER_LOCK );
    }
}

void task_usb_reader_fn(void) {
    while(1) {
        rtos_signal_wait( RTOS_SIGNAL_ID_USB_RX_PACKET );

        rtos_mutex_lock( RTOS_MUTEX_ID_READER_LOCK );

        usb_read_buffer.index +=
            USBBufferRead((tUSBBuffer *)&g_sRxBuffer,
                          (uint8_t*)(usb_read_buffer.buffer+usb_read_buffer.index),
                          USB_SERIAL_BUFFER_SIZE-usb_read_buffer.index);

        rtos_mutex_unlock( RTOS_MUTEX_ID_READER_LOCK );
    }
}

void usb_puts(char *s, uint32_t len) {
    rtos_mutex_lock( RTOS_MUTEX_ID_WRITER_LOCK );
    ustrncpy((char*)usb_write_buffer.buffer, s, len);
    usb_write_buffer.index = len;
    rtos_mutex_unlock( RTOS_MUTEX_ID_WRITER_LOCK );

    rtos_signal_send(RTOS_TASK_ID_TASK_USB_WRITER, RTOS_SIGNAL_ID_USB_WRITE);
}

void usb_print(char *s) {
    usb_puts(s, ustrlen(s));
}

void usb_gets_nonblocking(char **dest, uint32_t *n) {
    rtos_mutex_lock( RTOS_MUTEX_ID_READER_LOCK );
    *dest = (char*)usb_read_buffer.buffer;
    *n = usb_read_buffer.index;
    usb_read_buffer.index = 0;
    rtos_mutex_unlock( RTOS_MUTEX_ID_READER_LOCK );
}

void command_info(int arg) {
    usb_print("\n\reChronos RTOS + MPU + USB Stack");
    usb_print("\n\rDemonstrates a USB stack isolated from the RTOS kernel,");
    usb_print("\n\rperipherals & data associated with other tasks.");
    usb_print("\n\r");
    usb_print("\n\rThe USB stack emulates a serial device (this console), and eChronos");
    usb_print("\n\rallows only a very small communication pathway between tasks.");
    usb_print("\n\r");
    usb_print("\n\rCOMMANDS:");
    usb_print("\n\r    led on - enable the led blinking task");
    usb_print("\n\r    led off - disable the led blinking task");
    usb_print("\n\r    uart on - enable the uart keepalive task");
    usb_print("\n\r    uart off - disable the uart keepalive task");
    usb_print("\n\r");
    usb_print("\n\rDEMOS: (all executed in console task)");
    usb_print("\n\r    demo kernel - attempt to access kernel data");
    usb_print("\n\r    demo peripheral - attempt to disable RGB LED peripheral (directly)");
    usb_print("\n\r    demo intertask - attempt to write to USB stack state");
    usb_print("\n\r    demo interrupt - attempt to disable all interrupts");
}

void command_led(int arg) {
    usb_print("\n\r");
    if(arg) {
        led_on = 1;
        usb_print("Enabling");
    } else {
        led_on = 0;
        usb_print("Disabling");
    }
    usb_print(" the RGB LED task");
}

void command_uart(int arg) {
    usb_print("\n\r");
    if(arg) {
        uart_on = 1;
        usb_print("Enabling");
    } else {
        uart_on = 0;
        usb_print("Disabling");
    }
    usb_print(" the UART keepalive task");
}

extern uint32_t rtos_internal_current_task;
void command_kernel(int arg) {
    usb_print("\n\rAttempting to access kernel data from console task...");
    uint32_t t = (uint32_t)rtos_internal_current_task;
    ++t;
}

void command_peripheral(int arg) {
    usb_print("\n\rAttempting to disable RGB LED peripheral from console task...");
    SysCtlPeripheralDisable(SYSCTL_PERIPH_GPIOF);
}

void command_usb_state(int arg) {
    usb_print("\n\rAttempting to write to USB stack state from console task...");
    g_pui8USBTxBuffer[10] = 42;
}


void command_interrupt(int arg) {
    usb_print("\n\rAttempting to disable interrupts from console task...");
    ROM_IntMasterDisable();
}


struct command {
    char *name;
    void (*command)(int arg);
    int arg_to_pass;
};

struct command commands[] = {
    { "info", command_info, 0 },
    { "led on", command_led, 1 },
    { "led off", command_led, 0 },
    { "uart on", command_uart, 1 },
    { "uart off", command_uart, 0 },
    { "demo kernel", command_kernel, 0 },
    { "demo peripheral", command_peripheral, 0 },
    { "demo intertask", command_usb_state, 0 },
    { "demo interrupt", command_interrupt, 0 },
    };

void parse_command(char *s) {

    int did_command = 0;
    for(int i = 0; i != sizeof(commands)/sizeof(struct command); ++i) {
        if(ustrcmp(s, commands[i].name) == 0) {
            commands[i].command(commands[i].arg_to_pass);
            did_command = 1;
            break;
        }
    }

    if(!did_command) {
        usb_print("\n\rUnknown command: ");
        usb_print(s);
    }
}

#define CBUFFER_SIZE 128

void task_console_fn(void) {
    char cbuffer[CBUFFER_SIZE];
    uint32_t cindex = 0;

    while(1) {
        rtos_sleep(5);

        char *s;
        uint32_t len;

        usb_gets_nonblocking(&s, &len);

        if(len > 0) {
            // Echo back what the user is typing
            usb_puts(s, len);
        }

        // Add the new characters to the console buffer
        ustrncpy((char*)(cbuffer+cindex), s, len);
        cindex += len;

        for(int i = 0; i != len; ++i) {
            if(s[i] == '\r') {
                cbuffer[cindex - len + i] = '\0';
                parse_command(cbuffer);
                cindex = 0;
                usb_print("\n\r>>> ");
            }
        }

    }
}

void task_uart_fn(void) {
    while(1) {
        if(uart_on) {
            UARTprintf("I am a UART spitting out text...\n");
        }
        rtos_sleep(50);
    }
}


void task_blink_fn(void) {
    while(1) {

        if(led_on) {
            GPIOPinWrite(GPIO_PORTF_BASE, ALL_LEDS, RED_LED);
            rtos_signal_wait( RTOS_SIGNAL_ID_BLINK_DELAY );
            GPIOPinWrite(GPIO_PORTF_BASE, ALL_LEDS, GREEN_LED);
            rtos_signal_wait( RTOS_SIGNAL_ID_BLINK_DELAY );
            GPIOPinWrite(GPIO_PORTF_BASE, ALL_LEDS, BLUE_LED);
            rtos_signal_wait( RTOS_SIGNAL_ID_BLINK_DELAY );
        } else {
            GPIOPinWrite(GPIO_PORTF_BASE, ALL_LEDS, 0);
            rtos_sleep(10);
        }
    }
}

static void SetControlLineState(uint16_t ui16State) {
}

static bool SetLineCoding(tLineCoding *psLineCoding) {
    // We only support 115200/8/n/1
    if(psLineCoding->ui32Rate == 115200 &&
       psLineCoding->ui8Databits == 8 &&
       psLineCoding->ui8Parity == USB_CDC_PARITY_NONE &&
       psLineCoding->ui8Stop == USB_CDC_STOP_BITS_1) {
        return true;
    } else {
        return false;
    }
}

static void GetLineCoding(tLineCoding *psLineCoding) {
    psLineCoding->ui32Rate = 115200;
    psLineCoding->ui8Databits = 8;
    psLineCoding->ui8Parity = USB_CDC_PARITY_NONE;
    psLineCoding->ui8Stop = USB_CDC_STOP_BITS_1;
}


// This function is called by the CDC driver to perform control-related
// operations on behalf of the USB host.  These functions include setting
// and querying the serial communication parameters, setting handshake line
// states and sending break conditions.
uint32_t ControlHandler(void *pvCBData, uint32_t ui32Event,
               uint32_t ui32MsgValue, void *pvMsgData) {

    // Which event are we being asked to process?
    switch(ui32Event)
    {
        // We are connected to a host and communication is now possible.
        case USB_EVENT_CONNECTED:
            usb_ready = true;

            // Flush our buffers.
            USBBufferFlush(&g_sTxBuffer);
            USBBufferFlush(&g_sRxBuffer);

            break;

        // The host has disconnected.
        case USB_EVENT_DISCONNECTED:
            usb_ready = false;
            break;

        // Return the current serial communication parameters.
        case USBD_CDC_EVENT_GET_LINE_CODING:
            GetLineCoding(pvMsgData);
            break;

        // Set the current serial communication parameters.
        case USBD_CDC_EVENT_SET_LINE_CODING:
            SetLineCoding(pvMsgData);
            break;

        // Set the current serial communication parameters.
        case USBD_CDC_EVENT_SET_CONTROL_LINE_STATE:
            SetControlLineState((uint16_t)ui32MsgValue);
            break;

        // Ignore SUSPEND, RESUME, BREAKS for now.
        case USBD_CDC_EVENT_SEND_BREAK:
        case USBD_CDC_EVENT_CLEAR_BREAK:
        case USB_EVENT_SUSPEND:
        case USB_EVENT_RESUME:
            break;

        // We don't expect to receive any other events.  Ignore any that show
        // up in a release build or hang in a debug build.
        default:
#ifdef DEBUG
            while(1);
#else
            break;
#endif

    }

    return(0);
}

// This function is called by the CDC driver to notify us of any events
// related to operation of the transmit data channel (the IN channel carrying
// data to the USB host).
uint32_t TxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue,
          void *pvMsgData) {
    // Which event have we been sent?
    switch(ui32Event)
    {
        case USB_EVENT_TX_COMPLETE:
            // Since we are using the USBBuffer, we don't need to do anything
            // here.
            break;

        // We don't expect to receive any other events.  Ignore any that show
        // up in a release build or hang in a debug build.
        default:
#ifdef DEBUG
            while(1);
#else
            break;
#endif

    }
    return(0);
}

// This function is called by the CDC driver to notify us of any events
// related to operation of the receive data channel (the OUT channel carrying
// data from the USB host).
uint32_t RxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue,
          void *pvMsgData) {

    // Which event are we being sent?
    switch(ui32Event)
    {
        // A new packet has been received.
        case USB_EVENT_RX_AVAILABLE:
        {
            rtos_signal_send(RTOS_TASK_ID_TASK_USB_READER, RTOS_SIGNAL_ID_USB_RX_PACKET);
            break;
        }

        // We are being asked how much unprocessed data we have still to
        // process. We return 0 if the UART is currently idle or 1 if it is
        // in the process of transmitting something. The actual number of
        // bytes in the UART FIFO is not important here, merely whether or
        // not everything previously sent to us has been transmitted.
        {
            // We're always fine
            break;
        }

        // We are being asked to provide a buffer into which the next packet
        // can be read. We do not support this mode of receiving data so let
        // the driver know by returning 0. The CDC driver should not be sending
        // this message but this is included just for illustration and
        // completeness.
        case USB_EVENT_REQUEST_BUFFER:
        {
            return(0);
        }

        // We don't expect to receive any other events.  Ignore any that show
        // up in a release build or hang in a debug build.
        default:
#ifdef DEBUG
            while(1);
#else
            break;
#endif
    }

    return(0);
}


int main(void) {

    // Initialize the floating-point unit.
    InitializeFPU();

    // Set the clocking to run from the PLL at 50 MHz.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    // Set up the systick interrupt used by the RTOS
    ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / SYSTICKS_PER_SECOND);
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();

    // Initialize the UART for stdio so we can use UARTPrintf
    InitializeUARTStdio();

    // Enable the GPIO pin for the LEDs (PF3).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, RED_LED|GREEN_LED|BLUE_LED);

    // Initialize the USB-related peripherals
    usb_ready = false;
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_5 | GPIO_PIN_4);
    USBBufferInit(&g_sTxBuffer);
    USBBufferInit(&g_sRxBuffer);
    USBStackModeSet(0, eUSBModeForceDevice, 0);
    USBDCDCInit(0, &g_sCDCDevice);

    // Actually start the RTOS
    UARTprintf("Starting RTOS...\n");
    rtos_start();

    /* Should never reach here, but if we do, an infinite loop is
       easier to debug than returning somewhere random. */
    for (;;) ;
}
