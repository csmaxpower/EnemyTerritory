local Version = "1.0"
local Author  = "^eMoe's Tavern"
local Description = "^7Hide^1&^7Seek for ETLegacy"
local Homepage = "https://discord.moestavern.pub"


et.NUM_OF_WEAPONS = 56
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

function et_InitGame(levelTime,randomSeed,restart)
	maxclients = tonumber( et.trap_Cvar_Get( "sv_maxClients" ) )   --gets the maxclients
	et.G_Print("Hide & Seek Version:"..Version.." Loaded\n")
   	et.RegisterModname("ETL Hide & Seek by Moe's")
	et.trap_SendServerCommand(-1, "b 8 \""..Description.." ^1[^7LUA^1] ^7powered by "..Author.." ^0^7Visit "..Homepage.." \n\"" )
		count = {}
			for i = 0, maxclients - 1 do
			count[i] = 0
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

function et_ClientBegin( clientNum )
   et.trap_SendServerCommand(clientNum, "cp \"^wHide ^1& ^wSeek\"\n" )
end

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
