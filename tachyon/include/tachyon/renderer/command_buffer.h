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
#include "tachyon/core/math.h"
#include "tachyon/core/non_copyable.h"

namespace tachyon {

class CommandBuffer;

/**
 * Enumeration for each type of command in the buffer.
 */
enum class CommandType {
    Clear,
    DrawSprite
};

/**
 * Header stored in the command buffer that stores offset information.
 */
struct CommandHeader {
    size_t size;
    CommandType type;
};

/**
 * The command to clear the screen.
 */
struct ClearCommand {

    const static CommandType COMMAND_TYPE {CommandType::Clear};

    ClearCommand() = default;

    ClearCommand(vec4 color) {
        this->color = color;
    }

    vec4 color;
};

struct DrawSprite {

    const static CommandType COMMAND_TYPE {CommandType::DrawSprite};

    DrawSprite() = default;

    DrawSprite(u32 texture) {
        this->texture = texture;
    }

    u32 texture;

};

/**
 * An iterator over all commands in the buffer.
 */
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


/**
 * A buffer of render commands.
 */
class CommandBuffer : private NonCopyable {
public:

    explicit CommandBuffer(size_t capacity);

    /**
     * Adds a command to the buffer.
     *
     * @param args The constructor arguments to pass when constructing T.
     */
    template <typename T, typename... Args>
    T *push(Args&&... args);

    /**
     * Clears all commands in the buffer.
     */
    void clear();

    /**
     * Gets a pointer of the specified type at an offset within the command buffer.
     *
     * @param offset The offset into the buffer.
     * @return The data inside the buffer.
     */
    template <typename T>
    T *data(size_t offset) {
        return reinterpret_cast<T*>(&_buffer[offset]);
    }

    /**
     * Gets the current offset.
     *
     * @return The offset.
     */
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
