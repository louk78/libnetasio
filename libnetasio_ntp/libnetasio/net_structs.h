#ifndef __LIBNETASIO_NET_STRUCTS_H__
#define __LIBNETASIO_NET_STRUCTS_H__
#include "connection.h"
#include "net_protocol.h"

namespace libnetasio
{
	class CConnection;

	typedef struct s_conn_context
	{
		SMSG msg;
		CConnection* pConn;
		unsigned char cbCloseReasion;
	}SCONNCONTEXT, *PSCONNCONTEXT;
}







#endif
