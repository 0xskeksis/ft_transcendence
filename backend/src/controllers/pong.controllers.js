// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   pong.controllers.js                                :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2025/11/10 20:07:25 by ellanglo          #+#    #+#             //
//   Updated: 2025/11/10 20:17:28 by ellanglo         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
export async function sendData(request, reply)
{
	const score = 117;
	const lpos = 0.5;
	const rpos = 0.5;
	const bposx = 0.4;
	const bposy = 0.6;

	return reply.send({score, lpos, rpos, bposx, bposy});
}
