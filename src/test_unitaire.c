/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_unitaire.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bribot <bribot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 16:40:44 by bribot            #+#    #+#             */
/*   Updated: 2026/09/18 17:08:33 by bribot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	test_make_dot_product(void)
{
	float	first[3] = {1.5, 3, 5};
	float	second[3] = {2, 4.5, 6};

	if (make_dot_product(first, second) == 46.5)
		printf("\033[32;01m{1.5, 3, 5}.{2, 4.5, 6} GOOD\033[00m\n");
	else
		printf("\033[31;01m{1.5, 3, 5}.{2, 4.5, 6} WRONG\033[00m\n");
	float first1[3] = {1.5, 3, 5};
	float second1[3] = {-2, -4.5, -6};
	if (make_dot_product(first1, second1) == -46.5)
		printf("\033[32;01m{1.5, 3, 5}.{-2, -4.5, -6} GOOD\033[00m\n");
	else
		printf("\033[31;01m{1.5, 3, 5}.{-2, -4.5, -6} WRONG\033[00m\n");
	float	first2[3] = {0, 0, 0};
	float	second2[3] = {-2, -4.5, -6};
	if (make_dot_product(first2, second2) == 0)
		printf("\033[32;01m TEST03 GOOD\033[00m\n");
	else
		printf("\033[31;01m TEST03 WRONG\033[00m\n");
	float	first3[3] = {0, 5, 0};
	float	second3[3] = {-2, 0, -6};
	if (make_dot_product(first3, second3) == 0)
		printf("\033[32;01m TEST04 GOOD\033[00m\n");
	else
		printf("\033[31;01m TEST04 WRONG\033[00m\n");
	float	first4[3] = {-2, -2, -2};
	float	second4[3] = {-2, -2, -2};
	if (make_dot_product(first4, second4) == 12)
		printf("\033[32;01m TEST05 GOOD\033[00m\n");
	else
		printf("\033[31;01m TEST05 WRONG\033[00m\n");
}

int	test_unitaire(void)
{
	test_make_dot_product();
	return (0);
}
