/* Copyright (C) 2010 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NETSESSION_H
#define NETSESSION_H

#include "network/fsm.h"
#include "network/NetHost.h"
#include "ps/CStr.h"
#include "scriptinterface/ScriptVal.h"

class CNetClient;
class CNetServerWorker;

class CNetStatsTable;

/**
 * @file
 * Network client/server sessions.
 *
 * Each session has two classes: CNetClientSession runs on the client,
 * and CNetServerSession runs on the server.
 * A client runs one session at once; a server typically runs many.
 */

/**
 * The client end of a network session.
 * Provides an abstraction of the network interface, allowing communication with the server.
 */
class CNetClientSession
{
	NONCOPYABLE(CNetClientSession);

public:
	CNetClientSession(CNetClient& client);
	~CNetClientSession();

	bool Connect(u16 port, const CStr& server);

	/**
	 * Process queued incoming messages.
	 */
	void Poll();

	/**
	 * Flush queued outgoing network messages.
	 */
	void Flush();

	/**
	 * Disconnect from the server.
	 * Sends a disconnection notification to the server.
	 */
	void Disconnect(u32 reason);

	/**
	 * Send a message to the server.
	 */
	bool SendMessage(const CNetMessage* message);

private:
	CNetClient& m_Client;
	ENetHost* m_Host;
	ENetPeer* m_Server;
	CNetStatsTable* m_Stats;
};


/**
 * The server's end of a network session.
 * Represents an abstraction of the state of the client, storing all the per-client data
 * needed by the server.
 *
 * Thread-safety:
 * - This is constructed and used by CNetServerWorker in the network server thread.
 */
class CNetServerSession : public CFsm
{
	NONCOPYABLE(CNetServerSession);

public:
	CNetServerSession(CNetServerWorker& server, ENetPeer* peer);

	CNetServerWorker& GetServer() { return m_Server; }

	const CStr& GetGUID() const { return m_GUID; }
	void SetGUID(const CStr& guid) { m_GUID = guid; }

	const CStrW& GetUserName() const { return m_UserName; }
	void SetUserName(const CStrW& name) { m_UserName = name; }

	u32 GetHostID() const { return m_HostID; }
	void SetHostID(u32 id) { m_HostID = id; }

	/**
	 * Sends a disconnection notification to the client,
	 * and sends a NMT_CONNECTION_LOST message to the session FSM.
	 * The server will receive a disconnection notification after a while.
	 * The server will not receive any further messages sent via this session.
	 */
	void Disconnect(u32 reason);

	/**
	 * Sends an unreliable disconnection notification to the client.
	 * The server will not receive any disconnection notification.
	 * The server will not receive any further messages sent via this session.
	 */
	void DisconnectNow(u32 reason);

	/**
	 * Send a message to the client.
	 */
	bool SendMessage(const CNetMessage* message);

private:
	CNetServerWorker& m_Server;

	ENetPeer* m_Peer;

	CStr m_GUID;
	CStrW m_UserName;
	u32 m_HostID;
};

#endif	// NETSESSION_H
