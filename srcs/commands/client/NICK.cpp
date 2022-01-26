/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wperu <wperu@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 11:07:43 by wperu             #+#    #+#             */
/*   Updated: 2022/01/26 17:37:25 by wperu            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/commands/NICK.hpp"

nick::nick()
{
    
}

nick::~nick()
{
    
}

bool nick:: _checknick(std::string user, std::vector<Client *> *clients)
{
    for (std::vector<Client *>::iterator it = clients->begin(); it != clients->end(); it++)
	{
		Client *c = *it;
		if (c->get_nickname() == user)
			return false;
	}
	return true;
}

bool nick::_validnick(std::string nick)
{
    size_t i = 0;
	while (i < nick.length())
	{
		if (nick[i] <= 32 || nick[i] > 126 || nick[i] == ':' || nick[i] == '#' || nick[i] == '!' || nick[i] == '@')
			return false;
		i++;
	}
    return true;
}

bool nick::_is_informed(Client *dest, std::vector<Client *> informed)
{
	for(std::vector<Client *>::iterator it = informed.begin(); it != informed.end(); it++)
	{
		Client *search = *it;
		if(search->get_nickname() == dest->get_nickname())
			return true;
	}
	return false;
}


void nick:: _announcement_new_nick(std::string message, Client *cli,std::vector<Channel *> *chan)
{
    std::vector<Client *> informe;
	
	for(std::vector<Channel *>::iterator it = chan->begin(); it != chan->end(); it++)
	{
		Channel *c = *it;
		if(c->is_members(cli->get_nickname()))
		{
			std::vector<Client *> clis = c->get_members();
			for(std::vector<Client *>::iterator its = clis.begin(); its != clis.end();its++)
			{
				Client *dest = *its;
				if(!_is_informed(dest,informe))
				send(dest->get_sock_fd(),message.c_str(),message.length(),0);
				informe.push_back(dest);
			}
		}
	}
}

void nick::excute(std::string buf, Client *cli, std::vector<Channel *> *chan, std::vector<Client *> *Clients)
{
    std::string message;
    if(buf.find(' ') == buf.npos)
    {
        message = ":server " + std::string(ERR_NONICKNAMEGIVEN) + ":No nickname given\r\n";
        send(cli->get_sock_fd(), message.c_str(), message.length(), 0);
		return ;
    }

    int begin = buf.find(' ') + 1;
    int len = buf.length() - begin + 1;
    if(buf.find('/r') != buf.npos)
        len = buf.find('/r') - begin;
    std::string nick = buf.substr(begin,len);
    if(nick[0] == ':')
        nick = nick.substr(1, nick.length() - 1);
    if(nick == cli->get_nickname())
        return;
    if (!_validnick(nick))
	{
		message = ":server " + std::string(ERR_ERRONEUSNICKNAME) + " nick: Erroneus nickname\r\n";
		send(cli->get_sock_fd(), message.c_str(), message.length(), 0);
		return ;
	}
	if (!_checknick(nick, Clients))
	{
		message = ":server " + std::string(ERR_NICKNAMEINUSE) + " nick: Nickname is already in use\r\n";
		send(cli->get_sock_fd(), message.c_str(), message.length(), 0);
		return ;
	}

    message = ":" + cli->get_nickname() + " NICK " + nick +"\r\n";
	cli->set_nickname(nick);
    
    _announcement_new_nick(message, cli, chan);
}      
