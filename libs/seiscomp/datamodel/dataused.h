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


#ifndef SEISCOMP_DATAMODEL_DATAUSED_H
#define SEISCOMP_DATAMODEL_DATAUSED_H


#include <seiscomp/datamodel/types.h>
#include <seiscomp/datamodel/object.h>
#include <seiscomp/core/exceptions.h>


namespace Seiscomp {
namespace DataModel {


DEFINE_SMARTPOINTER(DataUsed);

class MomentTensor;


/**
 * \brief The DataUsed class describes the type of data that has been
 * \brief used for a
 * \brief moment-tensor inversion.
 */
class SC_SYSTEM_CORE_API DataUsed : public Object {
	DECLARE_SC_CLASS(DataUsed)
	DECLARE_SERIALIZATION;
	DECLARE_METAOBJECT;

	// ------------------------------------------------------------------
	//  Xstruction
	// ------------------------------------------------------------------
	public:
		//! Constructor
		DataUsed();

		//! Copy constructor
		DataUsed(const DataUsed &other);

		//! Destructor
		~DataUsed() override;


	// ------------------------------------------------------------------
	//  Operators
	// ------------------------------------------------------------------
	public:
		//! Copies the metadata of other to this
		DataUsed &operator=(const DataUsed &other);
		//! Checks for equality of two objects. Child objects
		//! are not part of the check.
		bool operator==(const DataUsed &other) const;
		bool operator!=(const DataUsed &other) const;

		//! Wrapper that calls operator==
		bool equal(const DataUsed &other) const;


	// ------------------------------------------------------------------
	//  Setters/Getters
	// ------------------------------------------------------------------
	public:
		//! Type of waveform data.
		void setWaveType(DataUsedWaveType waveType);
		DataUsedWaveType waveType() const;

		//! Number of stations that have contributed data of the type
		//! given in waveType.
		void setStationCount(int stationCount);
		int stationCount() const;

		//! Number of data components of the type given in waveType.
		void setComponentCount(int componentCount);
		int componentCount() const;

		//! Shortest period present in data in seconds.
		void setShortestPeriod(const OPT(double)& shortestPeriod);
		double shortestPeriod() const;


	// ------------------------------------------------------------------
	//  Public interface
	// ------------------------------------------------------------------
	public:
		MomentTensor *momentTensor() const;

		//! Implement Object interface
		bool assign(Object *other) override;
		bool attachTo(PublicObject *parent) override;
		bool detachFrom(PublicObject *parent) override;
		bool detach() override;

		//! Creates a clone
		Object *clone() const override;

		void accept(Visitor *visitor) override;


	// ------------------------------------------------------------------
	//  Implementation
	// ------------------------------------------------------------------
	private:
		// Attributes
		DataUsedWaveType _waveType;
		int _stationCount;
		int _componentCount;
		OPT(double) _shortestPeriod;
};


}
}


#endif
