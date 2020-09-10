// Chemfiles, a modern library for chemistry file reading and writing
// Copyright (C) Guillaume Fraux and contributors -- BSD license

#ifndef CHEMFILES_FORMAT_POSCAR_HPP
#define CHEMFILES_FORMAT_POSCAR_HPP

#include <cstdint>
#include <string>
#include <memory>

#include "chemfiles/File.hpp"
#include "chemfiles/Format.hpp"
#include "chemfiles/external/optional.hpp"

namespace chemfiles {
class Frame;
class MemoryBuffer;

/// [POSCAR] file format reader and writer.
///
/// [POSCAR]: https://cms.mpi.univie.ac.at/vasp/vasp/POSCAR_file.html
class POSCARFormat final: public TextFormat {
public:
    POSCARFormat(std::string path, File::Mode mode, File::Compression compression):
        TextFormat(std::move(path), mode, compression){}
    
    POSCARFormat(std::shared_ptr<MemoryBuffer> memory, File::Mode mode, File::Compression compression):
        TextFormat(std::move(memory), mode, compression){}

    void read_next(Frame& frame) override;
    void write_next(const Frame& frame) override;
    optional<uint64_t> forward() override;
};

template<> FormatInfo format_information<POSCARFormat>();

} // namespace chemfiles

#endif
