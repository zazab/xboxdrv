/* 
**  Xbox/Xbox360/Xbox One USB Gamepad Userspace Driver
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

#ifndef HEADER_XBOX_ONE_CONTROLLER_HPP
#define HEADER_XBOX_ONE_CONTROLLER_HPP

#include <libusb.h>

#include "controller/usb_controller.hpp"
#include "xbox360_default_names.hpp"

struct XPadDevice;

class XboxOneController : public USBController
{
private:
  int m_endpoint_in;
  int m_endpoint_out;

  Xbox360DefaultNames xbox;
  
public:
  XboxOneController(libusb_device* dev, bool try_detach);
  virtual ~XboxOneController();

  void set_rumble_real(uint8_t left, uint8_t right);
  void set_led_real(uint8_t status);
  bool parse(const uint8_t* data, int len, ControllerMessage* msg_out);

private:
  XboxOneController (const XboxController&);
  XboxOneController& operator= (const XboxController&);
};

#endif

/* EOF */
