// Chemfiles, a modern library for chemistry file reading and writing
// Copyright (C) Guillaume Fraux and contributors -- BSD license

#include "chemfiles/File.hpp"
#include "chemfiles/Format.hpp"
#include "chemfiles/Atom.hpp"
#include "chemfiles/Frame.hpp"
#include "chemfiles/Topology.hpp"
#include "chemfiles/Property.hpp"

#include "chemfiles/types.hpp"
#include "chemfiles/parse.hpp"
#include "chemfiles/error_fmt.hpp"
#include "chemfiles/external/optional.hpp"
#include "chemfiles/warnings.hpp"

#include "chemfiles/formats/POSCAR.hpp"

using namespace chemfiles;

template<> FormatInfo chemfiles::format_information<POSCARFormat>() {
    return FormatInfo("POSCAR").description(
        "VASP's POSCAR text format"
    );
}

void POSCARFormat::read_next(Frame& frame) {
    if (file_.tellpos() != 0) {
        throw format_error("POSCAR format only supports reading one frame");
    }
    // TODO
}

void POSCARFormat::write_next(const Frame& frame) {
    if (file_.tellpos() != 0) {
        throw format_error("POSCAR format only supports writing one frame");
    }
    // TODO
}

optional<uint64_t> POSCARFormat::forward() {
    // POSCAR only supports one step, so always act like there is only one
    auto position = file_.tellpos();
    if (position == 0) {
        // advance pointer for the next call
        file_.readline();
        return position;
    } else {
        return nullopt;
    }
}

void POSCARFormat::read_comment(Frame& frame) {}

void POSCARFormat::read_unit_cell(Frame& frame) {}

void POSCARFormat::read_species(Frame& frame) {}

void POSCARFormat::read_selective_dynamics(Frame& frame) {}

void POSCARFormat::read_cartesian_direct(Frame& frame) {}

void POSCARFormat::read_atoms(Frame& frame) {}

void POSCARFormat::write_comment(const Frame& frame) {}

void POSCARFormat::write_unit_cell(const Frame& frame) {}

void POSCARFormat::write_species(const Frame& frame) {}

void POSCARFormat::write_selective_dynamics(const Frame& frame) {}

void POSCARFormat::write_cartesian_direct(const Frame& frame) {}

void POSCARFormat::write_atoms(const Frame& frame) {}