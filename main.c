#include <sys/_types/_ssize_t.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <termios.h>

struct player
{
	char	name[100];
	int		pv;
	int		x;
	int		y;
} player_1 = {.pv = 100, .x = 20, .y = 10};

struct termios	g_original;
struct termios	g_modified;

int	init_terminal(void)
{
	if (tcgetattr(0, &g_original) == -1)
		return (1) ;
	g_modified = g_original;
	g_modified.c_lflag &= ~(ICANON | ECHO);
	return (0);
}

void	set_canonical(int mode)
{
	if (!mode)
		tcsetattr(0, TCSANOW, &g_modified);
	else
		tcsetattr(0, TCSANOW, &g_original);
	return ;
}

int	ft_strcmp(char *buf, char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == buf[i])
		i++;
	return (str[i] - buf[i]);
}

int	ft_strcpy(char *buf, char *str)
{
	int	i;

	i = 0;
	while (buf[i])
	{
		str[i] = buf[i];
		i++;
	}
	str[i] = '\0';
	return (0);
}

int	sleep_ms(int delay_ms)
{
	struct timespec	delay;

	delay.tv_sec = delay_ms / 1000;
	delay.tv_nsec = (delay_ms % 1000) * 1000000;
	nanosleep(&delay, NULL);
	return (0);
}

int	type(char *str, int delay_ms)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		sleep_ms(delay_ms);
		i++;
	}
	return (0);
}

void	initializing(void)
{
	write(1, "\x1b[2J\x1b[H", 7);
	type("\ninitializing...\n", 80);
	sleep_ms(80);
	write(1, "\x1b[2J\x1b[H", 7);
	type("\nwait a second...\n", 80);
	sleep_ms(80);
	write(1, "\x1b[2J\x1b[H", 7);
	type("\nglitch found!\n", 80);
	sleep_ms(200);
}

int	main_menu(void)
{
	char			buffer[100];
	ssize_t			n;

	write(1, "\x1b[2J\x1b[H", 7);
	type("\nWelcome on The Glitcher!\n", 25);
	type("1. New game\n", 25);
	type("2. Quit\n\n", 25);
	n = 0;
	n = read(0, buffer, sizeof(buffer) - 1);
	buffer[n - 1] = '\0';
	if (!ft_strcmp(buffer, "New game"))
	{
		write(1, "\x1b[2J\x1b[H", 7);
		type("\nWelcome adventurer!\n", 25);
		type("What's your name ?\n\n", 25);
		n = read(0, buffer, sizeof(buffer) - 1);
		buffer[n - 1] = '\0';
		ft_strcpy(buffer, player_1.name);
		write(1, "\x1b[2J\x1b[H", 7);
		type("\nYour adventure is about to begin ", 80);
		type(player_1.name, 80);
		type(" . . .\n", 500);
		sleep_ms(1000);
	}
	return (1);
}

int	render_map(int width, int height)
{
	int	x;
	int	y;

	y = 0;
	while (y <= height)
	{
		x = 0;
		while (x <= width)
		{
			if (player_1.x == x && player_1.y == y)
				write(1, " @", 2);
			else if (x == 0 || y == 0 || x == width || y == height)
				write(1, " ^", 2);
			else
				write(1, " .", 2);
			x++;
		}
		write(1, "\n", 1);
		y++;
	}
	return (0);
}

int	game_loop(void)
{
	int		running;
	ssize_t	n;
	char	buffer[100];

	n = 0;
	running = 1;
	set_canonical(0);
	while (running)
	{
		write(1, "\x1b[2J\x1b[H", 7);
		render_map(30, 15);
		n = read(0, buffer, sizeof(buffer) - 1);
		if (n == 1 && buffer[0] == 'w')
			player_1.y--;
		if (n == 1 && buffer[0] == 'a')
			player_1.x--;
		if (n == 1 && buffer[0] == 's')
			player_1.y++;
		if (n == 1 && buffer[0] == 'd')
			player_1.x++;
	}
	return (0);
}

int	main(void)
{
	init_terminal();
	initializing();
	main_menu();
	game_loop();
	return (0);
}
