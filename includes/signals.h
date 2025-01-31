/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoeno <ssoeno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 21:49:29 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/01/31 11:18:09 by ssoeno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H
# define _POSIX_C_SOURCE 200809L
# include <signal.h>
# include <stdbool.h>
# include <sys/wait.h>

// signal
typedef struct sigaction		t_sig;
extern volatile sig_atomic_t	g_sig;

void	set_idle_sig_handlers(void);
void	set_parent_sig_handlers(void);
void	set_child_sig_handlers(void);
void	check_core_dump(int status);

bool	is_interactive_mode(void);
int		sigint_event_hook(void);
int		heredoc_sigint_event_hook(void);

#endif