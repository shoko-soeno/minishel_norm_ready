/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamatsuu <tamatsuu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 21:49:29 by tamatsuu          #+#    #+#             */
/*   Updated: 2025/02/08 19:42:00 by tamatsuu         ###   ########.fr       */
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
void	set_exec_handler(int signum);

bool	is_interactive_mode(void);
int		sigint_event_hook(void);
int		heredoc_sigint_event_hook(void);

#endif