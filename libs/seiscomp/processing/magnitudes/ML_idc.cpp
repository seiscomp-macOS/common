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


#define SEISCOMP_COMPONENT Magnitudes/ML_idc

#include "ML_idc_private.h"

#include <seiscomp/logging/log.h>
#include <seiscomp/system/environment.h>

#include <mutex>

#include <cmath> // log10
#include <vector> // log10
#include <cfloat>
#include <fstream>


#define AMP_TYPE "SBSNR"
#define MAG_TYPE "ML(IDC)"

#define MINIMUM_DISTANCE 0.0   // in degrees
#define MAXIMUM_DISTANCE 20.0  // in degrees

#define MAXIMUM_DEPTH    40.0  // in km


using namespace std;


namespace {


std::string ExpectedAmplitudeUnit = "nm";

Util::TabValues tableA;
bool validTableA = false;
bool readTableA = false;
mutex mutexTableA;
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Magnitude_ML_idc::Magnitude_ML_idc()
: MagnitudeProcessor(MAG_TYPE) {
	_amplitudeType = AMP_TYPE;
	Magnitude_ML_idc::setDefaults();
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void Magnitude_ML_idc::setDefaults() {
	_minimumDistanceDeg = MINIMUM_DISTANCE;
	_maximumDistanceDeg = MAXIMUM_DISTANCE;
	_maximumDepthKm = MAXIMUM_DEPTH;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool Magnitude_ML_idc::setup(const Settings &settings) {
	_attenuation.reset();

	if ( !MagnitudeProcessor::setup(settings) ) {
		return false;
	}

	try {
		string tablePath = Environment::Instance()->absolutePath(settings.getString("magnitudes.ML(IDC).A"));
		size_t p;

		p = tablePath.find("{net}");
		if ( p != string::npos ) {
			tablePath.replace(p, 5, settings.networkCode);
		}

		p = tablePath.find("{sta}");
		if ( p != string::npos ) {
			tablePath.replace(p, 5, settings.stationCode);
		}

		p = tablePath.find("{loc}");
		if ( p != string::npos ) {
			tablePath.replace(p, 5, settings.locationCode);
		}

		SEISCOMP_DEBUG("Read station specific ML(IDC) attentuation table at %s",
		               tablePath.c_str());

		_attenuation = new Util::TabValues;
		bool validLocalTable = _attenuation->read(tablePath);
		if ( !validLocalTable ) {
			_attenuation.reset();
			SEISCOMP_ERROR("Failed to read A values from: %s", tablePath.c_str());
			return false;
		}
	}
	catch ( ... ) {}

	if ( !_attenuation ) {
		mutexTableA.lock();

		if ( !readTableA ) {
			string tablePath = Environment::Instance()->absolutePath("@DATADIR@/magnitudes/IDC/global.ml");
			SEISCOMP_DEBUG("Read global ML(IDC) attentuation table at %s",
			               tablePath.c_str());

			validTableA = tableA.read(tablePath);
			if ( !validTableA ) {
				SEISCOMP_ERROR("Failed to read A values from: %s", tablePath.c_str());
			}

			readTableA = true;
		}
		else if ( !validTableA ) {
			SEISCOMP_ERROR("Invalid A value table");
		}

		mutexTableA.unlock();
		return validTableA;
	}

	return true;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
MagnitudeProcessor::Status
Magnitude_ML_idc::computeMagnitude(double amplitude,
                                   const std::string &unit,
                                   double period, double,
                                   double delta, double depth,
                                   const DataModel::Origin *,
                                   const DataModel::SensorLocation *,
                                   const DataModel::Amplitude *,
                                   const Locale *,
                                   double &value) {
	if ( !_attenuation && !validTableA ) {
		return IncompleteConfiguration;
	}

	if ( !convertAmplitude(amplitude, unit, ExpectedAmplitudeUnit) ) {
		return InvalidAmplitudeUnit;
	}

	double x_1st_deriv, z_1st_deriv, x_2nd_deriv, z_2nd_deriv;
	double interpolated_value;
	int interp_err;
	bool ok;

	if ( _attenuation ) {
		ok = _attenuation->interpolate(interpolated_value, false, true,
		                               delta, 0, &x_1st_deriv, &z_1st_deriv,
		                               &x_2nd_deriv, &z_2nd_deriv, &interp_err);
	}
	else {
		mutexTableA.lock();
		ok = tableA.interpolate(interpolated_value, false, true,
		                        delta, 0, &x_1st_deriv, &z_1st_deriv,
		                        &x_2nd_deriv, &z_2nd_deriv, &interp_err);
		mutexTableA.unlock();
	}

	if ( !ok ) {
		SEISCOMP_ERROR("Failed to interpolate attentuation value");
		return Error;
	}

	if ( interp_err ) {
		SEISCOMP_ERROR("Error on attentuation interpolation: %d", interp_err);
		return Error;
	}

	value = log10(amplitude) + double(interpolated_value);

	return OK;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
REGISTER_MAGNITUDEPROCESSOR(Magnitude_ML_idc, MAG_TYPE);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>}
}
