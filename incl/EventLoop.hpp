#pragma once
#ifndef EVENTLOOP_HPP
# define EVENTLOOP_HPP

class EventLoop
{
	public:
		EventLoop();
		~EventLoop();
		void	addFd(int fd, uint32_t events, 
		void	removeFd(int fd, uint32_t events,
		void	addClientSocket(
	private:
		int		_epollFd;
		void	setNonBlocking(ind fd);
}

#endif
