/*
* Copyright (c) 2016 ACCESS CO., LTD. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "config.h"

#ifndef GamepadProviderWKC_h
#define GamepadProviderWKC_h

#if ENABLE(GAMEPAD)

#include "GamepadProvider.h"
#include <wtf/HashSet.h>

namespace WebCore {

class GamepadProviderClient;

class GamepadProviderWKC : public GamepadProvider {
    WTF_MAKE_FAST_ALLOCATED;

public:
    WEBCORE_EXPORT static GamepadProviderWKC& singleton();

    virtual void startMonitoringGamepads(GamepadProviderClient*) override;
    virtual void stopMonitoringGamepads(GamepadProviderClient*) override;
    virtual const Vector<PlatformGamepad*>& platformGamepads() override { return m_gamepads; }

    WEBCORE_EXPORT int addGamepad(const WTF::String& in_gamepadId, int in_naxes, int in_nbuttons);
    WEBCORE_EXPORT void removeGamepad(unsigned int in_index);

    WEBCORE_EXPORT void updateGamepadValue(unsigned int index, long long timestamp, int naxes, const double* axes, int nbuttons, const double* buttons);

private:
    GamepadProviderWKC();

    int getUnusedIndex();

    Vector<PlatformGamepad*> m_gamepads;
    HashSet<GamepadProviderClient*> m_clients;
};

}

#endif // ENABLE(GAMEPAD)
#endif // GamepadProviderWKC_h
