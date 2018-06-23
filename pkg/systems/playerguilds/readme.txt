Here is my playerguilds package and packethook based on MontuZ distro version. It's in use at the moment on my shard and will require only minor mods to make it work with the distro.

It's here as-is, lifted directly from my shard. MontuZ headers and comments are still mostly intact giving him the credit.

A few notes to make it distro compliant:

* I don't use reports.inc so replace my logging lines with ones from the reports.inc distro utility. Either that or use the logging utility from the 096 distro like I do.
* My shard uses a separate expanded titling system which does the actual setting of title_guild on the player. See the GuildTitle function  in this package for some notes on how I call that and the simple mod to let GuildTitle set the title directly.
* I have replaced the word "Enemy" with "Rival", just my choice as I don't have PvP on my shard.
* A lot of the functions have been broken out into an inc file so that I can use them in guildCreate.src where a non guild member can view existing guilds and their charters.
* There is one reference to client.inc, but that's only to set a colour.