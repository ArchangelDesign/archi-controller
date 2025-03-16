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

#include "protocol.h"
#include "measurements.h"

#define MAX_BUFF_SIZE 128

uint8_t cr_receive_buffer[MAX_BUFF_SIZE];
uint8_t cr_receive_buffer_size = 0;

void comm_process_incoming_buffer();
void comm_send_message(uint8_t* data, uint16_t size);
void comm_rail_loop();
void comm_send_pv_update(int16_t temperature);
void comm_send_sp_update(int16_t temperature);
void comm_send_text_packet_int16(uint8_t high_hyle, uint8_t low_byte, int16_t packet);
void comm_send_op_update(uint8_t power);

void comm_init()
{
    Serial.begin(SERIAL_SPEED);
    delay(200);
}

void data_received(uint8_t buff[], uint8_t size)
{
    uint8_t index = cr_receive_buffer_size;
    for (int i = 0; i < size; i++) {
        cr_receive_buffer[index] = buff[i];
        index++;
    }
    cr_receive_buffer_size = index + 1;
}

void comm_rail_loop()
{
    if (Serial.available() > 0) {
        uint8_t buff[16];
        uint8_t buffer_size = Serial.available();
        Serial.readBytes(buff, buffer_size);
        data_received(buff, buffer_size);
        comm_process_incoming_buffer();
    }
}

void comm_send_message(uint8_t* data, uint16_t size)
{
    uint8_t *d = data;
    uint8_t bcc = 0;
    for (unsigned int i = 1; i < size - 1; i ++) {
        bcc ^= data[i];
    }
    d[size - 1] = bcc;
    Serial.write(d, size);
}

void comm_send_text_packet_int16(uint8_t high_byte, uint8_t low_byte, int16_t packet)
{
    uint8_t bin_packet[9] = { STX, high_byte, low_byte, DOT, DOT, DOT, DOT, ETX, 0x00 };
    uint8_t index = 3;
    char buf[5];
    memset(buf, 0, 5);
    itoa(packet, buf, 10);
    for (int i = 0; i < 5; i++) {
        if (buf[i] > 0) {
            bin_packet[index] = buf[i];
            index++;
        }
    }
    comm_send_message(bin_packet, 9);
}

void comm_send_text_packet_double(uint8_t high_byte, uint8_t low_byte, double packet)
{
    char buf[5];
    dtostrf(packet, 5, 2, buf);
    uint8_t bin_packet[9] = { STX, high_byte, low_byte, ZERO, ZERO, ZERO, ZERO, ETX, 0x00 };
    memcpy(&bin_packet[3], &buf, 4);

    comm_send_message(bin_packet, 9);
}

void comm_send_ack()
{
    Serial.write((char*)ACK, 1);
}

void comm_send_pv_update(int16_t temperature)
{
    comm_send_text_packet_int16(P, V, temperature);
}

void comm_send_sp_update(int16_t temperature)
{
    comm_send_text_packet_int16(S, P, temperature);
}

void comm_send_op_update(uint8_t power)
{
    comm_send_text_packet_int16(O, P, power);
}

void comm_send_pid_p_update(double p)
{
    comm_send_text_packet_double(P, P, p);
}

void comm_send_pid_i_update(double i)
{
    comm_send_text_packet_double(P, I, i);
}

void comm_send_pid_d_update(double d)
{
    comm_send_text_packet_double(P, D, d);
}

void process_enquiry()
{
    uint8_t start = get_first_eot_index(cr_receive_buffer, cr_receive_buffer_size);
    uint8_t end = get_first_enq_index(cr_receive_buffer, cr_receive_buffer_size);
    // shift the buffer
    memmove(&cr_receive_buffer[start], &cr_receive_buffer[end + 1], (cr_receive_buffer_size - (end + 1)) - start);
    comm_send_ack();
}

void process_command()
{

}

void comm_process_incoming_buffer()
{
    if (cr_receive_buffer_size == 0) {
        return;
    }

    if (has_enq_byte(cr_receive_buffer, cr_receive_buffer_size)) {
        process_enquiry();
    }

    if (has_stx_byte(cr_receive_buffer, cr_receive_buffer_size) && has_etx_byte(cr_receive_buffer, cr_receive_buffer_size)) {
        process_command();
    }

    if (cr_receive_buffer_size >= MAX_BUFF_SIZE) {
        cr_receive_buffer_size = 0;
    }
}