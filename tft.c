/*
 * File:   tft.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 6. November 2023, 18:45
 */

#include "tft.h"

/**
 * Does a hardware reset.
 */
static void hwReset(void) {
    PORT_DISP &= ~(1 << PIN_DRST);
    _delay_ms(10);
    PORT_DISP |= (1 << PIN_DRST);
}

static void displaySel(void) {
    PORT_DISP &= ~(1 << PIN_DCS);
}

static void displayDes(void) {
    PORT_DISP |= (1 << PIN_DCS);
}

/**
 * Sets display to send a command.
 */
static void displaySetCmd(void) {
    PORT_DISP &= ~(1 << PIN_DDC);
}

/**
 * Sets display to send data.
 */
static void displaySetData(void) {
    PORT_DISP |= (1 << PIN_DDC);
}

/**
 * Sends the given command to the display.
 *
 * @param cmd
 */
static void displayCmd(uint8_t cmd) {
    displaySetCmd();
    transmit(cmd);
}

/**
 * Sends the given data to the display.
 *
 * @param data
 */
static void displayData(uint8_t data) {
    displaySetData();
    transmit(data);
}

/**
 * Sets horizontal and/or vertical flip.
 *
 * @param hflip
 * @param vflip
 */
static void madctl(bool hflip, bool vflip) {
    // Memory data access control
    uint8_t madctl = 0b00110110;
    madctl |= (HFLIP << 7);
    madctl |= (VFLIP << 6);
    madctl |= (BGR << 3);

    if (hflip) {
        // Row Address Order (MY)
        madctl ^= (1 << 7);
    }
    if (vflip) {
        // Column Address Order (MX)
        madctl ^= (1 << 6);
    }

    displaySel();
    displayCmd(MADCTL);
    displayData(madctl);
    displayDes();
}

/**
 * Sets the given column start and end address.
 *
 * @param xs start address
 * @param xe end address
 */
static void caset(x_t xs, x_t xe) {
    displaySel();
    displayCmd(CASET);
    displayData(xs >> 8);
    displayData(xs);
    displayData(xe >> 8);
    displayData(xe);
    displayDes();
}

/**
 * Sets the given row start and end address.
 *
 * @param ys start address
 * @param ye end address
 */
static void raset(y_t ys, y_t ye) {
    displaySel();
    displayCmd(RASET);
    displayData(ys >> 8);
    displayData(ys);
    displayData(ye >> 8);
    displayData(ye);
    displayDes();
}

void initDisplay(void) {
    _delay_ms(100);

    // Hardware reset
    hwReset();

    // TODO necessary?
    _delay_ms(10);

    // Software reset
    displaySel();
    displayCmd(SWRESET);
    displayDes();

    // TODO necessary?
    _delay_ms(10);

    // Sleep out & booster on
    displaySel();
    displayCmd(SLPOUT);
    displayDes();

    // Partial off (Normal)
    displaySel();
    displayCmd(NORON);
    displayDes();

    // Display Inversion on/off
    displaySel();
    displayCmd(INVOFF + INVERT); // INVOFF + 1 = INVON
    displayDes();

    // Interface pixel format
    displaySel();
    displayCmd(COLMOD);
    displayData(0b00111101);
    displayDes();

    // Display on
    displaySel();
    displayCmd(DISPON);
    displayDes();

    // Sleep in & booster off
    // displaySel();
    // displayCmd(SLPIN);
    // displayDes();

    printString("Display init done\r\n");
}

void demoDisplay(void) {
    // TODO
}

void drawPixel(x_t x, y_t y, uint16_t color) {
    // TODO
}

void drawCircle(x_t x, y_t y, uint16_t radius, uint16_t color) {
    // TODO
}

void drawRectangle(x_t x, y_t y, width_t width, height_t height,
                   uint16_t color) {
    // TODO
}

void writeStart(void) {
    // Memory write
    displaySel();
    displayCmd(RAMWR);
    displaySetData();
}

void writeRestart(void) {
    displaySel();
}

void writeByte(uint8_t byte) {
    // Memory write
    transmit(byte);
}

void writeEnd(void) {
    // Memory write
    displayDes();
}

void fillArea(x_t x, x_t y,
              width_t width, height_t height,
              uint16_t color) {

    madctl(false, false);

    // X address start/end
    uint16_t xs = x;
    uint16_t xe = x + width - 1;
    caset(xs, xe);

    // Y address start/end
    uint16_t ys = y;
    uint16_t ye = y + height - 1;
    raset(ys, ye);

    writeStart();

    bytes_t pixels = (bytes_t)width * (bytes_t)height;
    for (bytes_t i = 0; i < pixels; i++) {
        transmit(color >> 8);
        transmit(color);
    }

    writeEnd();
}

void setArea(x_t x, y_t y,
             width_t width, height_t height,
             bool hflip, bool vflip) {

    madctl(hflip, vflip);

    // X address start/end
    uint16_t xs = x;
    uint16_t xe = x + width - 1;
    if (hflip) {
        xs = DISPLAY_WIDTH - x - width;
        xe = DISPLAY_WIDTH - x - 1;
    }
    caset(xs, xe);

    // Y address start/end
    uint16_t ys = y;
    uint16_t ye = y + height - 1;
    if (vflip) {
        ys = DISPLAY_HEIGHT - y - height;
        ye = DISPLAY_HEIGHT - y - 1;
    }
    raset(ys, ye);
}

void writeData(const __flash uint8_t *bitmap,
               width_t width, height_t height,
               space_t space, uint16_t bg, uint16_t fg) {
    writeStart();
    writeSpace(bitmap, width, height, space, bg, fg);
    writeEnd();
}
