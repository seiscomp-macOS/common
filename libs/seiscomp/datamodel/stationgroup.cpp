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
#include <seiscomp/datamodel/stationgroup.h>
#include <seiscomp/datamodel/inventory.h>
#include <algorithm>
#include <seiscomp/datamodel/version.h>
#include <seiscomp/datamodel/metadata.h>
#include <seiscomp/logging/log.h>


namespace Seiscomp {
namespace DataModel {


IMPLEMENT_SC_CLASS_DERIVED(StationGroup, PublicObject, "StationGroup");


namespace {
static Seiscomp::Core::MetaEnumImpl<StationGroupType> metaStationGroupType;
}


StationGroup::MetaObject::MetaObject(const Core::RTTI *rtti) : Seiscomp::Core::MetaObject(rtti) {
	addProperty(enumProperty("type", "StationGroupType", false, true, &metaStationGroupType, &StationGroup::setType, &StationGroup::type));
	addProperty(Core::simpleProperty("code", "string", false, false, true, false, false, false, nullptr, &StationGroup::setCode, &StationGroup::code));
	addProperty(Core::simpleProperty("start", "datetime", false, false, false, false, true, false, nullptr, &StationGroup::setStart, &StationGroup::start));
	addProperty(Core::simpleProperty("end", "datetime", false, false, false, false, true, false, nullptr, &StationGroup::setEnd, &StationGroup::end));
	addProperty(Core::simpleProperty("description", "string", false, false, false, false, false, false, nullptr, &StationGroup::setDescription, &StationGroup::description));
	addProperty(Core::simpleProperty("latitude", "float", false, false, false, false, true, false, nullptr, &StationGroup::setLatitude, &StationGroup::latitude));
	addProperty(Core::simpleProperty("longitude", "float", false, false, false, false, true, false, nullptr, &StationGroup::setLongitude, &StationGroup::longitude));
	addProperty(Core::simpleProperty("elevation", "float", false, false, false, false, true, false, nullptr, &StationGroup::setElevation, &StationGroup::elevation));
	addProperty(arrayClassProperty<StationReference>("stationReference", "StationReference", &StationGroup::stationReferenceCount, &StationGroup::stationReference, static_cast<bool (StationGroup::*)(StationReference*)>(&StationGroup::add), &StationGroup::removeStationReference, static_cast<bool (StationGroup::*)(StationReference*)>(&StationGroup::remove)));
}


IMPLEMENT_METAOBJECT(StationGroup)


StationGroupIndex::StationGroupIndex() {
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationGroupIndex::StationGroupIndex(const std::string& code_) {
	code = code_;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationGroupIndex::StationGroupIndex(const StationGroupIndex &idx) {
	code = idx.code;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroupIndex::operator==(const StationGroupIndex &idx) const {
	return code == idx.code;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroupIndex::operator!=(const StationGroupIndex &idx) const {
	return !operator==(idx);
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationGroup::StationGroup() {
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationGroup::StationGroup(const StationGroup &other)
: PublicObject() {
	*this = other;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationGroup::StationGroup(const std::string& publicID)
: PublicObject(publicID) {
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationGroup::~StationGroup() {
	for ( auto &stationReference : _stationReferences ) {
		stationReference->setParent(nullptr);
	}
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationGroup *StationGroup::Create() {
	StationGroup *object = new StationGroup();
	return static_cast<StationGroup*>(GenerateId(object));
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationGroup *StationGroup::Create(const std::string& publicID) {
	if ( PublicObject::IsRegistrationEnabled() && Find(publicID) != nullptr ) {
		SEISCOMP_ERROR(
			"There exists already a PublicObject with Id '%s'",
			publicID.c_str()
		);
		return nullptr;
	}

	return new StationGroup(publicID);
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationGroup *StationGroup::Find(const std::string& publicID) {
	return StationGroup::Cast(PublicObject::Find(publicID));
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::operator==(const StationGroup &rhs) const {
	if ( _index != rhs._index ) return false;
	if ( _type != rhs._type ) return false;
	if ( _start != rhs._start ) return false;
	if ( _end != rhs._end ) return false;
	if ( _description != rhs._description ) return false;
	if ( _latitude != rhs._latitude ) return false;
	if ( _longitude != rhs._longitude ) return false;
	if ( _elevation != rhs._elevation ) return false;
	return true;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::operator!=(const StationGroup &rhs) const {
	return !operator==(rhs);
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::equal(const StationGroup &other) const {
	return *this == other;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void StationGroup::setType(const OPT(StationGroupType)& type) {
	_type = type;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationGroupType StationGroup::type() const {
	if ( _type )
		return *_type;
	throw Seiscomp::Core::ValueException("StationGroup.type is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void StationGroup::setCode(const std::string& code) {
	_index.code = code;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
const std::string& StationGroup::code() const {
	return _index.code;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void StationGroup::setStart(const OPT(Seiscomp::Core::Time)& start) {
	_start = start;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Seiscomp::Core::Time StationGroup::start() const {
	if ( _start )
		return *_start;
	throw Seiscomp::Core::ValueException("StationGroup.start is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void StationGroup::setEnd(const OPT(Seiscomp::Core::Time)& end) {
	_end = end;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Seiscomp::Core::Time StationGroup::end() const {
	if ( _end )
		return *_end;
	throw Seiscomp::Core::ValueException("StationGroup.end is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void StationGroup::setDescription(const std::string& description) {
	_description = description;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
const std::string& StationGroup::description() const {
	return _description;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void StationGroup::setLatitude(const OPT(double)& latitude) {
	_latitude = latitude;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double StationGroup::latitude() const {
	if ( _latitude )
		return *_latitude;
	throw Seiscomp::Core::ValueException("StationGroup.latitude is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void StationGroup::setLongitude(const OPT(double)& longitude) {
	_longitude = longitude;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double StationGroup::longitude() const {
	if ( _longitude )
		return *_longitude;
	throw Seiscomp::Core::ValueException("StationGroup.longitude is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void StationGroup::setElevation(const OPT(double)& elevation) {
	_elevation = elevation;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
double StationGroup::elevation() const {
	if ( _elevation )
		return *_elevation;
	throw Seiscomp::Core::ValueException("StationGroup.elevation is not set");
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
const StationGroupIndex &StationGroup::index() const {
	return _index;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::equalIndex(const StationGroup *lhs) const {
	if ( !lhs ) {
		return false;
	}

	return lhs->index() == index();
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Inventory *StationGroup::inventory() const {
	return static_cast<Inventory*>(parent());
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationGroup &StationGroup::operator=(const StationGroup &other) {
	PublicObject::operator=(other);
	_index = other._index;
	_type = other._type;
	_start = other._start;
	_end = other._end;
	_description = other._description;
	_latitude = other._latitude;
	_longitude = other._longitude;
	_elevation = other._elevation;
	return *this;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::assign(Object *other) {
	StationGroup *otherStationGroup = StationGroup::Cast(other);
	if ( !other ) {
		return false;
	}

	*this = *otherStationGroup;

	return true;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::attachTo(PublicObject *parent) {
	if ( !parent ) {
		return false;
	}

	// check all possible parents
	Inventory *inventory = Inventory::Cast(parent);
	if ( inventory != nullptr )
		return inventory->add(this);

	SEISCOMP_ERROR("StationGroup::attachTo(%s) -> wrong class type", parent->className());
	return false;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::detachFrom(PublicObject *object) {
	if ( !object ) {
		return false;
	}

	// check all possible parents
	Inventory *inventory = Inventory::Cast(object);
	if ( inventory != nullptr ) {
		// If the object has been added already to the parent locally
		// just remove it by pointer
		if ( object == parent() )
			return inventory->remove(this);
		// The object has not been added locally so it must be looked up
		else {
			StationGroup *child = inventory->findStationGroup(publicID());
			if ( child != nullptr )
				return inventory->remove(child);
			else {
				SEISCOMP_DEBUG("StationGroup::detachFrom(Inventory): stationGroup has not been found");
				return false;
			}
		}
	}

	SEISCOMP_ERROR("StationGroup::detachFrom(%s) -> wrong class type", object->className());
	return false;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::detach() {
	if ( !parent() ) {
		return false;
	}

	return detachFrom(parent());
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Object *StationGroup::clone() const {
	StationGroup *clonee = new StationGroup();
	*clonee = *this;
	return clonee;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::updateChild(Object *child) {
	StationReference *stationReferenceChild = StationReference::Cast(child);
	if ( stationReferenceChild != nullptr ) {
		StationReference *stationReferenceElement = stationReference(stationReferenceChild->index());
		if ( stationReferenceElement != nullptr ) {
			*stationReferenceElement = *stationReferenceChild;
			stationReferenceElement->update();
			return true;
		}
		return false;
	}

	return false;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void StationGroup::accept(Visitor *visitor) {
	if ( visitor->traversal() == Visitor::TM_TOPDOWN )
		if ( !visitor->visit(this) )
			return;

	for ( auto &&elem : _stationReferences )
		elem->accept(visitor);

	if ( visitor->traversal() == Visitor::TM_BOTTOMUP )
		visitor->visit(this);
	else
		visitor->finished();
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
size_t StationGroup::stationReferenceCount() const {
	return _stationReferences.size();
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationReference *StationGroup::stationReference(size_t i) const {
	return _stationReferences[i].get();
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StationReference *StationGroup::stationReference(const StationReferenceIndex &i) const {
	for ( const auto &elem : _stationReferences ) {
		if ( i == elem->index() ) {
			return elem.get();
		}
	}

	return nullptr;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::add(StationReference *stationReference) {
	if ( !stationReference ) {
		return false;
	}

	// Element has already a parent
	if ( stationReference->parent() != nullptr ) {
		SEISCOMP_ERROR("StationGroup::add(StationReference*) -> element has already a parent");
		return false;
	}

	// Duplicate index check
	for ( auto &&elem : _stationReferences ) {
		if ( elem->index() == stationReference->index() ) {
			SEISCOMP_ERROR("StationGroup::add(StationReference*) -> an element with the same index has been added already");
			return false;
		}
	}

	// Add the element
	_stationReferences.push_back(stationReference);
	stationReference->setParent(this);

	// Create the notifiers
	if ( Notifier::IsEnabled() ) {
		NotifierCreator nc(OP_ADD);
		stationReference->accept(&nc);
	}

	// Notify registered observers
	childAdded(stationReference);

	return true;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::remove(StationReference *stationReference) {
	if ( !stationReference ) {
		return false;
	}

	if ( stationReference->parent() != this ) {
		SEISCOMP_ERROR("StationGroup::remove(StationReference*) -> element has another parent");
		return false;
	}

	auto it = std::find(_stationReferences.begin(), _stationReferences.end(), stationReference);
	// Element has not been found
	if ( it == _stationReferences.end() ) {
		SEISCOMP_ERROR("StationGroup::remove(StationReference*) -> child object has not been found although the parent pointer matches???");
		return false;
	}

	// Create the notifiers
	if ( Notifier::IsEnabled() ) {
		Notifier::Create(this, OP_REMOVE, it->get());
	}

	(*it)->setParent(nullptr);
	childRemoved((*it).get());

	_stationReferences.erase(it);

	return true;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::removeStationReference(size_t i) {
	// index out of bounds
	if ( i >= _stationReferences.size() )
		return false;

	// Create the notifiers
	if ( Notifier::IsEnabled() ) {
		Notifier::Create(this, OP_REMOVE, _stationReferences[i].get());
	}

	_stationReferences[i]->setParent(nullptr);
	childRemoved(_stationReferences[i].get());

	_stationReferences.erase(_stationReferences.begin() + i);

	return true;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StationGroup::removeStationReference(const StationReferenceIndex &i) {
	StationReference *object = stationReference(i);
	if ( !object ) {
		return false;
	}

	return remove(object);
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void StationGroup::serialize(Archive &ar) {
	// Do not read/write if the archive's version is higher than
	// currently supported
	if ( ar.isHigherVersion<Version::Major,Version::Minor>() ) {
		SEISCOMP_ERROR("Archive version %d.%d too high: StationGroup skipped",
		               ar.versionMajor(), ar.versionMinor());
		ar.setValidity(false);
		return;
	}

	PublicObject::serialize(ar);
	if ( !ar.success() ) return;

	ar & NAMED_OBJECT_HINT("type", _type, Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("code", _index.code, Archive::INDEX_ATTRIBUTE);
	if ( ar.supportsVersion<0,10>() )
		ar & NAMED_OBJECT_HINT("start", _start, Archive::XML_ELEMENT | Archive::SPLIT_TIME);
	else
		ar & NAMED_OBJECT_HINT("start", _start, Archive::XML_ELEMENT);
	if ( ar.supportsVersion<0,10>() )
		ar & NAMED_OBJECT_HINT("end", _end, Archive::XML_ELEMENT | Archive::SPLIT_TIME);
	else
		ar & NAMED_OBJECT_HINT("end", _end, Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("description", _description, Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("latitude", _latitude, Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("longitude", _longitude, Archive::XML_ELEMENT);
	ar & NAMED_OBJECT_HINT("elevation", _elevation, Archive::XML_ELEMENT);
	if ( ar.hint() & Archive::IGNORE_CHILDS ) return;
	ar & NAMED_OBJECT_HINT("stationReference",
		Seiscomp::Core::Generic::containerMember(
			_stationReferences,
			[this](const StationReferencePtr &stationReference) {
				return add(stationReference.get());
			}
		),
		Archive::STATIC_TYPE
	);
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}
}
