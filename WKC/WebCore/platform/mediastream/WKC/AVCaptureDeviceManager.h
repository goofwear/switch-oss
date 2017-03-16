/*
 * Copyright (C) 2013 Apple Inc. All rights reserved.
 * Copyright (C) 2015 ACCESS Co.,Ltd. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AVCaptureDeviceManager_h
#define AVCaptureDeviceManager_h

#if ENABLE(MEDIA_STREAM)

#include "MediaStreamTrackSourcesRequestClient.h"
#include "RealtimeMediaSource.h"
#include <wtf/RetainPtr.h>
#include <wtf/text/WTFString.h>

namespace WebCore {

class CaptureDevice;

class AVCaptureDeviceManager {
public:
    static AVCaptureDeviceManager& singleton();
    static bool isAvailable();

    Vector<RefPtr<TrackSourceInfo>> getSourcesInfo(const String&);
    bool verifyConstraintsForMediaType(RealtimeMediaSource::Type, MediaConstraints*, String&);
    RefPtr<RealtimeMediaSource> bestSourceForTypeAndConstraints(RealtimeMediaSource::Type, PassRefPtr<MediaConstraints>);

    enum ValidConstraints { Width = 0, Height, FrameRate, FacingMode, Gain };
    static const Vector<AtomicString>& validConstraintNames();
    static const Vector<AtomicString>& validFacingModes();

    static bool isValidConstraint(RealtimeMediaSource::Type, const String&);

    void registerForDeviceNotifications();
    void deviceConnected();

protected:
    AVCaptureDeviceManager();
    ~AVCaptureDeviceManager();

    CaptureDevice* bestDeviceForFacingMode(RealtimeMediaSourceStates::VideoFacingMode);
};

} // namespace WebCore

#endif // ENABLE(MEDIA_STREAM)

#endif // AVCaptureDeviceManager_h
