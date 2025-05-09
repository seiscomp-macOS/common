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


#ifndef SEISCOMP_DATAMODEL_NETWORK_H
#define SEISCOMP_DATAMODEL_NETWORK_H


#include <string>
#include <seiscomp/core/datetime.h>
#include <seiscomp/datamodel/blob.h>
#include <vector>
#include <seiscomp/datamodel/comment.h>
#include <seiscomp/datamodel/station.h>
#include <seiscomp/datamodel/notifier.h>
#include <seiscomp/datamodel/publicobject.h>
#include <seiscomp/core/exceptions.h>


namespace Seiscomp {
namespace DataModel {


DEFINE_SMARTPOINTER(Network);
DEFINE_SMARTPOINTER(Comment);
DEFINE_SMARTPOINTER(Station);

class Inventory;


class SC_SYSTEM_CORE_API NetworkIndex {
	// ------------------------------------------------------------------
	//  Xstruction
	// ------------------------------------------------------------------
	public:
		//! Constructor
		NetworkIndex();
		NetworkIndex(const std::string& code,
		             Seiscomp::Core::Time start);

		//! Copy constructor
		NetworkIndex(const NetworkIndex&);


	// ------------------------------------------------------------------
	//  Operators
	// ------------------------------------------------------------------
	public:
		bool operator==(const NetworkIndex&) const;
		bool operator!=(const NetworkIndex&) const;


	// ------------------------------------------------------------------
	//  Attributes
	// ------------------------------------------------------------------
	public:
		std::string code;
		Seiscomp::Core::Time start;
};


/**
 * \brief This type describes a network of seismic stations
 */
class SC_SYSTEM_CORE_API Network : public PublicObject {
	DECLARE_SC_CLASS(Network)
	DECLARE_SERIALIZATION;
	DECLARE_METAOBJECT;

	// ------------------------------------------------------------------
	//  Xstruction
	// ------------------------------------------------------------------
	protected:
		//! Protected constructor
		Network();

	public:
		//! Copy constructor
		Network(const Network &other);

		//! Constructor with publicID
		Network(const std::string& publicID);

		//! Destructor
		~Network() override;


	// ------------------------------------------------------------------
	//  Creators
	// ------------------------------------------------------------------
	public:
		static Network *Create();
		static Network *Create(const std::string& publicID);


	// ------------------------------------------------------------------
	//  Lookup
	// ------------------------------------------------------------------
	public:
		static Network *Find(const std::string& publicID);


	// ------------------------------------------------------------------
	//  Operators
	// ------------------------------------------------------------------
	public:
		//! Copies the metadata of other to this
		//! No changes regarding child objects are made
		Network &operator=(const Network &other);
		//! Checks for equality of two objects. Child objects
		//! are not part of the check.
		bool operator==(const Network &other) const;
		bool operator!=(const Network &other) const;

		//! Wrapper that calls operator==
		bool equal(const Network &other) const;


	// ------------------------------------------------------------------
	//  Setters/Getters
	// ------------------------------------------------------------------
	public:
		//! Network code (50.16)
		void setCode(const std::string& code);
		const std::string& code() const;

		//! Start of network epoch in ISO datetime format. Needed
		//! primarily to identifytemorary networks that re-use network
		//! codes
		void setStart(Seiscomp::Core::Time start);
		Seiscomp::Core::Time start() const;

		//! End of station epoch. Empty string if the station is open
		void setEnd(const OPT(Seiscomp::Core::Time)& end);
		Seiscomp::Core::Time end() const;

		//! Network description (50.10)
		void setDescription(const std::string& description);
		const std::string& description() const;

		//! Institution(s) operating the network
		void setInstitutions(const std::string& institutions);
		const std::string& institutions() const;

		//! Region of the network (eg., euromed, global)
		void setRegion(const std::string& region);
		const std::string& region() const;

		//! Type of the network (eg., VBB, SP)
		void setType(const std::string& type);
		const std::string& type() const;

		//! ';p'; for permanent, ';t'; for temporary
		void setNetClass(const std::string& netClass);
		const std::string& netClass() const;

		//! Archive/Datacenter ID (metadata authority)
		void setArchive(const std::string& archive);
		const std::string& archive() const;

		//! Whether the network is "restricted"
		void setRestricted(const OPT(bool)& restricted);
		bool restricted() const;

		//! Whether the metadata is synchronized with other datacenters
		void setShared(const OPT(bool)& shared);
		bool shared() const;

		//! Any notes
		void setRemark(const OPT(Blob)& remark);
		Blob& remark();
		const Blob& remark() const;


	// ------------------------------------------------------------------
	//  Index management
	// ------------------------------------------------------------------
	public:
		//! Returns the object's index
		const NetworkIndex &index() const;

		//! Checks two objects for equality regarding their index
		bool equalIndex(const Network *lhs) const;


	// ------------------------------------------------------------------
	//  Public interface
	// ------------------------------------------------------------------
	public:
		/**
		 * Add an object.
		 * @param obj The object pointer
		 * @return true The object has been added
		 * @return false The object has not been added
		 *               because it already exists in the list
		 *               or it already has another parent
		 */
		bool add(Comment *obj);
		bool add(Station *obj);

		/**
		 * Removes an object.
		 * @param obj The object pointer
		 * @return true The object has been removed
		 * @return false The object has not been removed
		 *               because it does not exist in the list
		 */
		bool remove(Comment *obj);
		bool remove(Station *obj);

		/**
		 * Removes an object of a particular class.
		 * @param i The object index
		 * @return true The object has been removed
		 * @return false The index is out of bounds
		 */
		bool removeComment(size_t i);
		bool removeComment(const CommentIndex &i);
		bool removeStation(size_t i);
		bool removeStation(const StationIndex &i);

		//! Retrieve the number of objects of a particular class
		size_t commentCount() const;
		size_t stationCount() const;

		//! Index access
		//! @return The object at index i
		Comment *comment(size_t i) const;
		Comment *comment(const CommentIndex &i) const;

		Station *station(size_t i) const;
		Station *station(const StationIndex &i) const;

		//! Find an object by its unique attribute(s)
		Station *findStation(const std::string& publicID) const;

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
		NetworkIndex _index;

		// Attributes
		OPT(Seiscomp::Core::Time) _end;
		std::string _description;
		std::string _institutions;
		std::string _region;
		std::string _type;
		std::string _netClass;
		std::string _archive;
		OPT(bool) _restricted;
		OPT(bool) _shared;
		OPT(Blob) _remark;

		// Aggregations
		std::vector<CommentPtr> _comments;
		std::vector<StationPtr> _stations;

	DECLARE_SC_CLASSFACTORY_FRIEND(Network);
};


}
}


#endif
