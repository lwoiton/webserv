#pragma once
#ifndef TCPLISTENER_HPP
# define TCPLISTENER_HPP

class	TcpListener
{
	private:
		int		_sockfd;
		EventLoop&	_loop;
	public:
				TcpListener(EventLoop& loop);
				~TcpListener(void);
		void		start();
};

#endif
