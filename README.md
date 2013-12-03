# Pidgin random hashtag plugin

Whenever you send an instant message that ends with #-character, a "hashtag"
is appended to the message. The hashtag is chosen randomly from a config file
in the user's home directory (`~/.random_hashtags`).

## Compiling and installing the plugin

Install pre-requisite `pidgin-devel` and `libpurple-devel` packages. On Ubuntu:

	sudo apt-get build-dep pidgin
	sudo apt-get install pidgin-dev libpurple-dev

Compiling the plugin

	make

Installing the plugin to `~/.purple/plugins/`

	make install

Add some hashtags to config file

	cat > ~/.random_hashtags << EOF
	hello
	another
	third
	EOF

Enable plugin *Random hashtag* from Pidgin plugins list.
