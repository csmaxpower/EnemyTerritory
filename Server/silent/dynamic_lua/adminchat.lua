--[[
Purple Admin Chat
Version 0.1

--]]

modname = "AdminChat"
version = "0.1"
admin_chat = "ml"
min_level = 3


function et_InitGame(levelTime, randomSeed, restart)
  et.RegisterModname(modname .. " " .. version)
  maxclients = tonumber(et.trap_Cvar_Get("sv_maxclients"))-1
end

function et_ClientCommand(clientNum, command) 
   if command == admin_chat and getlevel(clientNum) then
		if et.trap_Argc() > 1 then
		local message = ""
			for i = 1, et.trap_Argc() - 1, 1 do
				message = message .. et.trap_Argv(i) .. " "
			end
			for i = 0, maxclients, 1 do
				sendstring = playerName(clientNum) .. " ^w(^8AdminChat^w)^8: " .. message
				et.trap_SendServerCommand(i, "chat \"" .. sendstring .. "\"")
				return 1
			end
		end
		elseif command == admin_chat and not getlevel(clientNum) then
			et.trap_SendServerCommand(clientNum, "cpm \"^1 You Are Not Allowed To Use Admin Chat.\n\" " )
		return 1
    end
end

function getlevel(client)
	local lvl = et.G_shrubbot_level(client)
	if lvl >= min_level then
		return true
	end
		return nil
end

function playerName(id)
  local name = et.Info_ValueForKey(et.trap_GetUserinfo(id), "name")
  if name == "" then
    return "*unknown*"
  end
  return name
end