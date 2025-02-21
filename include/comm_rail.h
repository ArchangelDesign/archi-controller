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
    for (int i = 1; i < size - 1; i ++) {
        bcc ^= data[i];
    }
    d[size - 1] = bcc;
    Serial.write(d, size);
}

void comm_send_pv_update(int16_t temperature)
{
    uint8_t packet[9] = { STX, P, V, DOT, DOT, DOT, DOT, ETX, 0x00 };
    uint8_t index = 3;
    char buf[5];
    memset(buf, 0, 5);
    itoa(temperature, buf, 10);
    for (int i = 0; i < 5; i++) {
        if (buf[i] > 0) {
            packet[index] = buf[i];
            index++;
        }
    }
    comm_send_message(packet, 9);
}

void comm_process_incoming_buffer()
{
    if (cr_receive_buffer_size == 0) {
        return;
    }

    if (cr_receive_buffer_size >= MAX_BUFF_SIZE) {
        cr_receive_buffer_size = 0;
    }

    msm_current_temperature++;

}