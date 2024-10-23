/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:07:20 by lwoiton           #+#    #+#             */
/*   Updated: 2024/10/22 14:58:33 by lwoiton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef UTILS_HPP
# define UTILS_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

std::string sizeToString(size_t value);
std::string intToString(int number);
std::string readFile(const std::string& filename);
#endif // UTILS_HPP