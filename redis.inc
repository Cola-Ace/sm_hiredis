#if defined _redis_included
  #endinput
#endif
#define _redis_included

/*
 * Connect to redis.
 *
 * @param ip Redis ip.
 * @param port Redis port.
 *
 * @return Return true if connect success.
 */
native bool Redis_Connect(const char[] ip, int port);

/*
 * Get key.
 *
 * @param key Key.
 * @param str String to save result.
 * @param size String size.
 */
native int Redis_Get(const char[] key, char[] str, int size);

/*
 * Set key.
 *
 * @param key Key.
 * @param str Value to set.
 */
native int Redis_Set(const char[] key, const char[] str);

/*
 * Delete key.
 *
 * @param key key.
 */
native int Redis_Del(const char[] key);

public Extension __ext_redis =
{
	name = "redis",
	file = "redis.ext",
#if defined AUTOLOAD_EXTENSIONS
	autoload = 1,
#else
	autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
}

#if !defined REQUIRE_EXTENSIONS
public __ext_redis_SetNTVOptional(){
	MarkNativeAsOptional("Redis_Connect");
	MarkNativeAsOptional("Redis_Set");
	MarkNativeAsOptional("Redis_Get");
	MarkNativeAsOptional("Redis_Del")
}