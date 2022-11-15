--[[
Sol ( DB Lib )
( www.polish-frags.pl )
]]--


Table = "Clients"

DB = {
	['Connect'] = function ( c, DBName, DBUser, DBPass, DBAddr, Port )
		require "luasql.mysql"
		c.env = assert( luasql.mysql() )
		c.con = assert(c.env:connect(DBName, DBUser, DBPass, DBAddr, Port))
	end,

	['Disconnect'] = function ( c )
		c.con:close()
		c.env:close()
	end,

	['CreateClient'] = function ( c, num )
		local userinfo = et.trap_GetUserinfo(num)
		local cname = string.gsub(et.gentity_get(num,"pers.netname"),"'", "")
		local dbmsg = "INSERT INTO "..Table.." (guid, ip, name, cname, kills, deaths, admin) VALUES ('"
		..GetGuid( num ).."', '"
		..ComIP(num).."', '"
		..string.gsub(et.Q_CleanStr( et.gentity_get(num,"pers.netname") ),"'", "").."', '"
		..cname.."', '"
		..et.gentity_get(num,"sess.kills").."', '"
		..et.gentity_get(num,"sess.deaths").."', '"
		..et.G_shrubbot_level(num).."')"
		c.cur = assert (c.con:execute(dbmsg))
	end,

	['GetClient'] = function ( c, guid )
		c.cur = assert (c.con:execute("SELECT * FROM "..Table.." WHERE guid='".. guid .."'"))
		c.row = c.cur:fetch ({}, "a")
		c.cur:close()
		return c.row
	end,

	['GetRank'] = function ( c, guid, by )
		c.cur = assert ( c.con:execute("SELECT guid FROM "..Table.." ORDER BY "..by.." DESC"))
		c.row = c.cur:fetch({},"a")
		local rank = 1
		while c.row do
			if c.row.guid == guid then
				break
			end
			c.row = c.cur:fetch(c.row, "a")
			rank = rank + 1
		end
		return rank
	end,

	['GetTop'] = function ( c, by )
		c.cur = assert (c.con:execute("SELECT * FROM "..Table.." ORDER BY "..by.." DESC LIMIT 3"))
		c.row = c.cur:fetch ({}, "a")
		et.trap_SendConsoleCommand(et.EXEC_APPEND, string.format("chat ^2Top3 %s.\n",by))
		if by == "kills" then
			while c.row do
				et.trap_SendConsoleCommand(et.EXEC_APPEND, string.format("chat %s ^3(%d)\n",c.row.cname,c.row.kills))
				c.row = c.cur:fetch (c.row, "a")
			end
		elseif by == "deaths" then
			while c.row do
				et.trap_SendConsoleCommand(et.EXEC_APPEND, string.format("chat %s ^3(%d)\n",c.row.cname,c.row.deaths))
				c.row = c.cur:fetch (c.row, "a")
			end
		end
	end,

	['UpdateClient'] = function ( c, num, k,d )
		local userinfo 	= et.trap_GetUserinfo(num)
		local cname 	= string.gsub(et.gentity_get(num,"pers.netname"),"'", "")
		local guid		= GetGuid( num )
		local cl		= DB:GetClient( guid )
		local dbmsg 	= "UPDATE "..Table.." SET ip='"..ComIP(num).."', \
			name='"..string.gsub(et.Q_CleanStr( et.gentity_get(num,"pers.netname") ),"'", "").."', \
			cname='"..cname.."', \
			kills='"..(tonumber(cl["kills"])+(et.gentity_get(num,"sess.kills")-k)).."', \
			deaths='"..(tonumber(cl["deaths"])+(et.gentity_get(num,"sess.deaths")-d)).."', \
			admin='"..et.G_shrubbot_level(num).."' \
			WHERE guid='"..guid.."'"
		c.cur = assert (c.con:execute(dbmsg))
	end,

	['ResetRatio'] = function ( c, num )
		local userinfo 	= et.trap_GetUserinfo(num)
		local cl		= DB:GetClient(GetGuid( num ))
		local dbmsg 	= "UPDATE "..Table.." SET kills='0', \
			deaths='0' WHERE guid='"..GetGuid( num ).."'"
		c.cur = assert (c.con:execute(dbmsg))
	end,

	['CreateTable'] = function ( c )
		local dbmsg = "CREATE TABLE IF NOT EXISTS "..Table.." ( 	\
				guid	TEXT	DEFAULT '',							\
				ip		TEXT	DEFAULT '',									\
				name	TEXT	DEFAULT '',									\
				cname	TEXT	DEFAULT '',									\
				kills	INTEGER	DEFAULT 0,									\
				deaths	INTEGER	DEFAULT 0,									\
				admin	INTEGER	DEFAULT 0)"
		c.cur = assert (c.con:execute(dbmsg))
		return true
	end
}

function CleanGuid( guid )
	if string.len( guid ) == 32 then
		return guid
	else
		return string.sub(guid, 1, 32)
	end
end

function GetGuid( num )
	if et.trap_Cvar_Get("fs_game") == "silent" then
		return et.gentity_get( num, "sess.guid" )
	else
		return (et.Info_ValueForKey(et.trap_GetUserinfo( num ), "cl_guid"))
	end
end

function ComIP(num)
	local ips = et.Info_ValueForKey(et.trap_GetUserinfo(num), "ip")
	if ips ~= "localhost" then
		s,e,ip = string.find(ips,"(%d+%.%d+%.%d+%.%d+)")
		return ip
	else
		return "localhost"
	end
end
