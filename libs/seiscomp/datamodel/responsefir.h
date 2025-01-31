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


#ifndef SEISCOMP_DATAMODEL_RESPONSEFIR_H
#define SEISCOMP_DATAMODEL_RESPONSEFIR_H


#include <string>
#include <seiscomp/datamodel/realarray.h>
#include <seiscomp/datamodel/blob.h>
#include <seiscomp/datamodel/publicobject.h>
#include <seiscomp/core/exceptions.h>


namespace Seiscomp {
namespace DataModel {


DEFINE_SMARTPOINTER(ResponseFIR);

class Inventory;


class SC_SYSTEM_CORE_API ResponseFIRIndex {
	// ------------------------------------------------------------------
	//  Xstruction
	// ------------------------------------------------------------------
	public:
		//! Constructor
		ResponseFIRIndex();
		ResponseFIRIndex(const std::string& name);

		//! Copy constructor
		ResponseFIRIndex(const ResponseFIRIndex&);


	// ------------------------------------------------------------------
	//  Operators
	// ------------------------------------------------------------------
	public:
		bool operator==(const ResponseFIRIndex&) const;
		bool operator!=(const ResponseFIRIndex&) const;


	// ------------------------------------------------------------------
	//  Attributes
	// ------------------------------------------------------------------
	public:
		std::string name;
};


/**
 * \brief This type describes a finite impulse response filter
 */
class SC_SYSTEM_CORE_API ResponseFIR : public PublicObject {
	DECLARE_SC_CLASS(ResponseFIR)
	DECLARE_SERIALIZATION;
	DECLARE_METAOBJECT;

	// ------------------------------------------------------------------
	//  Xstruction
	// ------------------------------------------------------------------
	protected:
		//! Protected constructor
		ResponseFIR();

	public:
		//! Copy constructor
		ResponseFIR(const ResponseFIR &other);

		//! Constructor with publicID
		ResponseFIR(const std::string& publicID);

		//! Destructor
		~ResponseFIR() override;


	// ------------------------------------------------------------------
	//  Creators
	// ------------------------------------------------------------------
	public:
		static ResponseFIR *Create();
		static ResponseFIR *Create(const std::string& publicID);


	// ------------------------------------------------------------------
	//  Lookup
	// ------------------------------------------------------------------
	public:
		static ResponseFIR *Find(const std::string& publicID);


	// ------------------------------------------------------------------
	//  Operators
	// ------------------------------------------------------------------
	public:
		//! Copies the metadata of other to this
		//! No changes regarding child objects are made
		ResponseFIR &operator=(const ResponseFIR &other);
		//! Checks for equality of two objects. Child objects
		//! are not part of the check.
		bool operator==(const ResponseFIR &other) const;
		bool operator!=(const ResponseFIR &other) const;

		//! Wrapper that calls operator==
		bool equal(const ResponseFIR &other) const;


	// ------------------------------------------------------------------
	//  Setters/Getters
	// ------------------------------------------------------------------
	public:
		//! Unique response name
		void setName(const std::string& name);
		const std::string& name() const;

		//! Gain of response (48.05/58.04)
		void setGain(const OPT(double)& gain);
		double gain() const;

		//! Gain frequency (48.06/58.05)
		void setGainFrequency(const OPT(double)& gainFrequency);
		double gainFrequency() const;

		//! Decimation factor (47.06/57.05)
		void setDecimationFactor(const OPT(int)& decimationFactor);
		int decimationFactor() const;

		//! Estimated delay (47.08/57.07)
		void setDelay(const OPT(double)& delay);
		double delay() const;

		//! Applied correction (47.09/57.08)
		void setCorrection(const OPT(double)& correction);
		double correction() const;

		//! Number of coefficients (41.08/61.08)
		void setNumberOfCoefficients(const OPT(int)& numberOfCoefficients);
		int numberOfCoefficients() const;

		//! Symmetry code (41.05/61.05)
		void setSymmetry(const std::string& symmetry);
		const std::string& symmetry() const;

		//! Coefficients normalized to gain=1.0 (41.09/61.09)
		void setCoefficients(const OPT(RealArray)& coefficients);
		RealArray& coefficients();
		const RealArray& coefficients() const;

		void setRemark(const OPT(Blob)& remark);
		Blob& remark();
		const Blob& remark() const;


	// ------------------------------------------------------------------
	//  Index management
	// ------------------------------------------------------------------
	public:
		//! Returns the object's index
		const ResponseFIRIndex &index() const;

		//! Checks two objects for equality regarding their index
		bool equalIndex(const ResponseFIR *lhs) const;


	// ------------------------------------------------------------------
	//  Public interface
	// ------------------------------------------------------------------
	public:
		Inventory *inventory() const;

		//! Implement Object interface
		bool assign(Object *other) override;
		bool attachTo(PublicObject *parent) override;
		bool detachFrom(PublicObject *parent) override;
		bool detach() override;

		//! Creates a clone
		Object *clone() const override;

		//! Implement PublicObject interface
		bool updateChild(Object *child) override;

		void accept(Visitor *visitor) override;


	// ------------------------------------------------------------------
	//  Implementation
	// ------------------------------------------------------------------
	private:
		// Index
		ResponseFIRIndex _index;

		// Attributes
		OPT(double) _gain;
		OPT(double) _gainFrequency;
		OPT(int) _decimationFactor;
		OPT(double) _delay;
		OPT(double) _correction;
		OPT(int) _numberOfCoefficients;
		std::string _symmetry;
		OPT(RealArray) _coefficients;
		OPT(Blob) _remark;

	DECLARE_SC_CLASSFACTORY_FRIEND(ResponseFIR);
};


}
}


#endif
