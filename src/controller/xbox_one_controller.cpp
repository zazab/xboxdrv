/* 
**  Xbox/Xbox360 USB Gamepad Userspace Driver
**  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "controller/xbox_one_controller.hpp"

#include <sstream>
#include <stdexcept>
#include <string.h>

#include "controller_message.hpp"
#include "raise_exception.hpp"
#include "unpack.hpp"
#include "usb_helper.hpp"
#include "xboxmsg.hpp"

XboxOneController::XboxOneController(libusb_device* dev, bool try_detach) :
  USBController(dev),
  m_endpoint_in(1),
  m_endpoint_out(2),
  xbox(m_message_descriptor)
{
  // find endpoints
  m_endpoint_in  = usb_find_ep(LIBUSB_ENDPOINT_IN,  LIBUSB_CLASS_VENDOR_SPEC, 71, 208);
  m_endpoint_out = usb_find_ep(LIBUSB_ENDPOINT_OUT, LIBUSB_CLASS_VENDOR_SPEC, 71, 208);
  
  usb_claim_interface(0, try_detach);
  usb_submit_read(m_endpoint_in, 32);
}

XboxOneController::~XboxOneController()
{
}

void
XboxOneController::set_rumble_real(uint8_t left, uint8_t right)
{
  uint8_t rumblecmd[] = { 0x00, 0x06, 0x00, left, 0x00, right };
  usb_write(m_endpoint_out, rumblecmd, sizeof(rumblecmd));
}

void
XboxOneController::set_led_real(uint8_t status)
{
  // Controller doesn't have a LED
}

bool
XboxOneController::parse(const uint8_t* data, int len, ControllerMessage* msg_out)
{
  printf("len: %d\n", len);
  for (int i = 0; i < len; i++) {
      printf("data[%d]: %d\t", i, data[i]);
  }
  if (len == 20 && data[0] == 0x00 && data[1] == 0x14)
  {
    // memcpy(&msg_out->xbox, data, sizeof(XboxMsg));

    //unsigned int type       :8;
    //unsigned int length     :8;

    msg_out->set_key(xbox.dpad_up,    unpack::bit(data+2, 0));
    msg_out->set_key(xbox.dpad_down,  unpack::bit(data+2, 1));
    msg_out->set_key(xbox.dpad_left,  unpack::bit(data+2, 2));
    msg_out->set_key(xbox.dpad_right, unpack::bit(data+2, 3));

    msg_out->set_key(xbox.btn_start,   unpack::bit(data+2, 4));
    msg_out->set_key(xbox.btn_back,    unpack::bit(data+2, 5));
    msg_out->set_key(xbox.btn_thumb_l, unpack::bit(data+2, 6));
    msg_out->set_key(xbox.btn_thumb_r, unpack::bit(data+2, 7));

    //unsigned int dummy       :8;

    msg_out->set_abs(xbox.abs_a, data[4], 0, 255);
    msg_out->set_abs(xbox.abs_b, data[5], 0, 255);
    msg_out->set_abs(xbox.abs_x, data[6], 0, 255);
    msg_out->set_abs(xbox.abs_y, data[7], 0, 255);

    msg_out->set_abs(xbox.abs_black, data[8], 0, 255);
    msg_out->set_abs(xbox.abs_white, data[9], 0, 255);

    msg_out->set_abs(xbox.abs_lt, data[10], 0, 255);
    msg_out->set_abs(xbox.abs_rt, data[11], 0, 255);


    msg_out->set_abs(xbox.abs_x1, unpack::int16le(data+12), -32768, 32767);
    msg_out->set_abs(xbox.abs_y1, unpack::int16le(data+13), -32768, 32767);

    msg_out->set_abs(xbox.abs_x2, unpack::int16le(data+14), -32768, 32767);
    msg_out->set_abs(xbox.abs_y2, unpack::int16le(data+15), -32768, 32767);

    return true;
  }
  else
  {
    return false;
  }
}

/* EOF */
