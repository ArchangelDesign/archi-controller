#pragma once

#include <EEPROM.h>
#include <Arduino.h>

#define ADDR_P 0
#define ADDR_I 4
#define ADDR_D 8

void ps_store_p(double p)
{
    EEPROM.put(ADDR_P, p);
}

void ps_store_i(double i)
{
    EEPROM.put(ADDR_I, i);
}

void ps_store_d(double d)
{
    EEPROM.put(ADDR_D, d);
}

double ps_read_p()
{
    double p;
    EEPROM.get(ADDR_P, p);

    return p;
}

double ps_read_i()
{
    double i;
    EEPROM.get(ADDR_I, i);
    return i;
}

double ps_read_d()
{
    double d;
    EEPROM.get(ADDR_D, d);
    return d;
}

double ps_get_or_store_p(double p)
{
    double stored_p = ps_read_p();
    if (stored_p < 0.01 || isnan(stored_p)) {
        ps_store_p(p);
        return p;
    } else {
        return stored_p;
    }
}

double ps_get_or_store_i(double i)
{
    double stored_i = ps_read_i();
    if (stored_i < 0.01 || stored_i > 10 || isnan(stored_i)) {
        ps_store_i(i);
        return i;
    } else {
        return stored_i;
    }
}

double ps_get_or_store_d(double d)
{
    double stored_d = ps_read_d();
    if (stored_d < 0.01 || stored_d > 10 || isnan(stored_d)) {
        ps_store_d(d);
        return d;
    } else {
        return stored_d;
    }
}