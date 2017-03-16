/*
 * Copyright (C) 2011, 2014 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "TextRun.h"

namespace WebCore {

struct ExpectedTextRunSize {
#if PLATFORM(WKC)
    WTF_MAKE_FAST_ALLOCATED;
public:
#endif
    void* renderingContext;
    StringView text;
    unsigned integer1;
    unsigned integer2;
    float float1;
    float float2;
    float float3;
    unsigned bitfields : 9;
};

COMPILE_ASSERT(sizeof(TextRun) == sizeof(ExpectedTextRunSize), TextRun_is_not_of_expected_size);

#if !PLATFORM(WKC)
bool TextRun::s_allowsRoundingHacks = false;
#else
WKC_DEFINE_GLOBAL_CLASS_OBJ(bool, TextRun, s_allowsRoundingHacks, false);
#endif

void TextRun::setAllowsRoundingHacks(bool allowsRoundingHacks)
{
    s_allowsRoundingHacks = allowsRoundingHacks;
}

bool TextRun::allowsRoundingHacks()
{
    return s_allowsRoundingHacks;
}

}
