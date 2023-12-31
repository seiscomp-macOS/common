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


#define SEISCOMP_COMPONENT L2Picker

#include <seiscomp/logging/log.h>
#include <seiscomp/processing/operator/ncomps.h>
#include <seiscomp/processing/operator/l2norm.h>
#include <seiscomp/math/filter.h>

#include "S_l2.h"


using namespace std;

namespace Seiscomp {

namespace Processing {

REGISTER_SECONDARYPICKPROCESSOR(SL2Picker, "S-L2");


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
SL2Picker::SL2Picker() : SAICPicker("L2-AIC", Horizontal) {}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
SL2Picker::~SL2Picker() { }
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool SL2Picker::setup(const Settings &settings) {
	if ( !SecondaryPicker::setup(settings) ) return false;

	// Check all three components for valid gains and orientations
	for ( int i = 1; i < 3; ++i ) {
		if ( _streamConfig[i].code().empty() ) {
			SEISCOMP_ERROR("[S-L2] component[%d] code is empty", i);
			setStatus(Error, i);
			return false;
		}

		if ( _streamConfig[i].gain == 0.0 ) {
			SEISCOMP_ERROR("[S-L2] component[%d] gain is missing", i);
			setStatus(MissingGain, i);
			return false;
		}
	}

	try { setNoiseStart(settings.getDouble("spicker.L2.noiseBegin")); }
	catch ( ... ) {}

	try { setSignalStart(settings.getDouble("spicker.L2.signalBegin")); }
	catch ( ... ) {}

	try { setSignalEnd(settings.getDouble("spicker.L2.signalEnd")); }
	catch ( ... ) {}

	AICConfig cfg = aicConfig();

	try { cfg.threshold = settings.getDouble("spicker.L2.threshold"); }
	catch ( ... ) {}

	try { cfg.minSNR = settings.getDouble("spicker.L2.minSNR"); }
	catch ( ... ) {}

	try { cfg.margin = settings.getDouble("spicker.L2.marginAIC"); }
	catch ( ... ) {}

	try { cfg.timeCorr = settings.getDouble("spicker.L2.timeCorr"); }
	catch ( ... ) {}

	try { cfg.filter = settings.getString("spicker.L2.filter"); }
	catch ( ... ) { cfg.filter = "BW(4,0.3,1.0)"; }

	try { cfg.detecFilter = settings.getString("spicker.L2.detecFilter"); }
	catch ( ... ) { cfg.detecFilter = "STALTA(1,10)"; }

	return setAicConfig(cfg);
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WaveformOperator* SL2Picker::createFilterOperator(Filter* compFilter)
{
	// Create a waveform operator that combines the two horizontal channels and
	// computes the l2norm of each 2 component sample
	typedef Operator::StreamConfigWrapper<double,2,Operator::L2Norm> OpWrapper;
	typedef Operator::FilterWrapper<double,2,OpWrapper> FilterL2Norm;
	typedef NCompsOperator<double,2,FilterL2Norm> L2Norm;
	return new L2Norm(
		FilterL2Norm(compFilter,
			OpWrapper(_streamConfig+1, Operator::L2Norm<double,2>())
		)
	);
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}
}
