#include <philosopher.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

static int	ft_itoa_append(char *buf, int n, int pos)
{
	char	tmp[32];
	int		len;

	len = 0;
	while (n > 0)
	{
		tmp[len++] = '0' + (n % 10);
		n /= 10;
	}
	while (len--)
		buf[pos++] = tmp[len];
	return (pos);
}

void	join(char *dest, const char *prefix, int n)
{
	int	i;

	i = 0;
	while (prefix[i])
	{
		dest[i] = prefix[i];
		i++;
	}
	i = ft_itoa_append(dest, n, i);
	dest[i] = '\0';
}

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

void	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
	{
		usleep(500);
	}
}

long	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long	ft_atoi(const char *str)
{
	long	num;
	int		sign;
	int		i;

	i = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		++i;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	num = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num *= 10;
		num = num + (str[i] - '0');
		++i;
		if (num > INT_MAX)
			break ;
	}
	return (num * sign);
}
