// Support for extracting the hardware chip id on stm32
//
// Copyright (C) 2019  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "generic/canserial.h" // canserial_set_uuid
#include "generic/usb_cdc.h" // usb_fill_serial
#include "generic/usbstd.h" // usb_string_descriptor
#include "internal.h" // UID_BASE
#include "sched.h" // DECL_INIT

#define CHIP_UID_LEN 12

// Use fixed uuid for main board in openorangestorm, so we can set it up with ssh-ing into it first.
// This only affects STM32 builds, so it doesn't change anything for rp2040 builds.
// This results in the CAN ID 7dfcfd19b04d
static const uint8_t my_uuid[CHIP_UID_LEN] __attribute__((unused)) = {
    0xAF, 0xFE, 0xAF, 0xFE, 0xAF, 0xFE, 0xAF, 0xFE,
    0xAF, 0xFE, 0xAF, 0xFE,
};

static struct {
    struct usb_string_descriptor desc;
    uint16_t data[CHIP_UID_LEN * 2];
} cdc_chipid;

struct usb_string_descriptor *
usbserial_get_serialid(void)
{
   return &cdc_chipid.desc;
}

void
chipid_init(void)
{
    if (CONFIG_USB_SERIAL_NUMBER_CHIPID)
        usb_fill_serial(&cdc_chipid.desc, ARRAY_SIZE(cdc_chipid.data)
                        , (void*)UID_BASE);
    if (CONFIG_CANBUS)
        //canserial_set_uuid((void*)UID_BASE, CHIP_UID_LEN);
        canserial_set_uuid((void*)my_uuid, CHIP_UID_LEN);
}
DECL_INIT(chipid_init);
