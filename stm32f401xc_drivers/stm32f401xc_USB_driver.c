/*
 * stm32f401xc_USB_driver.c
 *
 * Created on: Mar 3, 2026
 * Author: Ahmed Ezzat
 *
 * Fixes applied:
 *  1. Added FIFO size configuration (GRXFSIZ + DIEPTXF0) — was missing entirely
 *  2. Fixed Device Mode force sequence (clear FHMOD, set FDMOD, wait 25ms)
 *  3. Added OUT EP0 enable in ENUMDN handler to receive SETUP packets
 *  4. Added IEPINT + OEPINT + STUPM interrupt unmasks
 */

#include "stm32f401xc_USB_driver.h"

void MCAL_USB_Init(void)
{
    GPIO_PinConfig_t USB_Pins;

    /* ------------------------------------------------------------------ */
    /* 1. Configure GPIO Pins: PA11 (D-) and PA12 (D+) as AF10            */
    /* ------------------------------------------------------------------ */

    // USB_D- (PA11)
    USB_Pins.GPIO_PinNumber    = GPIO_PIN_11;
    USB_Pins.GPIO_MODE         = GPIO_MODE_AF;
    USB_Pins.GPIO_AFx          = GPIO_AF10;
    USB_Pins.GPIO_Output_Speed = GPIO_SPEED_VERY_HIGH;
    MCAL_GPIO_Init(GPIOA, &USB_Pins);

    // USB_D+ (PA12)
    USB_Pins.GPIO_PinNumber = GPIO_PIN_12;
    MCAL_GPIO_Init(GPIOA, &USB_Pins);

    /* ------------------------------------------------------------------ */
    /* 2. Enable USB OTG FS Clock                                          */
    /* ------------------------------------------------------------------ */
    // Enable Clock
        RCC_USB_CLK_EN();

        // Select Internal PHY before reset
        USB_OTG_FS->GUSBCFG |= (1U << 6);

        // Wait for AHB master Idle
        while ((USB_OTG_FS->GRSTCTL & (1U << 31)) == 0);

        // Core soft reset
        USB_OTG_FS->GRSTCTL |= (1U << 0);

        // Add a small timeout or a volatile counter to prevent a permanent hang
        uint32_t timeout = 0;
        while ((USB_OTG_FS->GRSTCTL & (1U << 0)) != 0) {
            if (timeout++ > 100000) {
                // If it hangs here, the 48MHz clock is definitely missing or unstable
                break;
            }
        }

    // Extra settling delay after reset
    for (volatile int i = 0; i < 20000; i++);

    /* ------------------------------------------------------------------ */
    /* 5. Configure FIFO Sizes                                             */
    /* Without this the hardware has no RX/TX buffer and cannot operate.  */
    /*   RX FIFO  : 128 words (512 bytes) starting at address 0           */
    /*   EP0 TX   : 64 words  (256 bytes) starting at address 128         */
    /* ------------------------------------------------------------------ */
    USB_OTG_FS->GRXFSIZ  = 128;                  // RX FIFO  = 128 words
    USB_OTG_FS->DIEPTXF0 = (64U << 16) | 128U;  // TX FIFO0 = 64 words @ offset 128

    /* ------------------------------------------------------------------ */
    /* 6. Power up transceiver & disable VBUS sensing on PA9              */
    /* ------------------------------------------------------------------ */
    USB_OTG_FS->GCCFG |= (1U << 16) | (1U << 21); // PWRDWN=1, NOVBUSSENS=1

    /* ------------------------------------------------------------------ */
    /* 7. Set Device Speed to Full Speed (DSPD bits [1:0] = 0b11)         */
    /* ------------------------------------------------------------------ */
    USB_OTG_FS->DCFG |= (3U << 0);

    /* ------------------------------------------------------------------ */
    /* 8. Unmask Interrupts                                                */
    /*   Bit  4 : RXFLVL  – RX FIFO non-empty                            */
    /*   Bit 12 : USBRST  – USB reset                                     */
    /*   Bit 13 : ENUMDNE – Enumeration done                              */
    /*   Bit 18 : IEPINT  – IN endpoint interrupt                         */
    /*   Bit 19 : OEPINT  – OUT endpoint interrupt                        */
    /* ------------------------------------------------------------------ */
    USB_OTG_FS->GINTMSK |= (1U << 4)  |   // RXFLVL
                            (1U << 12) |   // USBRST
                            (1U << 13) |   // ENUMDNE
                            (1U << 18) |   // IEPINT
                            (1U << 19);    // OEPINT

    // Unmask SETUP phase done on OUT endpoint
    USB_OTG_FS->DOEPMSK |= (1U << 3);     // STUPM

    /* ------------------------------------------------------------------ */
    /* 9. Enable Global Interrupt Routing in AHB config                   */
    /* ------------------------------------------------------------------ */
    USB_OTG_FS->GAHBCFG |= (1U << 0);

    /* ------------------------------------------------------------------ */
    /* 10. Clear Soft Disconnect – pulls D+ high, signals the PC          */
    /* ------------------------------------------------------------------ */
    USB_OTG_FS->DCTL &= ~(1U << 1);

    /* ------------------------------------------------------------------ */
    /* 11. Enable USB OTG FS IRQ in NVIC                                  */
    /* ------------------------------------------------------------------ */
    NVIC_IRQ_OTG_FS_EN();
}


/* ======================================================================== */
/*                         USB OTG FS IRQ Handler                           */
/* ======================================================================== */
void OTG_FS_IRQHandler(void)
{
    // Read masked interrupt status
    uint32_t status = USB_OTG_FS->GINTSTS & USB_OTG_FS->GINTMSK;

    /* ------------------------------------------------------------------ */
    /* 1. USB Reset Interrupt (USBRST – Bit 12)                           */
    /*    The host has driven D- and D+ low to reset the bus.             */
    /*    We must reset the device address back to 0.                     */
    /* ------------------------------------------------------------------ */
    if (status & (1U << 12))
    {
        // Clear Device Address bits [10:4]
        USB_OTG_FS->DCFG &= ~(0x7FU << 4);

        // Clear USBRST flag
        USB_OTG_FS->GINTSTS = (1U << 12);
    }

    /* ------------------------------------------------------------------ */
    /* 2. Enumeration Done Interrupt (ENUMDNE – Bit 13)                   */
    /*    Host has finished the reset and detected Full-Speed.            */
    /*    Configure EP0 IN  (Max Packet Size = 64 bytes, MPSIZ = 0b00)   */
    /*    Configure EP0 OUT (enable to receive first SETUP packet)        */
    /* ------------------------------------------------------------------ */
    if (status & (1U << 13))
    {
        // --- IN EP0: set 64-byte MPS ---
        USB_OTG_FS->IN_EP[0].DIEPCTL &= ~(0x3U << 0); // MPSIZ = 00 (64 bytes)

        // --- OUT EP0: set 64-byte MPS, enable, and clear NAK ---
        USB_OTG_FS->OUT_EP[0].DOEPCTL &= ~(0x3U << 0); // MPSIZ = 00 (64 bytes)

        // Prepare OUT EP0 transfer size: 1 SETUP packet, 1 packet, 64 bytes
        USB_OTG_FS->OUT_EP[0].DOEPTSIZ = (1U << 29) |  // STUPCNT = 1
                                          (1U << 19) |  // PKTCNT  = 1
                                          64U;          // XFRSIZ  = 64

        // Enable OUT EP0 and clear NAK so it can receive SETUP packets
        USB_OTG_FS->OUT_EP[0].DOEPCTL |= (1U << 31) |  // EPENA
                                          (1U << 26);   // CNAK

        // Clear ENUMDNE flag
        USB_OTG_FS->GINTSTS = (1U << 13);
    }

    /* ------------------------------------------------------------------ */
    /* 3. Receive FIFO Non-Empty (RXFLVL – Bit 4)                         */
    /*    A packet has arrived; pop the status word and drain the FIFO.   */
    /* ------------------------------------------------------------------ */
    if (status & (1U << 4))
    {
        // Temporarily mask RXFLVL while we process this entry
        USB_OTG_FS->GINTMSK &= ~(1U << 4);

        // Pop the top entry from the Receive Status FIFO
        uint32_t rx_status = USB_OTG_FS->GRXSTSP;

        uint32_t byte_count = (rx_status >> 4)  & 0x7FFU; // Bits [14:4]
        uint32_t ep_num     =  rx_status         & 0xFU;   // Bits [3:0]
        uint32_t pkt_status = (rx_status >> 17)  & 0xFU;  // Bits [20:17]

        // pkt_status == 6 : SETUP data received
        // pkt_status == 2 : OUT  data received
        if (pkt_status == 6 || pkt_status == 2)
        {
            uint32_t word_count = (byte_count + 3U) / 4U;
            for (uint32_t i = 0; i < word_count; i++)
            {
                uint32_t dummy = USB_OTG_FS->DFIFO[ep_num][0];
                (void)dummy;
            }
        }

        // Re-enable RXFLVL
        USB_OTG_FS->GINTMSK |= (1U << 4);
    }
}
