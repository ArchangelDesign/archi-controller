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

/**
 * Each command starts with EOT and last byte is BCC
 * Second to last byte is ETX
 * Bytes of index 1 & 2 are the same and represent first digit of the address
 * Bytes of index 3 & 4 are the same and represend second digit of the address
 * The message starts with STX byte and ends with ETX
 * ETX should be second to last byte.
 */
bool process_comm_rail_command(uint8_t *buffer, uint8_t size);

/**
 * Returns the sie of the message body (from STX to ETX)
 */
uint8_t get_message_size(uint8_t *buffer, uint8_t size);

/**
 * Execute on the command sent from the computer
 */
bool process_command_from_master(char *command, char *value, uint8_t address);

bool process_message_body(char *message, uint8_t size, uint8_t addr_high, uint8_t addr_low);

bool is_pid_p_update_command(char *command);
bool is_pid_i_update_command(char *command);
bool is_pid_d_update_command(char *command);
bool is_set_point_update_command(char *command);
bool is_start_command(char *command);
bool is_stop_command(char *commend);
bool is_pause_command(char *command);
bool is_ramp_update_command(char *command);

bool process_comm_rail_command(uint8_t *buffer, uint8_t size)
{
    if (buffer[0] != ASCII_EOT) {
        return false;
    }

    if (buffer[size - 2] != ASCII_ETX) {
        return false;
    }

    uint8_t addr_high = buffer[1];
    uint8_t addr_low = buffer[3];

    if (addr_high != buffer[2]) {
        // address validation failed
        return false;
    }

    if (addr_low != buffer[4]) {
        // address validation failed
        return false;
    }

    // @TODO: verify BCC
    // uint8_t bcc = buffer[size - 1];

    uint8_t message_size = get_message_size(buffer, size);
    char *message = new char[message_size];
    memcpy(message, &buffer[6], message_size);

    bool result = process_message_body(message, message_size, addr_high, addr_low);
    free(message);

    return result;
}

uint8_t get_message_size(uint8_t *buffer, uint8_t size)
{
    // 4 bytes for address, 1 byte for EOT, 1 byte for STX, 1 byte for ETX and 1 byte for BCC
    // the rest is the message body
    return size - 8;
}

bool process_message_body(char *message, uint8_t size, uint8_t addr_high, uint8_t addr_low)
{
    char *command = new char[2];
    char *value = new char[size - 2];
    char *address = new char[2];

    command[0] = message[0];
    command[1] = message[1];
    address[0] = addr_high;
    address[1] = addr_low;
    uint8_t addr = atoi(address);

    memcpy(value, &message[3], size - 2);

    bool result = process_command_from_master(command, value, addr);

    free(command);
    free(value);
    free(address);

    return result;
}

bool process_command_from_master(char *command, char *value, uint8_t address)
{
    if (is_pid_p_update_command(command)) {
        msm_p = atof(value);
        return true;
    }

    if (is_pid_i_update_command(command)) {
        msm_i = atof(value);
        return true;
    }

    if (is_pid_d_update_command(command)) {
        msm_d = atof(value);
        return true;
    }

    if (is_set_point_update_command(command)) {
        msm_zone_a_target_set_point = atoi(value);
        return true;
    }

    return false;
}

bool is_pid_p_update_command(char *command)
{
    return (command[0] == ASCII_X && command[1] == ASCII_P);
}

bool is_pid_i_update_command(char *command)
{
    return (command[0] == ASCII_T && command[1] == ASCII_I);
}

bool is_pid_d_update_command(char *command)
{
    return (command[0] == ASCII_T && command[1] == ASCII_D);
}

bool is_set_point_update_command(char * command)
{
    return (command[0] == ASCII_S && command[1] == ASCII_P);
}