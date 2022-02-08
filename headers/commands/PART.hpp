/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wperu <wperu@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 13:19:05 by wperu             #+#    #+#             */
/*   Updated: 2022/02/08 17:24:09 by wperu            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "commands.hpp"

class part : public commands
{

	public:
		part();
		~part();
		void	execute(std::string buf, Client *cli, std::vector<Channel> *chan, std::vector<pollfd> &fds);
};