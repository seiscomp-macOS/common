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


#ifndef SEISCOMP_SERVICES_RECORDSTREAM_MEMORY_H
#define SEISCOMP_SERVICES_RECORDSTREAM_MEMORY_H

#include <iostream>
#include <sstream>
#include <seiscomp/io/recordstream.h>
#include <seiscomp/core.h>


namespace Seiscomp {
namespace RecordStream {


DEFINE_SMARTPOINTER(Memory);

class SC_SYSTEM_CORE_API Memory:  public Seiscomp::IO::RecordStream {
	DECLARE_SC_CLASS(Memory);

	// ----------------------------------------------------------------------
	//  X'truction
	// ----------------------------------------------------------------------
	public:
		Memory();
		Memory(const char *data, int size);
		Memory(const Memory &mem);
		virtual ~Memory();


	// ----------------------------------------------------------------------
	//  X'truction
	// ----------------------------------------------------------------------
	public:
		Memory &operator=(const Memory &mem);

		bool setSource(const std::string &) override;
		bool addStream(const std::string &networkCode,
		               const std::string &stationCode,
		               const std::string &locationCode,
		               const std::string &channelCode) override;

		bool addStream(const std::string &networkCode,
		               const std::string &stationCode,
		               const std::string &locationCode,
		               const std::string &channelCode,
		               const OPT(Seiscomp::Core::Time) &startTime,
		               const OPT(Seiscomp::Core::Time) &endTime) override;

		bool setStartTime(const OPT(Seiscomp::Core::Time) &stime) override;
		bool setEndTime(const OPT(Seiscomp::Core::Time) &etime) override;

		void close() override;

		bool setRecordType(const char *type) override;

		Record *next() override;


	private:
		RecordFactory      *_factory;
		std::istringstream  _stream;
};

}
}

#endif
