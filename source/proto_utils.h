/*
Copyright (c) 2020, SEVANA OÜ
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the <organization>.
4. Neither the name of the <organization> nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __PROTO_UTILS_H
#define __PROTO_UTILS_H

#include <exception>
#include <functional>
#include <cstdint>
#include <string>

namespace proto {

class error: public std::exception
{
public:
    error(int code);
    ~error();

    int code() const;
protected:
    int mCode;
};

enum
{
    error_listener_failed  = 1000,
    error_base_failed      = 1001
};

typedef std::function<void(const void* buffer, size_t len)> msg_arrived;

class msgparser
{
public:
    static const uint32_t MAGIC = 0x372947;
    struct header
    {
        uint32_t mMagic = 0;
        uint32_t mLength = 0xFFFFFFFF;
    };

    msgparser();
    ~msgparser();

    void set_callback(msg_arrived cb);
    void parse(const void* buffer, size_t num);

protected:
    msg_arrived mCallback;
    header mHeader;
    std::string mBuffer;
};

}
#endif
