/*
 * Copyright (c) 2013-2014 ACCESS CO., LTD. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

#ifndef _WKC_HELPERS_WKC_BATTERYCONTROLLER_H_
#define _WKC_HELPERS_WKC_BATTERYCONTROLLER_H_

#include "WKCString.h"

namespace WKC {

class BatteryControllerPrivate;
class BatteryStatus;

class WKC_API BatteryController {
public:
    void updateBatteryStatus(const BatteryStatus&);
    void didChangeBatteryStatus(const String& eventType, const BatteryStatus&);

protected:
    // Applications must not create/destroy WKC helper instances by new/delete.
    // Or, it causes memory leaks or crashes.
    BatteryController(BatteryControllerPrivate&);
    ~BatteryController();

private:
    BatteryController(const BatteryController&);
    BatteryController& operator=(const BatteryController&);

    BatteryControllerPrivate& m_private;
};
} // namespace

#endif // _WKC_HELPERS_WKC_BATTERYCONTROLLER_H_
