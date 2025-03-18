/*
    ** Archi Controller - BGA Rework Station Controller **
    copyright (c) 2025 Black Horse Repairs LLC and contributors.
    All rights reserved.
    https://blackhorserepairs.com

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <Arduino.h>

#define ASCII_P 0x50
#define ASCII_S 0x53
#define ASCII_V 0x56
#define ASCII_O 0x4F
#define ASCII_I 0x49
#define ASCII_D 0x44
#define ASCII_X 0x58
#define ASCII_T 0x54
#define ASCII_STX 0x02 // Start of Text
#define ASCII_ETX 0x03 // End of text
#define ASCII_EOT 0x04 // End of Transmission
#define ASCII_ENQ 0x05 // Enquiry
#define ASCII_ACK 0x06 
#define ASCII_NAK 0x15 // Error response

#define DOT 0x2E // .
#define ZERO 0x30 // 0


bool has_enq_byte(uint8_t *buffer, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ASCII_ENQ) {
            return true;
        }
    }

    return false;
}


bool has_stx_byte(uint8_t *buffer, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ASCII_STX) {
            return true;
        }
    }

    return false;
}


bool has_etx_byte(uint8_t *buffer, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ASCII_ETX) {
            return true;
        }
    }

    return false;
}

/**
 * Returns true if the buffer contains a full packet
 */
bool has_full_packet(uint8_t *buffer, uint8_t size)
{
    bool has_start = false;
    bool has_end = false;
    bool has_bcc = false;

    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ASCII_STX) {
            has_start = true;
        }
        if (buffer[i] == ASCII_ETX) {
            has_end = true;
            // check for BCC
            has_bcc = (i + 2) <= size;
        }
    }

    return has_start && has_end && has_bcc;
}

/**
 * Returns true if the buffer contains a single full packet, false otherwise.
 */
bool is_single_packet(uint8_t *buffer, uint8_t size)
{
    uint8_t counter = 0;
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ASCII_ETX) {
            counter++;
        }
    }
    if (counter > 1) {
        return false;
    }
    counter = 0;
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ASCII_STX) {
            counter++;
        }
    }
    if (counter > 1) {
        return false;
    }
    counter = 0;
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ASCII_EOT) {
            counter++;
        }
    }
    if (counter > 1) {
        return false;
    }

    return true;
}

uint8_t get_first_eot_index(uint8_t buffer[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ASCII_EOT) {
            return i;
        }
    }
    return 0; // this should never happen
}

uint8_t get_first_etx_index(uint8_t buffer[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ASCII_ETX) {
            return i;
        }
    }
    return size - 2; // this should never happen
}


uint8_t get_first_enq_index(uint8_t buffer[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ASCII_ENQ) {
            return i;
        }
    }
    return size - 1; // this should never happen
}
