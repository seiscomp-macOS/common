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


#ifndef SEISCOMP_DATAMODEL_INTEGERQUANTITY_H
#define SEISCOMP_DATAMODEL_INTEGERQUANTITY_H


#include <seiscomp/core/baseobject.h>
#include <seiscomp/core.h>
#include <seiscomp/core/exceptions.h>


namespace Seiscomp {
namespace DataModel {


DEFINE_SMARTPOINTER(IntegerQuantity);


/**
 * \brief Physical quantities expressed as integers are represented
 * \brief by their
 * \brief measured or computed values and optional values for
 * \brief symmetric or upper
 * \brief and lower uncertainties. The interpretation of these
 * \brief uncertainties is
 * \brief not defined in the standard. They can contain statistically
 * \brief well-defined
 * \brief error measures, but the mechanism can also be used to
 * \brief simply describe a
 * \brief possible value range. If the confidence level of the
 * \brief uncertainty is known,
 * \brief it can be listed in the optional attribute confidenceLevel.
 * \brief Note that uncertainty, upperUncertainty, and
 * \brief lowerUncertainty are given as absolute values of the
 * \brief deviation
 * \brief from the main value.
 */
class SC_SYSTEM_CORE_API IntegerQuantity : public Core::BaseObject {
	DECLARE_SC_CLASS(IntegerQuantity)
	DECLARE_SERIALIZATION;
	DECLARE_METAOBJECT;

	// ------------------------------------------------------------------
	//  Xstruction
	// ------------------------------------------------------------------
	public:
		//! Constructor
		IntegerQuantity();

		//! Copy constructor
		IntegerQuantity(const IntegerQuantity &other);

		//! Custom constructor
		IntegerQuantity(int value,
		                const OPT(int)& uncertainty = Seiscomp::Core::None,
		                const OPT(int)& lowerUncertainty = Seiscomp::Core::None,
		                const OPT(int)& upperUncertainty = Seiscomp::Core::None,
		                const OPT(double)& confidenceLevel = Seiscomp::Core::None);

		//! Destructor
		~IntegerQuantity() override;


	// ------------------------------------------------------------------
	//  Operators
	// ------------------------------------------------------------------
	public:
		operator int&();
		operator int() const;

		//! Copies the metadata of other to this
		IntegerQuantity &operator=(const IntegerQuantity &other);
		//! Checks for equality of two objects. Child objects
		//! are not part of the check.
		bool operator==(const IntegerQuantity &other) const;
		bool operator!=(const IntegerQuantity &other) const;

		//! Wrapper that calls operator==
		bool equal(const IntegerQuantity &other) const;


	// ------------------------------------------------------------------
	//  Setters/Getters
	// ------------------------------------------------------------------
	public:
		//! Value of the quantity. The unit is implicitly defined and
		//! depends on the context.
		void setValue(int value);
		int value() const;

		//! Uncertainty as the absolute value of symmetric deviation
		//! from the main value.
		void setUncertainty(const OPT(int)& uncertainty);
		int uncertainty() const;

		//! Uncertainty as the absolute value of deviation from the
		//! main value towards smaller values.
		void setLowerUncertainty(const OPT(int)& lowerUncertainty);
		int lowerUncertainty() const;

		//! Uncertainty as the absolute value of deviation from the
		//! main value towards larger values.
		void setUpperUncertainty(const OPT(int)& upperUncertainty);
		int upperUncertainty() const;

		//! Confidence level of the uncertainty, given in percent.
		void setConfidenceLevel(const OPT(double)& confidenceLevel);
		double confidenceLevel() const;


	// ------------------------------------------------------------------
	//  Implementation
	// ------------------------------------------------------------------
	private:
		// Attributes
		int _value;
		OPT(int) _uncertainty;
		OPT(int) _lowerUncertainty;
		OPT(int) _upperUncertainty;
		OPT(double) _confidenceLevel;
};


}
}


#endif
