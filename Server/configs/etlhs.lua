local Version = "1.0"
local Author  = "^eMoe's Tavern"
local Description = "^7Hide^1&^7Seek for ETLegacy"
local Homepage = "https://discord.moestavern.pub"


et.MAX_WEAPONS = 56
samplerate = 200


local weapons = {
	false,  --et.WP_NONE	0	No weapon
	false,  --et.WP_KNIFE	1	Axis Dagger Knife
	false,  --et.WP_LUGER	2	Luger
	false,  --et.WP_MP40	3	MP40
	false,  --et.WP_GRENADE_LAUNCHER	4	Axis Hand Grenade
	false,  --et.WP_PANZERFAUST	5	Panzerfaust
	false,  --et.WP_FLAMETHROWER	6	Flamethrower
	true,   --et.WP_COLT	7	Colt 1911
	false,  --et.WP_THOMPSON	8	Thompson
	false,  --et.WP_GRENADE_PINEAPPLE	9	Allies Hand Grenade
	false,  --et.WP_STEN	10	Sten
	true,   --et.WP_MEDIC_SYRINGE	11	Syringe
	true,   --et.WP_AMMO	12	Ammo pack
	false,  --et.WP_ARTY	13	Artillery
	false,  --et.WP_SILENCER	14	Silenced Luger
	false,  --et.WP_DYNAMITE	15	Dynamite
	false,  --et.WP_SMOKETRAIL	16	Artillery Initial smoke
	false,  --et.WP_MAPMORTAR	17	Fixed Mortars
	false,  --et.VERYBIGEXPLOSION	18	Airstrike Explosion effect
	true,   --et.WP_MEDKIT	19	Medic pack
	false,  --et.WP_BINOCULARS	20	Binoculars
	false,  --et.WP_PLIERS	21	Pliers
	false,  --et.WP_SMOKE_MARKER	22	Airstrike Marker
	false,  --et.WP_KAR98	23	Kar98 (Axis Rifle)
	false,  --et.WP_CARBINE	24	M1 Garand (Allies Rifle)
	false,  --et.WP_GARAND	25	Scoped M1 Garand (Allies Sniper Rifle)
	false,  --et.WP_LANDMINE	26	Landmine
	false,  --et.WP_SATCHEL	27	Satchel
	false,  --et.WP_SATCHEL_DET	28	Satchel Detonator
	false,  --et.WP_SMOKE_BOMB	29	Smoke Grenade
	false,  --et.WP_MOBILE_MG42	30	Mobile MG42
	false,  --et.WP_K43	31	K43 (Axis Sniper Rifle)
	false,  --et.WP_FG42	32	FG42
	false,  --et.WP_DUMMY_MG42	33	Fixed MG42
	false,  --et.WP_MORTAR	34	Allies Mortar
	false,  --et.WP_AKIMBO_COLT	35	Akimbo Colts 1911
	false,  --et.WP_AKIMBO_LUGER	36	Akimbo Lugers
	false,  --et.WP_GPG40	37	Kar98 (Grenade Loaded)
	false,  --et.WP_M7	38	M1 Garand (Grenade Loaded)
	false,  --et.WP_SILENCED_COLT	39	Silenced Colt 1911
	false,  --et.WP_GARAND_SCOPE	40	Scoped M1 Garand (Scoped Mode)
	false,  --et.WP_K43_SCOPE	41	K43 (Scoped Mode)
	false,  --et.WP_FG42_SCOPE	42	FG42 (Scoped Mode)
	false,  --et.WP_MORTAR_SET	43	Allies Deployed Mortar
	false,  --et.WP_MEDIC_ADRENALINE	44	Adrenaline
	false,  --et.WP_AKIMBO_SILENCEDCOLT	45	Akimbo Silenced Colts 1911
	false,  --et.WP_AKIMBO_SILENCEDLUGER	46	Akimbo Silenced Lugers
	false,  --et.WP_MOBILE_MG42_SET	47	Deployed Mobile MG42
	false,  --et.WP_KNIFE_KABAR	48	Allies KA-BAR Knife
	false,  --et.WP_MOBILE_BROWNING	49	Mobile Browning
	false,  --et.WP_MOBILE_BROWNING_SET	50	Deployed Mobile Browning
	false,  --et.WP_MORTAR2	51	Axis Mortar
	false,  --et.WP_MORTAR2_SET	52	Axis Deployed Mortar
	false,  --et.WP_BAZOOKA	53	Bazooka
	false,  --et.WP_MP34	54	MP34
	false   --et.WP_AIRSTRIKE	55	Airstrike
}

maxclients = 0
gamestate = 0
gameActive = false
freezeTime = 0
startTime = 0
antigibEnable = true
clientSpawnOrigins = { }
alliesReleasedMessage = false
alliedMaxHp = 300
axisMaxHp = 151

for k=0,63 do clientSpawnOrigins[k] = 0 end

function et_InitGame(levelTime,randomSeed,restart)
	maxclients = tonumber( et.trap_Cvar_Get( "sv_maxClients" ) )   --gets the maxclients
	gamestate = tonumber( et.trap_Cvar_Get( "gamestate" ) )
	et.RegisterModname("Hide and Seek by Moe's")
	if gamestate == 0 then
		gameActive = true
		startTime = levelTime + 2000
		freezeTime = levelTime + (AMOUNT_OF_TIME_TO_FREEZE*1000)
	end
end

-- called every server frame
function et_RunFrame( levelTime )
	if math.mod(levelTime, samplerate) ~= 0 then return end
	-- for all clients
	for clientNum=0, (maxclients - 1) do
		local team = et.gentity_get(clientNum, "sess.sessionTeam")
		for k=1, (et.MAX_WEAPONS - 1), 1 do
			if not weapons[k] then
				et.gentity_set(clientNum, "ps.ammoclip", k, 0)
				et.gentity_set(clientNum, "ps.ammo", k, 0)
				et.gentity_set(clientNum, "ps.powerups", 12, levelTime + 10000 ) -- no fatigue
				if team == 1 then
					--et.gentity_set(clientNum, "ps.stats", 4, axisMaxHp)
				elseif team == 2 then
					--et.gentity_set(clientNum, "ps.stats", 4, alliedMaxHp)
					et.gentity_set(clientNum, "ps.ammo", 7, 69)
				end
				--et.gentity_set(clientNum, "ps.ammo", 11, 150)
			end
		end
		freezeBlackout(clientNum, levelTime)
		antigib(clientNum, levelTime)
	end

end

function et_ClientCommand(client, command)
	if string.lower(command) == "kill" then
		return 1
	end
	if string.lower(command) == "forcetapout" then
		return 1
	end
	if string.lower(command) == "lock" then
		return 1
	end
	if string.lower(command) == "readyteam" then
		return 1
	end
	if string.lower(command) == "timeout" then
		return 1
	end
	if string.lower(command) == "gethp" then
		local hp = et.gentity_get(client, "health")
		local maxhp = et.gentity_get(client, "ps.stats", 4)
		et.trap_SendServerCommand(client, "chat \"^7Your current hp: " .. hp .. " stat4:" .. maxhp .. "^7\"")
		return 1
	end
	if string.lower(command) == "sethp" then
		local x = et.trap_Argv(1)
		et.gentity_set(client, "health", tonumber(x))
		et.trap_SendServerCommand(client, "chat \"^7Set to " .. x .. "^7\"")
		return 1
	end
	if string.lower(command) == "setmaxhp" then
		local x = et.trap_Argv(1)
		et.gentity_set(client, "ps.stats", 4, tonumber(x))
		et.trap_SendServerCommand(client, "chat \"^7Set to " .. x .. "^7\"")
		return 1
	end
	return 0
end

function et_ClientSpawn(clientNum, revived)
	local team = et.gentity_get(clientNum, "sess.sessionTeam")
	local needles = getAlliedPlayers()
	if not needles or needles == 0 then needles = 1 end
	if revived == 0 then
		--clientSpawnOrigins[clientNum] = et.gentity_get(clientNum, "ps.origin")
		if team == 1 then -- axis
			et.gentity_set(clientNum, "ps.stats", 4, axisMaxHp - (axisMaxHp/10)) -- unify max health?
			et.gentity_set(clientNum, "health", axisMaxHp)
			et.gentity_set(clientNum, "ps.ammoclip",11, needles)
		elseif team == 2 then
			et.gentity_set(clientNum, "ps.stats", 4, alliedMaxHp- (alliedMaxHp/10))
			et.gentity_set(clientNum, "health", alliedMaxHp)
		end
	end
end

function getAlliedPlayers()
	local allies = 0
	for clientNum=0, maxclients-1 do
		local team = et.gentity_get(clientNum, "sess.sessionTeam")
		if team == 2 then
			allies = allies + 1
		end
	end
	return allies
end


function freezeBlackout(clientNum, levelTime)
	if not gameActive then return end
	if freezeTime <= levelTime then
		if not alliesReleasedMessage then
			et.trap_SendServerCommand( -1, "chat \"^1A^7llies have been released^1!^7\"")
			alliesReleasedMessage = true
		end
		return
	end
	if et.gentity_get(clientNum, "sess.sessionTeam") == 2 then
		et.gentity_set(clientNum, "ps.powerups", 14, levelTime + 500 )
		et.gentity_set(clientNum, "ps.powerups", 1, levelTime + 5000 )
		if levelTime >= startTime then
			if clientSpawnOrigins[clientNum] == 0 then
				clientSpawnOrigins[clientNum] = et.gentity_get(clientNum, "ps.origin")
			else
				et.gentity_set(clientNum, "ps.origin", clientSpawnOrigins[clientNum])
			end
		end
	end
end

function antigib(clientNum, levelTime)
	if not antigibEnable then return end
	local hp = tonumber(et.gentity_get(clientNum,"health"))
	local body = tonumber(et.gentity_get(clientNum, "r.contents"))
	if body == 67108864 then
		et.gentity_set(clientNum, "ps.powerups", 1, levelTime + 9999999 )
	elseif body == 33554432 and hp >= 76 then
		et.gentity_set(clientNum, "ps.powerups", 1, 0 )
	end
end

function removeEntities()
	for i = 64, 1021 do
		local classname = et.gentity_get(i, "classname")
		if string.find(classname, "mg42", 0, 1) or classname == "func_door_rotating" then
			et.trap_UnlinkEntity( i )
		end
		if mapname == "adlernest" then
			if et.gentity_get(i, "origin") == "733 -2971 -120" then
				et.trap_UnlinkEntity( i )
			end
		end
	end
end


-- called every server frame
function et_RunFrame( levelTime )

   if math.mod(levelTime, samplerate) ~= 0 then return end
   -- for all clients
   for j = 0, (maxclients - 1) do
      for k=1, (et.NUM_OF_WEAPONS - 1), 1 do
          if not weapons[k] then
            et.gentity_set(j, "ps.ammoclip", k, 0)
            et.gentity_set(j, "ps.ammo", k, 0)
            et.gentity_set(j, "ps.ammo", 8, 150)
	    et.gentity_set(j, "ps.ammo", 12, 150)
	    et.gentity_set(j, "ps.powerups", 13, levelTime + 10000 )
         end
      end
   end
end

function et_ClientBegin( clientNum )
   et.trap_SendServerCommand(clientNum, "cp \"^wHide ^1& ^wSeek\"\n" )
end

function et_ClientCommand(client, command)
  if string.lower(command) == "kill" then
    et.trap_SendServerCommand( client, "cp \"^1Sorry^z, ^wit's ^1disabled ^win the current game type^z.\n\"" )
    return 1
  end
  if string.lower(command) == "forcetapout" then
    et.trap_SendServerCommand( clientNum, "cp \"^1Sorry, tapping out is disabled in the current game type.\n\"" )
    return 1
  end
  if string.lower(command) == "lock" then
    et.trap_SendServerCommand( clientNum, "cpm \"^1Sorry, this command is disabled in the current game type.\n\"" )
    return 1
  end
end
