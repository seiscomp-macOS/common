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
#include <seiscomp/datamodel/originuncertainty.h>
#include <seiscomp/datamodel/version.h>
#include <seiscomp/datamodel/metadata.h>
#include <seiscomp/logging/log.h>


namespace Seiscomp {
namespace DataModel {


IMPLEMENT_SC_CLASS(OriginUncertainty, "OriginUncertainty");


namespace {
static Seiscomp::Core::MetaEnumImpl<OriginUncertaintyDescription> metaOriginUncertaintyDescription;
}


OriginUncertainty::MetaObject::MetaObject(const Core::RTTI *rtti) : Seiscomp::Core::MetaObject(rtti) {
	addProperty(Core::simpleProperty("horizontalUncertainty", "float", false, false, false, false, true, false, nullptr, &OriginUncertainty::setHorizontalUncertainty, &OriginUncertainty::horizontalUncertainty));
	addProperty(Core::simpleProperty("minHorizontalUncertainty", "float", false, false, false, false, true, false, nullptr, &OriginUncertainty::setMinHorizontalUncertainty, &OriginUncertainty::minHorizontalUncertainty));
	addProperty(Core::simpleProperty("maxHorizontalUncertainty", "float", false, false, false, false, true, false, nullptr, &OriginUncertainty::setMaxHorizontalUncertainty, &OriginUncertainty::maxHorizontalUncertainty));
	addProperty(Core::simpleProperty("azimuthMaxHorizontalUncertainty", "float", false, false, false, false, true, false, nullptr, &OriginUncertainty::setAzimuthMaxHorizontalUncertainty, &OriginUncertainty::azimuthMaxHorizontalUncertainty));
	addProperty(objectProperty<ConfidenceEllipsoid>("confidenceEllipsoid", "ConfidenceEllipsoid", false, false, true, &OriginUncertainty::setConfidenceEllipsoid, &OriginUncertainty::confidenceEllipsoid));
	addProperty(enumProperty("preferredDescription", "OriginUncertaintyDescription", false, true, &metaOriginUncertaintyDescription, &OriginUncertainty::setPreferredDescription, &OriginUncertainty::preferredDescription));
	addProperty(Core::simpleProperty("confidenceLevel", "float", false, false, false, false, true, false, nullptr, &OriginUncertainty::setConfidenceLevel, &OriginUncertainty::confidenceLevel));
}


IMPLEMENT_METAOBJECT(OriginUncertainty)


OriginUncertainty::OriginUncertainty() {
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
OriginUncertainty::OriginUncertainty(const OriginUncertainty &other)
: Core::BaseObject() {
	*this = other;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
OriginUncertainty::~OriginUncertainty() {
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool OriginUncertainty::operator==(const OriginUncertainty &rhs) const {
	if ( !(_horizontalUncertainty == rhs._horizontalUncertainty) )
		return false;
	if ( !(_minHorizontalUncertainty == rhs._minHorizontalUncertainty) )
		return false;
	if ( !(_maxHorizontalUncertainty == rhs._maxHorizontalUncertainty) )
		return false;
	if ( !(_azimuthMaxHorizontalUncertainty == rhs._azimuthMaxHorizontalUncertainty) )
		return false;
	if ( !(_confidenceEllipsoid == rhs._confidenceEllipsoid) )
		return false;
	if ( !(_preferredDescription == rhs._preferredDescription) )
		return false;
	if ( !(_confidenceLevel == rhs._confidenceLevel) )
		return false;
	return true;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool OriginUncertainty::operator!=(const OriginUncertainty &rhs) const {
	return !operator==(rhs);
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool OriginUncertainty::equal(const OriginUncertainty &other) const {
	return *this == other;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void OriginUncertainty::setHorizontalUncertainty(const OPT(double)& horizontalUncertainty) {
	_horizontalUncertainty = horizontalUncertainty;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double OriginUncertainty::horizontalUncertainty() const {
	if ( _horizontalUncertainty )
		return *_horizontalUncertainty;
	throw Seiscomp::Core::ValueException("OriginUncertainty.horizontalUncertainty is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void OriginUncertainty::setMinHorizontalUncertainty(const OPT(double)& minHorizontalUncertainty) {
	_minHorizontalUncertainty = minHorizontalUncertainty;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double OriginUncertainty::minHorizontalUncertainty() const {
	if ( _minHorizontalUncertainty )
		return *_minHorizontalUncertainty;
	throw Seiscomp::Core::ValueException("OriginUncertainty.minHorizontalUncertainty is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void OriginUncertainty::setMaxHorizontalUncertainty(const OPT(double)& maxHorizontalUncertainty) {
	_maxHorizontalUncertainty = maxHorizontalUncertainty;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double OriginUncertainty::maxHorizontalUncertainty() const {
	if ( _maxHorizontalUncertainty )
		return *_maxHorizontalUncertainty;
	throw Seiscomp::Core::ValueException("OriginUncertainty.maxHorizontalUncertainty is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void OriginUncertainty::setAzimuthMaxHorizontalUncertainty(const OPT(double)& azimuthMaxHorizontalUncertainty) {
	_azimuthMaxHorizontalUncertainty = azimuthMaxHorizontalUncertainty;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double OriginUncertainty::azimuthMaxHorizontalUncertainty() const {
	if ( _azimuthMaxHorizontalUncertainty )
		return *_azimuthMaxHorizontalUncertainty;
	throw Seiscomp::Core::ValueException("OriginUncertainty.azimuthMaxHorizontalUncertainty is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void OriginUncertainty::setConfidenceEllipsoid(const OPT(ConfidenceEllipsoid)& confidenceEllipsoid) {
	_confidenceEllipsoid = confidenceEllipsoid;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
ConfidenceEllipsoid& OriginUncertainty::confidenceEllipsoid() {
	if ( _confidenceEllipsoid )
		return *_confidenceEllipsoid;
	throw Seiscomp::Core::ValueException("OriginUncertainty.confidenceEllipsoid is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
const ConfidenceEllipsoid& OriginUncertainty::confidenceEllipsoid() const {
	if ( _confidenceEllipsoid )
		return *_confidenceEllipsoid;
	throw Seiscomp::Core::ValueException("OriginUncertainty.confidenceEllipsoid is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void OriginUncertainty::setPreferredDescription(const OPT(OriginUncertaintyDescription)& preferredDescription) {
	_preferredDescription = preferredDescription;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
OriginUncertaintyDescription OriginUncertainty::preferredDescription() const {
	if ( _preferredDescription )
		return *_preferredDescription;
	throw Seiscomp::Core::ValueException("OriginUncertainty.preferredDescription is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void OriginUncertainty::setConfidenceLevel(const OPT(double)& confidenceLevel) {
	_confidenceLevel = confidenceLevel;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double OriginUncertainty::confidenceLevel() const {
	if ( _confidenceLevel )
		return *_confidenceLevel;
	throw Seiscomp::Core::ValueException("OriginUncertainty.confidenceLevel is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
OriginUncertainty &OriginUncertainty::operator=(const OriginUncertainty &other) {
	_horizontalUncertainty = other._horizontalUncertainty;
	_minHorizontalUncertainty = other._minHorizontalUncertainty;
	_maxHorizontalUncertainty = other._maxHorizontalUncertainty;
	_azimuthMaxHorizontalUncertainty = other._azimuthMaxHorizontalUncertainty;
	_confidenceEllipsoid = other._confidenceEllipsoid;
	_preferredDescription = other._preferredDescription;
	_confidenceLevel = other._confidenceLevel;
	return *this;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void OriginUncertainty::serialize(Archive &ar) {
	// Do not read/write if the archive's version is higher than
	// currently supported
	if ( ar.isHigherVersion<Version::Major,Version::Minor>() ) {
		SEISCOMP_ERROR("Archive version %d.%d too high: OriginUncertainty skipped",
		               ar.versionMajor(), ar.versionMinor());
		ar.setValidity(false);
		return;
	}

	ar & NAMED_OBJECT_HINT("horizontalUncertainty", _horizontalUncertainty, Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("minHorizontalUncertainty", _minHorizontalUncertainty, Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("maxHorizontalUncertainty", _maxHorizontalUncertainty, Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("azimuthMaxHorizontalUncertainty", _azimuthMaxHorizontalUncertainty, Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("confidenceEllipsoid", _confidenceEllipsoid, Archive::STATIC_TYPE | Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("preferredDescription", _preferredDescription, Archive::XML_ELEMENT);
	if ( ar.supportsVersion<0,12>() ) {
		ar & NAMED_OBJECT_HINT("confidenceLevel", _confidenceLevel, Archive::XML_ELEMENT);
	}
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}
}
