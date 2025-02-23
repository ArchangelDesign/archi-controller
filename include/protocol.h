#pragma once

#include <Arduino.h>

#define P 0x50
#define S 0x53
#define V 0x56
#define O 0x4F
#define STX 0x02 // Start of Text
#define ETX 0x03 // End of text
#define EOT 0x04 // End of Transmission
#define ENQ 0x05 // Enquiry
#define ACK 0x05 
#define NAK 0x15 // Error response

#define DOT 0x2E // .


bool has_enq_byte(uint8_t *buffer, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ENQ) {
            return true;
        }
    }

    return false;
}


bool has_stx_byte(uint8_t *buffer, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == STX) {
            return true;
        }
    }

    return false;
}


bool has_etx_byte(uint8_t *buffer, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ETX) {
            return true;
        }
    }

    return false;
}

bool is_single_packet(uint8_t *buffer, uint8_t size)
{
    uint8_t counter = 0;
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ETX) {
            counter++;
        }
    }
    if (counter > 1) {
        return true;
    }
    counter = 0;
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == STX) {
            counter++;
        }
    }
    if (counter > 1) {
        return true;
    }
    counter = 0;
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == EOT) {
            counter++;
        }
    }
    if (counter > 1) {
        return true;
    }

    return false;
}