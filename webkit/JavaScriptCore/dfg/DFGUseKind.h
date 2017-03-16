/*
 * Copyright (C) 2013-2015 Apple Inc. All rights reserved.
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

#ifndef DFGUseKind_h
#define DFGUseKind_h

#if ENABLE(DFG_JIT)

#include "DFGNodeFlags.h"
#include "SpeculatedType.h"
#include <wtf/PrintStream.h>

namespace JSC { namespace DFG {

enum UseKind {
    // The DFG has 3 representations of values used:

    // 1. The JSValue representation for a JSValue that must be stored in a GP
    //    register (or a GP register pair), and follows rules for boxing and unboxing
    //    that allow the JSValue to be stored as either fully boxed JSValues, or
    //    unboxed Int32, Booleans, Cells, etc. in 32-bit as appropriate.
    UntypedUse, // UntypedUse must come first (value 0).
    Int32Use,
    KnownInt32Use,
    MachineIntUse,
    NumberUse,
    RealNumberUse,
    BooleanUse,
    CellUse,
    KnownCellUse,
    ObjectUse,
    FunctionUse,
    FinalObjectUse,
    ObjectOrOtherUse,
    StringIdentUse,
    StringUse,
    KnownStringUse,
    StringObjectUse,
    StringOrStringObjectUse,
    NotStringVarUse,
    NotCellUse,
    OtherUse,
    MiscUse,

    // 2. The Double representation for an unboxed double value that must be stored
    //    in an FP register.
    DoubleRepUse,
    DoubleRepRealUse,
    DoubleRepMachineIntUse,

    // 3. The Int52 representation for an unboxed integer value that must be stored
    //    in a GP register.
    Int52RepUse,

    LastUseKind // Must always be the last entry in the enum, as it is used to denote the number of enum elements.
};

inline SpeculatedType typeFilterFor(UseKind useKind)
{
    switch (useKind) {
    case UntypedUse:
        return SpecFullTop;
    case Int32Use:
    case KnownInt32Use:
        return SpecInt32;
    case Int52RepUse:
        return SpecMachineInt;
    case MachineIntUse:
        return SpecInt32 | SpecInt52AsDouble;
    case NumberUse:
        return SpecBytecodeNumber;
    case RealNumberUse:
        return SpecBytecodeRealNumber;
    case DoubleRepUse:
        return SpecFullDouble;
    case DoubleRepRealUse:
        return SpecDoubleReal;
    case DoubleRepMachineIntUse:
        return SpecInt52AsDouble;
    case BooleanUse:
        return SpecBoolean;
    case CellUse:
    case KnownCellUse:
        return SpecCell;
    case ObjectUse:
        return SpecObject;
    case FunctionUse:
        return SpecFunction;
    case FinalObjectUse:
        return SpecFinalObject;
    case ObjectOrOtherUse:
        return SpecObject | SpecOther;
    case StringIdentUse:
        return SpecStringIdent;
    case StringUse:
    case KnownStringUse:
        return SpecString;
    case StringObjectUse:
        return SpecStringObject;
    case StringOrStringObjectUse:
        return SpecString | SpecStringObject;
    case NotStringVarUse:
        return ~SpecStringVar;
    case NotCellUse:
        return ~SpecCell;
    case OtherUse:
        return SpecOther;
    case MiscUse:
        return SpecMisc;
    default:
        RELEASE_ASSERT_NOT_REACHED();
        return SpecFullTop;
    }
}

inline bool shouldNotHaveTypeCheck(UseKind kind)
{
    switch (kind) {
    case UntypedUse:
    case KnownInt32Use:
    case KnownCellUse:
    case KnownStringUse:
    case Int52RepUse:
    case DoubleRepUse:
        return true;
    default:
        return false;
    }
}

inline bool mayHaveTypeCheck(UseKind kind)
{
    return !shouldNotHaveTypeCheck(kind);
}

inline bool isNumerical(UseKind kind)
{
    switch (kind) {
    case Int32Use:
    case KnownInt32Use:
    case NumberUse:
    case RealNumberUse:
    case Int52RepUse:
    case DoubleRepUse:
    case DoubleRepRealUse:
    case MachineIntUse:
    case DoubleRepMachineIntUse:
        return true;
    default:
        return false;
    }
}

inline bool isDouble(UseKind kind)
{
    switch (kind) {
    case DoubleRepUse:
    case DoubleRepRealUse:
    case DoubleRepMachineIntUse:
        return true;
    default:
        return false;
    }
}

inline bool isCell(UseKind kind)
{
    switch (kind) {
    case CellUse:
    case KnownCellUse:
    case ObjectUse:
    case FunctionUse:
    case FinalObjectUse:
    case StringIdentUse:
    case StringUse:
    case KnownStringUse:
    case StringObjectUse:
    case StringOrStringObjectUse:
        return true;
    default:
        return false;
    }
}

// Returns true if it uses structure in a way that could be clobbered by
// things that change the structure.
inline bool usesStructure(UseKind kind)
{
    switch (kind) {
    case StringObjectUse:
    case StringOrStringObjectUse:
        return true;
    default:
        return false;
    }
}

// Returns true if we've already guaranteed the type 
inline bool alreadyChecked(UseKind kind, SpeculatedType type)
{
    // If the check involves the structure then we need to know more than just the type to be sure
    // that the check is done.
    if (usesStructure(kind))
        return false;
    
    return !(type & ~typeFilterFor(kind));
}

inline UseKind useKindForResult(NodeFlags result)
{
    ASSERT(!(result & ~NodeResultMask));
    switch (result) {
    case NodeResultInt52:
        return Int52RepUse;
    case NodeResultDouble:
        return DoubleRepUse;
    default:
        return UntypedUse;
    }
}

} } // namespace JSC::DFG

namespace WTF {

void printInternal(PrintStream&, JSC::DFG::UseKind);

} // namespace WTF

#endif // ENABLE(DFG_JIT)

#endif // DFGUseKind_h

