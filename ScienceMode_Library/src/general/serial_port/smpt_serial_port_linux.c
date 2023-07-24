/***************************************************************************//**
* \file smpt_ll_serial_port_linux.c
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

#include "smpt_definitions.h"
#include "smpt_messages.h"

#ifdef __linux__

#ifdef ANDROID
#else

#include "smpt_serial_port_linux.h"
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

static int set_flowcontrol(int fd, int control);

bool smpt_open_serial_port_linux(int *fd, const char port_name[],
                                 unsigned int baud_rate, bool debug_error)
{
    bool result;

    *fd = open(port_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (*fd > 0)
    {
        struct termios tio = {0};

        tio.c_cc[VMIN]  = 0;
        tio.c_cc[VTIME] = 1;
        tio.c_cflag &= ~(PARENB | PARODD);
        tio.c_cflag = baud_rate |CS8 | CLOCAL | CREAD | CRTSCTS | CSTOPB;

        tio.c_iflag = IGNBRK | IGNPAR;


        tcsetattr(*fd, TCSANOW, &tio);
        if (tcsetattr(*fd, TCSANOW, &tio) != 0)
        {
            if (debug_error)
            {
                smpt_error("smpt_open_serial_port_linux(): Could not set serial attributes (err: %d)",
                           errno);
                result = false;
            }
        }

        if ((cfsetospeed(&tio, baud_rate) < 0) ||
            (cfsetispeed(&tio, baud_rate) < 0))
        {
            if (debug_error)
            {
                smpt_error("smpt_open_serial_port_linux(): Could not set baud rate");
            }
            result = false;
        }

        tcflush(*fd, TCIOFLUSH);
        result = true;
    }
    else
    {
        result = false;
    }
    return result;
}


bool smpt_write_to_serial_port_linux(int *const fd, const uint8_t output_data[],
                                     uint32_t output_data_length)
{
    bool result = true;
    uint32_t bytes_written = write(*fd, output_data, output_data_length);

    if (bytes_written != output_data_length)
    {
        smpt_error("smpt_write_to_serial_port_linux(): Could not write all bytes \
                   (written: %d, length: %d)",
                    bytes_written, output_data_length);
        result = false;
    }

    return result;
}


bool smpt_read_from_serial_port_linux(uint32_t *bytes_read, int fd,
                                      uint8_t input_data[], uint32_t input_data_length)
{
    bool result;

    if (fd > 0)
    {
        int result_read = read(fd, input_data, input_data_length);
        if (result_read > 0)
        {
            *bytes_read = result_read;
            result = true;
        }
        else
        {
            if (errno != EAGAIN) /* EAGAIN error is no error, because the device is O_NONBLOCK
                                    man read: EAGAIN - Non-blocking I/O has been selected using O_NONBLOCK
                                              and no data was immediately available for reading. */
            {
                smpt_error("smpt_read_from_serial_port_linux(): Read error (%s)", strerror(errno));
                result = false;
            }
            else
            {
                result = true;
            }

            *bytes_read = 0;
        }
    }
    else
    {
        result = false;
        *bytes_read = 0;
    }

    return result;
}


bool smpt_close_serial_port_linux(int fd)
{
    set_flowcontrol(fd, 0);
    tcflush(fd, TCIOFLUSH);
    return (close(fd) == 0);
}

int set_flowcontrol(int fd, int control)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0)
    {
        smpt_error("smpt_serial_port_linux.c: error from tggetattr");
        return -1;
    }

    if (control)
    {
        tty.c_cflag |= CRTSCTS;
    }
    else
    {
        tty.c_cflag &= ~CRTSCTS;
    }

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        smpt_error("smpt_serial_port_linux.c: error setting term attributes");
        return -1;
    }
    return 0;
}

#endif /* __ANDROID_API__ */
#endif /* __linux__ */
