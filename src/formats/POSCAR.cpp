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

void POSCARFormat::read_next(Frame& frame) {}

void POSCARFormat::write_next(const Frame& frame) {}

optional<uint64_t> POSCARFormat::forward() {}
