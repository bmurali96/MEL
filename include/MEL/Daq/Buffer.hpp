// MIT License
//
// MEL - Mechatronics Engine & Library
// Copyright (c) 2018 Mechatronics and Haptic Interfaces Lab - Rice University
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#ifndef MEL_BUFFER_HPP
#define MEL_BUFFER_HPP

#include <MEL/Config.hpp>
#include <MEL/Core/Types.hpp>
#include <vector>
#include <iostream>
#include <map>

namespace mel {

class ModuleBase;

//==============================================================================
// BASE DECLARATION
//==============================================================================

/// Base class for Buffer
class MEL_API BufferBase {
public:
    /// Constructor
    BufferBase(ModuleBase* module);

protected:

    friend class ModuleBase;

    /// Called by ModuleBase when channel numbers change
    virtual void change_channel_numbers(const std::map<uint32, std::size_t>& old_map,
                                        const std::map<uint32, std::size_t>& new_map) = 0;

    /// Returns vector index associated with channel number
    std::size_t index(uint32 channel_number) const;

protected:

    ModuleBase* module_; ///< pointer to parent module
};

//==============================================================================
// CLASS DECLARATION
//==============================================================================

/// A Buffer of generic values stored on a Module, indexible by channel number
template <typename T>
class Buffer : public BufferBase {
public:

    /// Constructor
    Buffer(ModuleBase* module, T default_value = T());

    /// Read access indexed by channel number
    const T& operator[](uint32 channel_number) const;

    /// Write access indexed by channel number
    T& operator[](uint32 channel_number);

    /// Gets size of buffer
    std::size_t size() const;

    /// Gets reference to raw values array
    std::vector<T>& get();

    /// Sets raw values array
    void set(const std::vector<T>& values);

    /// Sets the default value subsequent new values should be instantied with
    void set_default_value(T default_value);

    /// Overload stream operator
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Buffer<U>& container);

private:

    /// Called by ModuleBase when channel numbers change
    void change_channel_numbers(const std::map<uint32, std::size_t>& old_map,
                                const std::map<uint32, std::size_t>& new_map) override;

private:

    T default_value_;        ///< default value
    std::vector<T> values_;  ///< raw values
};

} // namespace mel

#include <MEL/Daq/Detail/Buffer.inl>

#endif // MEL_BUFFER_HPP
