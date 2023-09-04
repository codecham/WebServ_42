/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dduvivie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 12:11:28 by dduvivie          #+#    #+#             */
/*   Updated: 2023/09/04 12:11:30 by dduvivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "methods.hpp"

void	Exec(Server& server, Request& request, Response& response)
{
	std::string	method = request.getMethod();

	if (method == "GET")
		handleGet(server, request, response);
	else if (method == "POST")
		handlePost(server, request, response);
	else if (method == "DELETE")
		handleDelete(server, request, response);
	else
		response.createResponse("405", server);
}
