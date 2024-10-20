/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 13:37:12 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/20 14:48:42 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int g_signal_received = 0;

void	handle_signal(int __attribute__((unused)) signum)
{
	// if (signalReceived == 0)
	std::cerr << "signal received\n";
	g_signal_received = 1;
}

int	main(void)
{
	Logger& logger = Logger::getInstance();
	logger.LOG_INFO("TEST");
	Server	server("server_config.txt");

	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
	try
	{
		server.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}