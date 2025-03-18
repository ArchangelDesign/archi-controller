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

#define P 0x50
#define S 0x53
#define V 0x56
#define O 0x4F
#define I 0x49
#define D 0x44
#define X 0x58
#define T 0x54
#define STX 0x02 // Start of Text
#define ETX 0x03 // End of text
#define EOT 0x04 // End of Transmission
#define ENQ 0x05 // Enquiry
#define ACK 0x06 
#define NAK 0x15 // Error response

#define DOT 0x2E // .
#define ZERO 0x30 // 0


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

uint8_t get_first_eot_index(uint8_t buffer[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == EOT) {
            return i;
        }
    }
    return 0; // this should never happen
}

uint8_t get_first_etx_index(uint8_t buffer[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ETX) {
            return i;
        }
    }
    return size - 2; // this should never happen
}


uint8_t get_first_enq_index(uint8_t buffer[], uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        if (buffer[i] == ENQ) {
            return i;
        }
    }
    return size - 1; // this should never happen
}
