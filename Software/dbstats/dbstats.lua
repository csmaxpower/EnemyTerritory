--[[
Sol ( DB Stats )
( www.polish-frags.pl )
Version 0.2
]]-- 

modname="dbstats"
version="0.2"

require("lualibs/dblib")

DB:Connect("moeswebserver_legacystatstest", "moeswebserver", "oK8CtDqHn6", "moestavern.site.nfoservers.com", 3306)

mode 			= 1		-- recomended for large server ( > 15-20 clients ) this is a slower way but without warrrrrps
SyncTime 		= 15	-- seconds between one and other client ( works if mode eq 1 )
SyncTimeMode	= 2		-- seconds between loops ( works if mode eq 0 )

cl = 0
Kills 	= { }
Deaths	= { }
Client 	= { }
Maxchars = 10
oldtime = os.time()

function et_InitGame( levelTime, randomSeed, restart )

	begin_time		= os.time()
	ispast			= false
	sv_maxclients 	= tonumber(et.trap_Cvar_Get("sv_maxclients"))
	modname			= et.trap_Cvar_Get("fs_game")

	cl = tonumber(sv_maxclients - 1)
	et.RegisterModname( "K/D DBStats v".. Version .." by ".. Author )
	for i = 0, sv_maxclients - 1 do
		Kills[i] 	= 0
		Deaths[i]	= 0
	end
end

function et_ClientBegin( num )
	if string.len( GetGuid( num ) ) == 32 then
		DB:CreateClient( num )
	end
	Kills[num] 	= 0
	Deaths[num]	= 0
end

function et_ClientDisconnect( num )
	check = DB:GetClient(GetGuid( num ))
	if check ~= nil then
		DB:UpdateClient( num, Kills[num],Deaths[num] )
		Kills[num] 	= 0
		Deaths[num]	= 0
	end
end

function et_ConsoleCommand( cmd )
	if cmd == "!sqlcreate" then
		et.G_Print("Creating Database!....\n")
		if DB:CreateTable() == true then
			et.G_Print("Done!\n")
		else
			et.G_Print("Something wrong!\n")
		end
	end
end

function et_ClientCommand(clientnum, command)
	if command ~= "say" and et.gentity_get(clientnum, "sess.muted") == true and et.ClientIsFlooding(clientnum) == 1 then
		return
	end

	local argv1 = string.lower( et.trap_Argv( 1 ) )

	if argv1 == "!ratio" or argv1 == "!stats" then
		local guid = GetGuid( clientnum )
		local info = DB:GetClient( guid )
		local rank = DB:GetRank( guid, "kills" )
		if info ~= nil then
			if rank == 1 then
				et.trap_SendConsoleCommand(et.EXEC_APPEND, string.format("chat %s's ^3stats: (K:%d/D:%d) ^2Rank ^3#%d\n",et.gentity_get(clientnum, "pers.netname"), info["kills"], info["deaths"], rank))
			elseif rank == 2 then
				et.trap_SendConsoleCommand(et.EXEC_APPEND, string.format("chat %s's ^3stats: (K:%d/D:%d) ^2Rank ^9#%d\n",et.gentity_get(clientnum, "pers.netname"), info["kills"], info["deaths"], rank))
			elseif rank == 3 then
				et.trap_SendConsoleCommand(et.EXEC_APPEND, string.format("chat %s's ^3stats: (K:%d/D:%d) ^2Rank ^x#%d\n",et.gentity_get(clientnum, "pers.netname"), info["kills"], info["deaths"], rank))
			else
				et.trap_SendConsoleCommand(et.EXEC_APPEND, string.format("chat %s's ^3stats: (K:%d/D:%d) ^2Rank #%d\n",et.gentity_get(clientnum, "pers.netname"), info["kills"], info["deaths"], rank))
			end
		end
	elseif argv1 == "!listratio" then
		et.trap_SendServerCommand(clientnum, string.format('print "^d--------------------------------------------------\n"'))
		et.trap_SendServerCommand(clientnum, string.format('print "^d\t^1        Player       ^d\t^1         Ratio            ^d\t^1\n"'))
		et.trap_SendServerCommand(clientnum, string.format('print "^d--------------------------------------------------\n"'))
		for i = 0, (tonumber(et.trap_Cvar_Get( "sv_maxclients" )) - 1) do
			Client = DB:GetClient( GetGuid( i ) )
			if Client ~= nil then
				et.trap_SendServerCommand(clientnum, string.format('print "^d\t^1  ^7%'.. Maxchars ..'s         ^d\t^1      ^3%'.. Maxchars ..'s          ^d\t^1\n"', string.sub(uncolor(tostring(et.gentity_get(i, "pers.netname"))),0,Maxchars), Float2String(Client["kills"],Client["deaths"])))
			end
		end
		et.trap_SendServerCommand(clientnum, string.format('print "^d--------------------------------------------------\n"'))
	elseif argv1 == "!resetstats" then
		DB:ResetRatio(clientnum)
		Kills[clientnum] 	= 0
		Deaths[clientnum]	= 0
		et.trap_SendServerCommand(clientnum, string.format('cpm "^3You Have Reset Your Ratio!"'))
	elseif argv1 == "!top" then -- top kills and top deaths only!
		DB:GetTop(et.trap_Argv(2))
	end
end

function et_RunFrame( levelTime )
	if mode == 1 then
		if os.time() - oldtime > SyncTimeMode and tonumber(et.trap_Cvar_Get("gamestate")) == 0 then
			if cl < 0 then
				cl = tonumber(sv_maxclients - 1)
			else
				if et.gentity_get(cl, "sess.sessionTeam") ~= 0 and et.gentity_get(cl, "ps.ping") ~= 0 then
					local guid = GetGuid( cl )
					check = DB:GetClient( guid )
					if check ~= nil then
						DB:UpdateClient( cl, Kills[cl],Deaths[cl] )
						Kills[cl] 	= et.gentity_get(cl,"sess.kills")
						Deaths[cl]	= et.gentity_get(cl,"sess.deaths")
					else
						return
					end
				end
				cl = cl - 1
			end
			oldtime = os.time()
		end
	elseif mode == 0 then
		if os.time() - oldtime > SyncTime and tonumber(et.trap_Cvar_Get("gamestate")) == 0 then
			for i = 0, sv_maxclients - 1 do
				if et.gentity_get(i, "sess.sessionTeam") ~= 0 then
					local guid = GetGuid( i )
					check = DB:GetClient( guid )
					if check ~= nil then
						DB:UpdateClient( i, Kills[i],Deaths[i] )
						Kills[i] 	= et.gentity_get(i,"sess.kills")
						Deaths[i]	= et.gentity_get(i,"sess.deaths")
					else
						return
					end
				end
			end
			oldtime = os.time()
		end
	end
end

function et.GetLevel( num )
   return et.G_shrubbot_level(num)
end

function Float2String( k,d )
	if tonumber( k ) <= 0 or ( k/d ) <= 0 then
		return "0"
	else
		return string.format("%0.3f",(k/d))
	end
end

function uncolor( name )
	return et.Q_CleanStr( name )
end

function IDFromName( name )
	return et.G_ClientNumberFromString( name )
end

function GetID( num )
	return string.sub(GetGuid( num ), 25,32)
end

function GetGuid( num )
	if modname == "silent" then
		return et.gentity_get( num, "sess.guid" )
	else
		return (et.Info_ValueForKey(et.trap_GetUserinfo( num ), "cl_guid"))
	end
end

function GetFromUserinfo( num, str )
	return et.Info_ValueForKey(et.trap_GetUserinfo( num ), str)
end
