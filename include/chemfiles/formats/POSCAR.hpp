// Chemfiles, a modern library for chemistry file reading and writing
// Copyright (C) Guillaume Fraux and contributors -- BSD license

#ifndef CHEMFILES_FORMAT_POSCAR_HPP
#define CHEMFILES_FORMAT_POSCAR_HPP

#include <cstdint>
#include <string>
#include <map>
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

private:
    /// Read the top-line comment into a property
    void read_comment(Frame& frame);
    /// Read the unit cell and associated scaling factor
    void read_unit_cell(Frame& frame);
    /// Read the optional chemical symbols and required type counts
    void read_species(Frame& frame);
    /// Read optional selective dynamics declaration
    void read_selective_dynamics(Frame& frame);
    /// Read binary cartesian/direct declaration
    void read_cartesian_direct(Frame& frame);
    /// Read all atomic properties
    void read_atoms(Frame& frame);

    /// Write the comment line
    void write_comment(const Frame& frame);
    /// Write the unit cell and scale factor
    void write_unit_cell(const Frame& frame);
    /// Write the optional chemical species and their counts
    void write_species(const Frame& frame);
    /// Write the optional selective dynamics declaration
    void write_selective_dynamics(const Frame& frame);
    /// Write the cartesian/direct declaration
    void write_cartesian_direct(const Frame& frame);
    /// Write the atomic data 
    void write_atoms(const Frame& frame);

    // =============== Data used for reading files
    /// Flag indicating selective dynamics
    bool has_selective_dynamics_ = false;
    /// Mapping of chemical symbols to their counts
    std::map<std::string, int> species_;
    /// Coordinate system types
    enum coordinate_t {
        CARTESIAN,
        DIRECT
    } coordinate_system_;
};

template<> FormatInfo format_information<POSCARFormat>();

} // namespace chemfiles

#endif
