#ifndef USB_SERIAL_H_
#define USB_SERIAL_H_

#include "main.h"

#ifdef USE_USB_OTG_FS
void usb_printf(const char* text, ...);
void usb_serial_deinit(void);
void usb_serial_init(void);
void activate_usb(void);
void deactivate_usb(void);
void write_usb(uint8_t byte);
void usb_write(uint8_t byte);
#endif


#endif
