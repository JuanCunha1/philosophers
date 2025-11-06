#include <philosopher_bonus.h>
#include <sys/time.h>


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
		usleep(200);
	}
}

long	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}