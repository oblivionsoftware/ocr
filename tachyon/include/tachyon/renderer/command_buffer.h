/*
 * Copyright 2016 Jeff Upton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cassert>
#include <vector>

#include "tachyon/core/common.h"
#include "tachyon/core/exception.h"
#include "tachyon/core/non_copyable.h"

namespace tachyon {

class CommandBuffer;

enum class CommandType {
    Clear
};

struct CommandHeader {
    size_t size;
    CommandType type;
};

struct ClearCommand {

    const static CommandType COMMAND_TYPE {CommandType::Clear};

    ClearCommand() = default;

    ClearCommand(r32 r, r32 g, r32 b, r32 a = 1.0f) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    r32 r {0.0f};
    r32 g {0.0f};
    r32 b {0.0f};
    r32 a {1.0f};
};

class CommandIterator {
public:

    explicit CommandIterator(CommandBuffer &buffer);

    bool next();

    template <typename T>
    T *command() const {
        return reinterpret_cast<T*>(_command);
    }

    CommandType type() const {
        return _type;
    }

private:

    CommandBuffer &_buffer;

    size_t _offset;

    CommandType _type;

    u8 *_command;

};


class CommandBuffer : private NonCopyable {
public:

    explicit CommandBuffer(size_t capacity);

    template <typename T, typename... Args>
    T *push(Args&&... args);

    void clear();

    template <typename T>
    T *data(size_t offset) {
        return reinterpret_cast<T*>(&_buffer[offset]);
    }

    size_t offset() const {
        return _offset;
    }

private:

    size_t _offset;

    std::vector<u8> _buffer;

};

template <typename T, typename... Args>
T *CommandBuffer::push(Args&&... args)
{
    size_t commandSize = sizeof(T);
    size_t pushSize = commandSize + sizeof(CommandHeader);

    assert(_offset + pushSize <= _buffer.size());

    CommandHeader *header = data<CommandHeader>(_offset);
    header->size = commandSize;
    header->type = T::COMMAND_TYPE;

    auto result = data<T>(_offset + sizeof(CommandHeader));
    new (result) T (std::forward<Args&&>(args)...);
    _offset += pushSize;

    return result;
}

inline CommandIterator::CommandIterator(CommandBuffer &buffer)
    : _buffer {buffer},
      _offset {0}
{
}

inline bool CommandIterator::next()
{
    if (_offset < _buffer.offset()) {
        auto header = _buffer.data<CommandHeader>(_offset);
        _type = header->type;

        _offset += sizeof(CommandHeader);
        _command = _buffer.data<u8>(_offset);

        _offset += header->size;

        return true; 
    }

    return false;
}

}
