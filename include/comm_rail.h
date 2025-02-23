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

void comm_send_text_packet_int16(uint8_t high_hyle, uint8_t low_byte, int16_t packet)
{
    uint8_t bin_packet[9] = { STX, high_hyle, low_byte, DOT, DOT, DOT, DOT, ETX, 0x00 };
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

void process_enquiry()
{

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