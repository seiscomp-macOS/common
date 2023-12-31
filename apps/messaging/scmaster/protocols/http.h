/***************************************************************************
 * Copyright (C) gempa GmbH                                                *
 * All rights reserved.                                                    *
 * Contact: gempa GmbH (seiscomp-dev@gempa.de)                             *
 *                                                                         *
 * Author: Jan Becker                                                      *
 * Email: jabe@gempa.de                                                    *
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


#ifndef SEISCOMP_BROKER_PROTOCOL_HTTP_H
#define SEISCOMP_BROKER_PROTOCOL_HTTP_H


#include <seiscomp/wired/protocols/http.h>
#include <seiscomp/io/database.h>
#include "../server.h"


namespace Seiscomp {
namespace Messaging {
namespace Protocols {


/**
 * @brief The WsSession class implements a WebSocket session that communicates
 *        via the WebSocket protocol with the clients.
 */
class HttpSession : public Wired::HttpSession {
	public:
		HttpSession(Wired::Socket *sock, Broker::Server *server);

	public:
		bool handleRequest(Wired::HttpRequest &req) override;
		bool handleGETRequest(Wired::HttpRequest &req) override;
		bool handlePOSTRequest(Wired::HttpRequest &req) override;

		void setAuthenticated(bool auth) { _isAuthenticated = auth; }
		bool isAuthenticated() const { return _isAuthenticated; }

		void setAuthorized(bool auth) { _isAuthorized = auth; }
		bool isAuthorized() const { return _isAuthorized; }

		Broker::Server *server() const {
			return _server;
		}

	protected:
		virtual bool handleBrokerUpgrade(const std::string &requestQueue);
		virtual bool handleDatabaseUpgrade(const std::string &dbURL);

	protected:
		Broker::Server *_server;

		bool            _isAuthenticated{true};
		bool            _isAuthorized{true};
};


}
}
}


#endif
