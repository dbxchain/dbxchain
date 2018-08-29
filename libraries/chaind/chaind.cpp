
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/json.h>

#include "net.h"
#include "chaind.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std ;

chaind::chaind g_chaind;

namespace chaind {

	void chaind::set_url( const string& s_ip, const uint32_t u_port )
	{
		this->ms_ip = s_ip;
		this->mu_port = u_port;
	}

	bool add_blacklist_account(const string& s_json)
	{
		std::cout << "chaind::net::connect server(" << ms_ip << ":" << mu_port << ") error" << std::endl;

		int i_socket = -1;
		if( !chaind::net::connect( i_socket, ms_ip, mu_port ) )
		{
			std::cerr << "chaind::net::connect server(" << ms_ip << ":" << mu_port << ") error" << std::endl;
			if ( i_socket != -1 )
				chaind::net::close(i_socket);
			return false;
		}

		if( chaind::json::write( i_socket, s_json ) < 0 )
		{
			std::cerr << "chaind::json::write server(" << i_socket << ") error" << std::endl ;
			chaind::net::close(i_socket);
			return false ;
		}

		std::vector<char> v_read ;
		int ret = chaind::json::read( i_socket, v_read, 0 ) ;
		if ( ret != chaind::RNET_SMOOTH )
		{
			std::cerr << "chaind::json::read() failure" << std::endl ;
			chaind::net::close(i_socket);
			return false ;
		}

		string s_read(v_read.begin(), v_read.end());
		std::cout << "s_read = " << std::endl << s_read << std::endl;

		Json::Value parse_root ;
		Json::Reader reader ;

		if ( !reader.parse( s_read, parse_root ) )
		{
			std::cerr << "rjson::parse json error" << std::endl ;
			chaind::net::close(i_socket);
			return false ;
		}

		if( parse_root[0].asInt() != 1 )
		{
			std::cerr << "blacklistd service record error" << std::endl ;
			chaind::net::close(i_socket);
			return false ;
		}

		chaind::net::close(i_socket);

		return true ;
	}

	bool chaind::set_asset_fee( const string& s_json, Json::UInt64& fee )
	{
		std::cout << "chaind::net::connect server(" << ms_ip << ":" << mu_port << ") error" << std::endl;

		int i_socket = -1;
		if( !chaind::net::connect( i_socket, ms_ip, mu_port ) )
		{
			std::cerr << "chaind::net::connect server(" << ms_ip << ":" << mu_port << ") error" << std::endl;
			if ( i_socket != -1 )
				chaind::net::close(i_socket);
			return false;
		}

		if( chaind::json::write( i_socket, s_json ) < 0 )
		{
			std::cerr << "chaind::json::write server(" << i_socket << ") error" << std::endl ;
			chaind::net::close(i_socket);
			return false ;
		}

		std::vector<char> v_read ;
		int ret = chaind::json::read( i_socket, v_read, 0 ) ;
		if ( ret != chaind::RNET_SMOOTH )
		{
			std::cerr << "chaind::json::read() failure" << std::endl ;
			chaind::net::close(i_socket);
			return false ;
		}

		string s_read(v_read.begin(), v_read.end());
		std::cout << "s_read = " << std::endl << s_read << std::endl;

		Json::Value parse_root ;
		Json::Reader reader ;

		if ( !reader.parse( s_read, parse_root ) )
		{
			std::cerr << "rjson::parse json error" << std::endl ;
			chaind::net::close(i_socket);
			return false ;
		}

		if( parse_root[0].asInt() != 1 )
		{
			std::cerr << "blacklistd service record error" << std::endl ;
			chaind::net::close(i_socket);
			return false ;
		}

		chaind::net::close(i_socket);

		fee = parse_root[1].asInt64();

		return true ;
	}

	bool chaind::check_in_blacklist( const string& s_json, const bool b_assert )
	{
		std::cout << "chaind::net::connect server(" << ms_ip << ":" << mu_port << ") error" << std::endl;

		int i_socket = -1;
		if( !chaind::net::connect( i_socket, ms_ip, mu_port ) )
		{
			std::cerr << "chaind::net::connect server(" << ms_ip << ":" << mu_port << ") error" << std::endl;
			if ( i_socket != -1 )
				chaind::net::close(i_socket);
			FC_ASSERT( false, "connect blacklistd service error" );
		}

		if( chaind::json::write( i_socket, s_json ) < 0 )
		{
			std::cerr << "chaind::json::write server(" << i_socket << ") error" << std::endl ;
			chaind::net::close(i_socket);
			FC_ASSERT( false, "write blacklistd service error" );
		}

		std::vector<char> v_read ;
		int ret = chaind::json::read( i_socket, v_read, 0 ) ;
		if ( ret != chaind::RNET_SMOOTH )
		{
			std::cerr << "chaind::json::read() failure" << std::endl ;
			chaind::net::close(i_socket);
			FC_ASSERT( false, "read blacklistd service error" );
		}

		std::string s_read(v_read.begin(), v_read.end());
		std::cout << "s_read = " << std::endl << s_read << std::endl;

		Json::Value parse_root ;
		Json::Reader reader ;

		if ( !reader.parse( s_read, parse_root ) )
		{
			std::cerr << "rjson::parse json error" << std::endl ;
			chaind::net::close(i_socket);
			FC_ASSERT( false, "rjson::parse json error" );
		}

		if( parse_root[0].asInt() != 1 )
		{
			std::cerr << "blacklistd service error : " << parse_root[1].asString() << std::endl ;
			chaind::net::close(i_socket);
			FC_ASSERT( false, "blacklistd service error : ${error}", ("error", parse_root[1].asString()) );
		}

		chaind::net::close(i_socket);
	}
}
