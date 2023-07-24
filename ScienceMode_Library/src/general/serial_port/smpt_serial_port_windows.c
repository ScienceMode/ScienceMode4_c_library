/***************************************************************************//**
* \file smpt_ll_serial_port_windows.c
* \copyright MPL and LGPL HASOMED GmbH 2013-2022
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Copyright (c) 2013-2023, MPL and LGPL HASOMED GmbH
*
* Alternatively, the contents of this file may be used under the terms
* of the GNU Lesser General Public License Version 3.0, as described below:
*
* This file is free software: you may copy, redistribute and/or modify
* it under the terms of the GNU Lesser General Public License as published by the
* Free Software Foundation, either version 3.0 of the License, or (at your
* option) any later version.
*
* This file is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
* Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program. If not, see http://www.gnu.org/licenses/.
*******************************************************************************/

#ifdef _WIN32

#include "smpt_serial_port_windows.h"
#include "smpt_messages.h"

static bool set_timeouts(HANDLE serial_port_handle, bool debug_error);
static void set_dcb(DCB *dcb, uint32_t baud_rate);
static bool set_comm_state(HANDLE *serial_port_handle, uint32_t baud_rate, bool debug_error);
static bool set_comm_mask(HANDLE *serial_port_handle, bool debug_error);

void set_dcb(DCB *dcb, uint32_t baud_rate)
{
    ZeroMemory(dcb, sizeof(DCB));
    dcb->DCBlength          = sizeof(DCB);
    dcb->fBinary            = TRUE;                 /* introduced 11.11.2015 */
    dcb->fInX               = FALSE;
    dcb->fOutX              = FALSE;
    dcb->fAbortOnError      = FALSE;
    dcb->fNull              = FALSE;
    dcb->fDtrControl        = DTR_CONTROL_DISABLE; /* introduced 11.11.2015 */
    dcb->BaudRate           = baud_rate;
    dcb->ByteSize           = 8;
    dcb->Parity             = NOPARITY;
    dcb->StopBits           = TWOSTOPBITS;
    dcb->fRtsControl        = RTS_CONTROL_HANDSHAKE;
    dcb->fOutxCtsFlow       = TRUE;
}

bool set_comm_state(HANDLE *serial_port_handle, uint32_t baud_rate, bool debug_error)
{
    bool result = true;
    DCB dcb = {0};

    if (GetCommState(*serial_port_handle, &dcb))
    {
        set_dcb(&dcb, baud_rate);
        if (!SetCommState(*serial_port_handle, &dcb))
        {
            if(debug_error)
            {
                smpt_error("set_comm_state(): Could not SetCommState.");
            }
            result = false;
        }
    }
    else
    {
        if(debug_error)
        {
            smpt_error("set_comm_state(): Could not GetCommState.");
        }
        result = false;
    }

    return result;
}

bool set_comm_mask(HANDLE *serial_port_handle, bool debug_error)
{
    bool result = true;
    DWORD dwtEvtMask = {0};
    dwtEvtMask |= EV_RXCHAR;
    if (!SetCommMask(*serial_port_handle, dwtEvtMask))
    {
        if(debug_error)
        {
            smpt_error("smpt_open_serial_port_win(): Could not SetCommMask");
        }
        result = false;
    }

    return result;
}

bool smpt_open_serial_port_win(HANDLE *serial_port_handle, const char port_name[],
                               uint32_t baud_rate, bool debug_error)
{
    bool result = true;    
    *serial_port_handle = CreateFileA(port_name, GENERIC_READ | GENERIC_WRITE,
                                      0, NULL, OPEN_EXISTING, 0, NULL);
    if (*serial_port_handle == INVALID_HANDLE_VALUE)
    {
        if (debug_error)
        {
            smpt_error("smpt_open_serial_port_win(): Invalid handle value. (%s)", port_name);
            smpt_error("GetLastError: %d", GetLastError());
        }
        result = false;
    }
    else
    {

        if (set_comm_state(serial_port_handle, baud_rate, debug_error))
        {
            result = set_timeouts(*serial_port_handle, debug_error);
            result &= set_comm_mask(serial_port_handle, debug_error);
        }
        else
        {
            if(debug_error)
            {
                smpt_error("smpt_open_serial_port_win(): Could not set_comm_state.");
            }
            result = false;
        }
    }

    return result;
}

bool set_timeouts(HANDLE serial_port_handle, bool debug_error)
{
    bool result = true;
    COMMTIMEOUTS com_time_out = {0};

    com_time_out.ReadIntervalTimeout = MAXDWORD;
    com_time_out.ReadTotalTimeoutMultiplier = 0;
    com_time_out.ReadTotalTimeoutConstant = 0;

    com_time_out.WriteTotalTimeoutMultiplier = 1;
    com_time_out.WriteTotalTimeoutConstant = 20;

    if (!SetCommTimeouts(serial_port_handle, &com_time_out))
    {
        if (debug_error)
        {
            smpt_error("set_timeouts(): Could not SetCommTimeouts.");
        }
        result = false;
    }

    return result;
}

bool smpt_write_to_serial_port_win(HANDLE serial_port_handle, const uint8_t output_data[],
                                   uint32_t output_data_length)
{
    bool result = true;
    uint32_t bytes_written = 0;

    if (!WriteFile(serial_port_handle,
                   output_data,
                   output_data_length,
                   (PDWORD)(&bytes_written),
                   NULL))
    {
        smpt_error("smpt_write_to_serial_port_win(): Could not WriteFile. (%d)", GetLastError());
        result = false;
    }

    if (bytes_written != output_data_length)
    {
        smpt_error("smpt_write_to_serial_port_win(): Could not write all bytes "
                   "(written: %d, length: %d, error: %d)",
                    bytes_written, output_data_length, GetLastError());
        result = false;
    }


    return result;
}

bool smpt_read_from_serial_port_win(uint32_t *bytes_read, HANDLE serial_port_handle,
                                    uint8_t input_data[], uint32_t input_data_length)
{
    bool result = true;
    if (!ReadFile(serial_port_handle,       /* handle of file to read */
                  input_data,               /* handle of file to read */
                  input_data_length,        /* number of bytes to read */
                  (PDWORD)(bytes_read),     /* pointer to number of bytes read */
                  NULL))                    /* pointer to structure for data */
    {
        smpt_error("smpt_read_from_serial_port_win(): Could not ReadFile.");
        return false;
    }

    return result;
}

bool smpt_close_serial_port_win(HANDLE serial_port_handle)
{
    bool result = true;
    if (!CloseHandle(serial_port_handle))
    {
        smpt_error("smpt_close_serial_port_win(): Could not CloseHandle.");
        result = false;
    }
    return result;
}
#else
typedef int dummy___;
#endif /* _WIN32 */

