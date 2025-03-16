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