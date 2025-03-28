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


#define SEISCOMP_COMPONENT DataModel
#include <seiscomp/datamodel/timequantity.h>
#include <seiscomp/datamodel/version.h>
#include <seiscomp/datamodel/metadata.h>
#include <seiscomp/logging/log.h>


namespace Seiscomp {
namespace DataModel {


IMPLEMENT_SC_CLASS(TimeQuantity, "TimeQuantity");


TimeQuantity::MetaObject::MetaObject(const Core::RTTI *rtti) : Seiscomp::Core::MetaObject(rtti) {
	addProperty(Core::simpleProperty("value", "datetime", false, false, false, false, false, false, nullptr, &TimeQuantity::setValue, &TimeQuantity::value));
	addProperty(Core::simpleProperty("uncertainty", "float", false, false, false, false, true, false, nullptr, &TimeQuantity::setUncertainty, &TimeQuantity::uncertainty));
	addProperty(Core::simpleProperty("lowerUncertainty", "float", false, false, false, false, true, false, nullptr, &TimeQuantity::setLowerUncertainty, &TimeQuantity::lowerUncertainty));
	addProperty(Core::simpleProperty("upperUncertainty", "float", false, false, false, false, true, false, nullptr, &TimeQuantity::setUpperUncertainty, &TimeQuantity::upperUncertainty));
	addProperty(Core::simpleProperty("confidenceLevel", "float", false, false, false, false, true, false, nullptr, &TimeQuantity::setConfidenceLevel, &TimeQuantity::confidenceLevel));
	addProperty(objectProperty<TimePDF1D>("pdf", "TimePDF1D", false, false, true, &TimeQuantity::setPdf, &TimeQuantity::pdf));
}


IMPLEMENT_METAOBJECT(TimeQuantity)


TimeQuantity::TimeQuantity() {
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
TimeQuantity::TimeQuantity(const TimeQuantity &other)
: Core::BaseObject() {
	*this = other;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
TimeQuantity::TimeQuantity(Seiscomp::Core::Time value,
                           const OPT(double)& uncertainty,
                           const OPT(double)& lowerUncertainty,
                           const OPT(double)& upperUncertainty,
                           const OPT(double)& confidenceLevel,
                           const OPT(TimePDF1D)& pdf)
: _value(value)
, _uncertainty(uncertainty)
, _lowerUncertainty(lowerUncertainty)
, _upperUncertainty(upperUncertainty)
, _confidenceLevel(confidenceLevel)
, _pdf(pdf)
{}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
TimeQuantity::~TimeQuantity() {
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
TimeQuantity::operator Seiscomp::Core::Time&() {
	return _value;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
TimeQuantity::operator Seiscomp::Core::Time() const {
	return _value;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool TimeQuantity::operator==(const TimeQuantity &rhs) const {
	if ( !(_value == rhs._value) )
		return false;
	if ( !(_uncertainty == rhs._uncertainty) )
		return false;
	if ( !(_lowerUncertainty == rhs._lowerUncertainty) )
		return false;
	if ( !(_upperUncertainty == rhs._upperUncertainty) )
		return false;
	if ( !(_confidenceLevel == rhs._confidenceLevel) )
		return false;
	if ( !(_pdf == rhs._pdf) )
		return false;
	return true;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool TimeQuantity::operator!=(const TimeQuantity &rhs) const {
	return !operator==(rhs);
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool TimeQuantity::equal(const TimeQuantity &other) const {
	return *this == other;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void TimeQuantity::setValue(Seiscomp::Core::Time value) {
	_value = value;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Seiscomp::Core::Time TimeQuantity::value() const {
	return _value;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void TimeQuantity::setUncertainty(const OPT(double)& uncertainty) {
	_uncertainty = uncertainty;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double TimeQuantity::uncertainty() const {
	if ( _uncertainty )
		return *_uncertainty;
	throw Seiscomp::Core::ValueException("TimeQuantity.uncertainty is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void TimeQuantity::setLowerUncertainty(const OPT(double)& lowerUncertainty) {
	_lowerUncertainty = lowerUncertainty;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double TimeQuantity::lowerUncertainty() const {
	if ( _lowerUncertainty )
		return *_lowerUncertainty;
	throw Seiscomp::Core::ValueException("TimeQuantity.lowerUncertainty is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void TimeQuantity::setUpperUncertainty(const OPT(double)& upperUncertainty) {
	_upperUncertainty = upperUncertainty;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double TimeQuantity::upperUncertainty() const {
	if ( _upperUncertainty )
		return *_upperUncertainty;
	throw Seiscomp::Core::ValueException("TimeQuantity.upperUncertainty is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void TimeQuantity::setConfidenceLevel(const OPT(double)& confidenceLevel) {
	_confidenceLevel = confidenceLevel;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double TimeQuantity::confidenceLevel() const {
	if ( _confidenceLevel )
		return *_confidenceLevel;
	throw Seiscomp::Core::ValueException("TimeQuantity.confidenceLevel is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void TimeQuantity::setPdf(const OPT(TimePDF1D)& pdf) {
	_pdf = pdf;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
TimePDF1D& TimeQuantity::pdf() {
	if ( _pdf )
		return *_pdf;
	throw Seiscomp::Core::ValueException("TimeQuantity.pdf is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
const TimePDF1D& TimeQuantity::pdf() const {
	if ( _pdf )
		return *_pdf;
	throw Seiscomp::Core::ValueException("TimeQuantity.pdf is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
TimeQuantity &TimeQuantity::operator=(const TimeQuantity &other) {
	_value = other._value;
	_uncertainty = other._uncertainty;
	_lowerUncertainty = other._lowerUncertainty;
	_upperUncertainty = other._upperUncertainty;
	_confidenceLevel = other._confidenceLevel;
	_pdf = other._pdf;
	return *this;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void TimeQuantity::serialize(Archive &ar) {
	// Do not read/write if the archive's version is higher than
	// currently supported
	if ( ar.isHigherVersion<Version::Major,Version::Minor>() ) {
		SEISCOMP_ERROR("Archive version %d.%d too high: TimeQuantity skipped",
		               ar.versionMajor(), ar.versionMinor());
		ar.setValidity(false);
		return;
	}

	ar & NAMED_OBJECT_HINT("value", _value, Archive::XML_ELEMENT | Archive::SPLIT_TIME | Archive::XML_MANDATORY);
	ar & NAMED_OBJECT_HINT("uncertainty", _uncertainty, Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("lowerUncertainty", _lowerUncertainty, Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("upperUncertainty", _upperUncertainty, Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("confidenceLevel", _confidenceLevel, Archive::XML_ELEMENT);
	if ( ar.supportsVersion<0,10>() ) {
		ar & NAMED_OBJECT_HINT("pdf", _pdf, Archive::STATIC_TYPE | Archive::XML_ELEMENT);
	}
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}
}
