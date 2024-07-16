myid=9999 -- 일단 id값 임의 초기화

function set_uid(x)
myid=x
end

--[[
function event_player_move(p_id)
	if API_get_x(p_id)==API_get_x(myid) then
		if API_get_y(p_id)==API_get_y(myid) then
			API_send_message(myid, player, "HELLO")
		end
	end
end
--]]

function quest_accept(p_id)
	if API_get_x(p_id)==API_get_x(myid) then
		if API_get_y(p_id)==API_get_y(myid) then
			API_send_message(myid, p_id, "Quest Accept")
		end
	end
end	

function event_player_move(p_id)
    local player_x = API_get_x(p_id)
    local player_y = API_get_y(p_id)
    local npc_x = API_get_x(myid)
    local npc_y = API_get_y(myid)
    
    if math.abs(player_x - npc_x) <= 2 and math.abs(player_y - npc_y) <= 2 then
        API_send_message(myid, p_id, "Enemy!")
    end
end