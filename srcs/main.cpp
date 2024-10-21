/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 13:37:12 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/21 12:20:28 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

volatile sig_atomic_t g_signal_received = 0;

void signalHandler(int signum)
{
	
    g_signal_received = 1;
	std::cout << std::endl;
	if (signum == SIGINT)
		LOG_INFO("SIGINT received");
	else if (signum == SIGTERM)
		LOG_INFO("SIGTERM received");
	else if (signum == SIGQUIT)
		LOG_INFO("SIGQUIT received");
}

int	main(void)
{
	Server	server("server_config.txt");

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGQUIT, signalHandler);
	try
	{
		server.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}