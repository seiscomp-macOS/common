/***************************************************************************
 * Copyright (C) gempa GmbH                                                *
 * All rights reserved.                                                    *
 * Contact: gempa GmbH (seiscomp-dev@gempa.de)                             *
 *                                                                         *
 * GNU Affero General Public License Usage                                 *
 * This file may be used under the terms of the GNU Affero                 *
 * Public License version 3.0 as published by the Free Software Foundation *
 * and appearing in the file LICENSE included in the packaging of this     *
 * file. Please review the following information to ensure the GNU Affero  *
 * Public License version 3.0 requirements will be met:                    *
 * https://www.gnu.org/licenses/agpl-3.0.html.                             *
 *                                                                         *
 * Other Usage                                                             *
 * Alternatively, this file may be used in accordance with the terms and   *
 * conditions contained in a signed written agreement between you and      *
 * gempa GmbH.                                                             *
 ***************************************************************************/

#include <seiscomp/seismology/regions/polygon.h>

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include <boost/regex.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#define SEISCOMP_COMPONENT PolyRegion
#include <seiscomp/logging/log.h>
#include <seiscomp/core/system.h>

namespace fs = boost::filesystem;

namespace Seiscomp {
namespace Geo {



PolyRegions::PolyRegions(const std::string &location) {
	read(location);
}


PolyRegions::~PolyRegions() {
	for (size_t i = 0; i < _regions.size(); i++ ) {
		delete _regions[i];
	}
}


size_t PolyRegions::read(const std::string& location) {
	fs::path directory;
	try {
		directory = SC_FS_PATH(location);
	}
	catch ( ... ) {
		SEISCOMP_ERROR("Invalid path '%s'", location.c_str());
		return 0;
	}

	if ( !fs::exists(directory) )
		return regionCount();

	fs::directory_iterator end_itr;
	std::vector<std::string> files;

	try {
		for ( fs::directory_iterator itr(directory); itr != end_itr; ++itr ) {

			if ( fs::is_directory(*itr) )
				continue;

			if ( boost::regex_match(SC_FS_IT_LEAF(itr), boost::regex(".*\\.(?:fep)")) )
				files.push_back(SC_FS_IT_STR(itr));
		}
	}
	catch ( const std::exception &ex ) {
		SEISCOMP_ERROR("Reading regions: %s", ex.what());
		return regionCount();
	}

	std::sort(files.begin(), files.end());

	for ( size_t i = 0; i < files.size(); ++i ) {
		if ( !readFepBoundaries(files[i]) )
			SEISCOMP_ERROR("Error reading file: %s", files[i].c_str());
	}

	info();

	// store directory path the data was read from
	_dataDir = directory.string();

	return regionCount();
}


bool PolyRegions::readFepBoundaries(const std::string& filename) {
	SEISCOMP_DEBUG("reading boundary polygons from file: %s", filename.c_str());

	std::ifstream infile(filename.c_str());

	if ( infile.bad() )
		return false;

	boost::regex vertexLine("^\\s*([-+]?[0-9]*\\.?[0-9]+)\\s+([-+]?[0-9]*\\.?[0-9]+)(?:\\s+([^\\d\\s].*)$|\\s*$)");
	boost::regex LLine("^\\s*L\\s+(.*)$");
	boost::smatch what;

	std::string line;
	bool newPolygon = true;
	GeoFeature *pr = nullptr;
	OPT(GeoCoordinate) last;

	while ( std::getline(infile, line) ) {
		if ( newPolygon ){
			pr = new GeoFeature();
			newPolygon = false;
		}

		if ( boost::regex_match(line, what, vertexLine) ) {
			if ( last ) pr->addVertex(*last);
			last = GeoCoordinate(atof(what.str(2).c_str()), atof(what.str(1).c_str())).normalize();
		}
		else if ( boost::regex_match(line, what, LLine) ) {
			if ( last && pr->vertices().size() > 0 ) {
				if ( *last != pr->vertices().back() )
					pr->addVertex(*last);
			}

			if ( pr->vertices().size() < 3 )
				delete pr;
			else {
				pr->setName(what.str(1));
				pr->setClosedPolygon(true);
				pr->updateBoundingBox();
				addRegion(pr);

				if ( pr->area() < 0 )
					pr->invertOrder();
			}

			last = Core::None;
			newPolygon = true;
		}
	}

	return true;
}


void PolyRegions::addRegion(GeoFeature *r) {
	_regions.push_back(r);
}


size_t PolyRegions::regionCount() const {
	return _regions.size();
}


GeoFeature *PolyRegions::region(int i) const{
	return _regions.size() > 0 ? _regions[i] : nullptr;
}


void PolyRegions::print(){
	for ( size_t i = 0; i < _regions.size(); i++ ){
		std::cerr << region(i)->name() << std::endl;
		std::cerr <<  region(i)->vertices().size() << std::endl;
	}
}


void PolyRegions::info(){
	SEISCOMP_DEBUG("Number of PolyRegions loaded: %lu", (unsigned long)_regions.size());

	int sum = 0;
	for ( size_t i = 0; i < _regions.size(); i++ )
		sum += region(i)->vertices().size();

	SEISCOMP_DEBUG("Total number of vertices read in: %d", sum);
}


GeoFeature *PolyRegions::findRegion(double lat, double lon) const {
	auto gc = GeoCoordinate(lat, lon).normalize();
	for ( size_t i = 0; i < regionCount(); ++i ) {
		if ( region(i)->contains(gc) ) {
			return region(i);
		}
	}

	return nullptr;
}


std::string PolyRegions::findRegionName(double lat, double lon) const {
	GeoFeature *region = findRegion(lat, lon);
	if ( region ) {
		return region->name();
	}

	return "";
}


}
}
