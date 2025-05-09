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


#ifndef SEISCOMP_IO_RECORDSTREAM_WS_H
#define SEISCOMP_IO_RECORDSTREAM_WS_H

#include <string>
#include <set>
#include <sstream>

#include <seiscomp/core.h>
#include <seiscomp/core/datetime.h>
#include <seiscomp/io/recordstream.h>
#include <seiscomp/io/socket.h>
#include <seiscomp/io/httpsocket.h>
#include <seiscomp/io/recordstream/streamidx.h>


struct _bson_t;
typedef bson_t _bson_t;


namespace Seiscomp {
namespace RecordStream {


class SC_SYSTEM_CORE_API HMBQueue  {
	public:
		//! C'tor
		HMBQueue();

		//! Destructor
		virtual ~HMBQueue();

		//! Adds the given stream
		void addStream(std::string loc, std::string cha,
		               const OPT(Core::Time) &stime,
		               const OPT(Core::Time) &etime);

		//! Sets the sequence number
		void setSequenceNumber(int64_t seq);

		//! Removes all entries
		void clear();

		//! Returns a BSON document
		bson_t* toBSON() const;

	private:
		OPT(Core::Time)       _stime;
		OPT(Core::Time)       _etime;
		int64_t               _seq;
		std::set<std::string> _topics;
};


template<typename SocketType>
class SC_SYSTEM_CORE_API HMBConnection : public Seiscomp::IO::RecordStream {
	//DECLARE_SC_CLASS(HMBConnection<SocketType>);

	public:
		//! C'tor
		HMBConnection();

		//! Initializing Constructor
		HMBConnection(std::string serverloc);

		//! Destructor
		~HMBConnection() override;

		//! The recordtype cannot be selected when using an HMB
		//! connection. It will always create MiniSeed records
		bool setRecordType(const char *type) override;

		//! Initialize the HMB connection.
		bool setSource(const std::string &source) override;

		//! Supply user credentials
		//! Adds the given stream to the server connection description
		bool addStream(const std::string &networkCode,
		               const std::string &stationCode,
		               const std::string &locationCode,
		               const std::string &channelCode) override;

		//! Adds the given stream to the server connection description
		bool addStream(const std::string &networkCode,
		               const std::string &stationCode,
		               const std::string &locationCode,
		               const std::string &channelCode,
		               const OPT(Core::Time) &startTime,
		               const OPT(Core::Time) &endTime) override;

		//! Adds the given start time to the server connection description
		bool setStartTime(const OPT(Core::Time) &stime) override;

		//! Adds the given end time to the server connection description
		bool setEndTime(const OPT(Core::Time) &etime) override;

		//! Sets timeout
		bool setTimeout(int seconds) override;

		//! Terminates the HMB connection.
		void close() override;

		Record *next() override;


	private:
		//! Removes all stream list, time window, etc. -entries from the connection description object.
		bool clear();

		//! Reconnects a terminated HMB connection.
		bool reconnect();


	private:
		IO::HttpSocket<SocketType>                  _sock;
		std::string                                 _serverHost;
		std::string                                 _serverPath;
		std::string                                 _user;
		std::string                                 _password;
		std::set<Seiscomp::RecordStream::StreamIdx> _streams;
		OPT(Core::Time)                             _stime;
		OPT(Core::Time)                             _etime;
		std::map<std::string, HMBQueue>             _queues;
		std::string                                 _sid;
		std::string                                 _cid;
		bool                                        _readingData;

		std::string bsonGetString(const bson_t *bson, const char *key);
		int64_t bsonGetInt(const bson_t *bson, const char *key);
		void bsonGetBlob(const bson_t *bson, const char *key, const void **data, int *data_len);
		void initSession();
		std::string receive();
};

}
}

#endif

