/*
 * Copyright (C) 2017 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "gapii/cc/spy.h"
#include "gapii/cc/call_observer.h"

#include "core/cc/coder/gles.h"

// This file contains a number of GLES method 'overrides' to optionally lie to the
// application about the driver not supporting precompiled shaders or programs.

namespace {

const char* kOESGetProgramBinary  = "OES_get_program_binary";
const char* kReplacementExtension = "__GAPID_PCS_DISABLED__"; // Must be the same length as kOESGetProgramBinary

}  // anonymous namespace

using core::coder::atom::Observations;
using namespace gapii::GLenum;

namespace gapii {

void Spy::glProgramBinary(CallObserver* observer, uint32_t program, uint32_t binary_format, void* binary,
                          int32_t binary_size) {
    if (mDisablePrecompiledShaders) {
        // GL_INVALID_ENUM is generated if binaryformat is not a supported format returned in
        // GL_SHADER_BINARY_FORMATS.
        setFakeGlError(GL_INVALID_ENUM);

        core::coder::gles::GlProgramBinary coder(
                observer->getScratch()->vector<core::Encodable*>(kMaxExtras),
                program,
                binary_format,
                core::coder::gles::Void__CP(reinterpret_cast<uintptr_t>(binary), 0),
                binary_size);
        observer->read(binary, binary_size);
        observer->observeReads();
        observer->observeWrites();
        coder.mextras.append(observer->getExtras());
        mEncoder->Variant(&coder);
    } else {
        GlesSpy::glProgramBinary(observer, program, binary_format, binary, binary_size);
    }
}

void Spy::glProgramBinaryOES(CallObserver* observer, uint32_t program, uint32_t binary_format, void* binary,
                             int32_t binary_size) {
    if (mDisablePrecompiledShaders) {
        // GL_INVALID_ENUM is generated if binaryformat is not a supported format returned in
        // GL_SHADER_BINARY_FORMATS.
        setFakeGlError(GL_INVALID_ENUM);

        core::coder::gles::GlProgramBinaryOES coder(
                observer->getScratch()->vector<core::Encodable*>(kMaxExtras),
                program,
                binary_format,
                core::coder::gles::Void__CP(reinterpret_cast<uintptr_t>(binary), 0),
                binary_size);
        observer->read(binary, binary_size);
        observer->observeReads();
        observer->observeWrites();
        coder.mextras.append(observer->getExtras());
        mEncoder->Variant(&coder);
    } else {
        GlesSpy::glProgramBinaryOES(observer, program, binary_format, binary, binary_size);
    }
}

void Spy::glShaderBinary(CallObserver* observer, int32_t count, uint32_t* shaders, uint32_t binary_format, void* binary,
                         int32_t binary_size) {
    if (mDisablePrecompiledShaders) {
        // GL_INVALID_ENUM is generated if binaryFormat is not a value recognized by the implementation.
        setFakeGlError(GL_INVALID_ENUM);

        observer->read(slice(shaders, (uint64_t)((GLsizei)(0)), (uint64_t)(count)));
        observer->read(slice(binary, (uint64_t)((GLsizei)(0)), (uint64_t)(binary_size)));
        observer->observeReads();
        observer->observeWrites();
        core::coder::gles::GlShaderBinary coder(
                observer->getScratch()->vector<core::Encodable*>(kMaxExtras),
                count,
                core::coder::gles::ShaderId__CP(reinterpret_cast<uintptr_t>(shaders), 0),
                binary_format,
                core::coder::gles::Void__CP(reinterpret_cast<uintptr_t>(binary), 0),
                binary_size);
        coder.mextras.append(observer->getExtras());
        mEncoder->Variant(&coder);
    } else {
        GlesSpy::glShaderBinary(observer, count, shaders, binary_format, binary, binary_size);
    }
}

void Spy::glGetInteger64v(CallObserver* observer, uint32_t param, int64_t* values) {
    if (mDisablePrecompiledShaders &&
        (param == GL_NUM_SHADER_BINARY_FORMATS || param == GL_NUM_PROGRAM_BINARY_FORMATS)) {
        values[0] = 0;

        observer->observeReads();
        observer->write(slice(values, 0, 1));
        observer->observeWrites();
        core::coder::gles::GlGetInteger64v coder(
                observer->getScratch()->vector<core::Encodable*>(kMaxExtras),
                param,
                core::coder::gles::GLint64__P(reinterpret_cast<uintptr_t>(values), 0));
        coder.mextras.append(observer->getExtras());
        mEncoder->Variant(&coder);
    } else {
        GlesSpy::glGetInteger64v(observer, param, values);
    }
}

void Spy::glGetIntegerv(CallObserver* observer, uint32_t param, int32_t* values) {
    if (mDisablePrecompiledShaders &&
        (param == GL_NUM_SHADER_BINARY_FORMATS || param == GL_NUM_PROGRAM_BINARY_FORMATS)) {
        values[0] = 0;

        observer->observeReads();
        observer->write(slice(values, 0, 1));
        observer->observeWrites();
        core::coder::gles::GlGetIntegerv coder(
                observer->getScratch()->vector<core::Encodable*>(kMaxExtras),
                param,
                core::coder::gles::GLint__P(reinterpret_cast<uintptr_t>(values), 0));
        coder.mextras.append(observer->getExtras());
        mEncoder->Variant(&coder);
    } else {
        GlesSpy::glGetIntegerv(observer, param, values);
    }
}

GLubyte* Spy::glGetString(CallObserver* observer, uint32_t name) {
    if (mDisablePrecompiledShaders && name == GL_EXTENSIONS) {
        std::string list = reinterpret_cast<const char*>(GlesSpy::mImports.glGetString(name));
        size_t start = list.find(kOESGetProgramBinary);
        if (start != std::string::npos) {
            static std::string copy = list;
            copy.replace(start, strlen(kOESGetProgramBinary), kReplacementExtension);
            // TODO: write atom.
            return reinterpret_cast<GLubyte*>(const_cast<char*>(copy.c_str()));
        }
    }
    return GlesSpy::glGetString(observer, name);
}

GLubyte* Spy::glGetStringi(CallObserver* observer, uint32_t name, GLuint index) {
    if (mDisablePrecompiledShaders && (name == GL_EXTENSIONS)) {
        const char* extension = reinterpret_cast<const char*>(GlesSpy::mImports.glGetStringi(name, index));
        if (strcmp(extension, kOESGetProgramBinary) == 0) {
            // TODO: write atom.
            return reinterpret_cast<GLubyte*>(const_cast<char*>(kReplacementExtension));
        }
    }
    return GlesSpy::glGetStringi(observer, name, index);
}


} // namespace gapii
