/*!
LTC LT_PMBusSpeedTest

@verbatim

Check bus speed capability.

@endverbatim

REVISION HISTORY
$Revision: 3845 $
$Date: 2015-08-24 14:11:21 -0600 (Mon, 24 Aug 2015) $

Copyright (c) 2014, Linear Technology Corp.(LTC)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of Linear Technology Corp.

The Linear Technology Linduino is not affiliated with the official Arduino team.
However, the Linduino is only possible because of the Arduino team's commitment
to the open-source community.  Please, visit http://www.arduino.cc and
http://store.arduino.cc , and consider a purchase that will help fund their
ongoing work.
*/

/*! @file
    @ingroup LT_PMBusDevice
    Library Header File for LT_PMBusSpeedTest
*/

#include "LT_PMBusSpeedTest.h"

LT_PMBusSpeedTest::LT_PMBusSpeedTest(LT_PMBus *pmbus):pmbus_(pmbus)
{
}

uint32_t LT_PMBusSpeedTest::test(uint8_t address, uint8_t tries)
{
  bool nok;

  LT_I2CBus *i2cbus = pmbus_->smbus()->i2cbus();
  i2cbus->changeSpeed(400000);
  nok = false;
  for (int i = 0; i < tries; i++)
  {
    pmbus_->setPage(address, 0x00);
    nok |= pmbus_->getPage(address) != 0x00;
    if (nok) break;
    pmbus_->setPage(address, 0xFF);
    nok |= pmbus_->getPage(address) != 0xFF;
    if (nok) break;
  }
  if (!nok) return 400000;

  i2cbus->changeSpeed(100000);
  nok = false;
  for (int i = 0; i < tries; i++)
  {
    pmbus_->setPage(address, 0x00);
    nok |= pmbus_->getPage(address) != 0x00;
    if (nok) break;
    pmbus_->setPage(address, 0xFF);
    nok |= pmbus_->getPage(address) != 0xFF;
    if (nok) break;
  }
  if (!nok) return 100000;

  // Arduino Mega did not run at 10kHz.
  i2cbus->changeSpeed(20000);
  nok = false;
  for (int i = 0; i < tries; i++)
  {
    pmbus_->setPage(address, 0x00);
    nok |= pmbus_->getPage(address) != 0x00;
    if (nok) break;
    pmbus_->setPage(address, 0xFF);
    nok |= pmbus_->getPage(address) != 0xFF;
    if (nok) break;
  }
  if (!nok) return 10000;
  return 0;
}