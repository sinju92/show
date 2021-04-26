// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Atcommands
 * Place the body of custom atcommands in this file.
 * Format:
 *	ACMD_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//ACMD_FUNC(newcommand)
//{
//	clif_displaymessage(fd, "It works!");
//	clif_specialeffect(&sd->bl, EF_HEARTCASTING, AREA);
//	return 0;
//}
//===============================
//@partybuff or @spb
//===============================
ACMD_FUNC(partybuff)
{ // [Vykimo]
	struct party_data* p = NULL;
	nullpo_retr(-1, sd);

	if( !sd->status.party_id ) {
		clif_displaymessage(fd, msg_txt(sd,1071)); // You're not in a party.
		return -1;
	}

	p = party_search(sd->status.party_id);

	if( sd->state.spb ) {
		sd->state.spb = 0;
		clif_displaymessage(fd, msg_txt(sd,1072)); // Displaying party member's buffs disabled.
	} else {
		sd->state.spb = 1;
		clif_displaymessage(fd, msg_txt(sd,1073)); // Displaying party member's buffs enabled.
	}

	clif_party_info(p,sd);
	return 0;
}

/*==========================================
* @afk
*------------------------------------------*/
ACMD_FUNC(afk) {

		nullpo_retr(-1, sd);
				
				if(sd->bl.m == map_mapname2mapid("prontera")) {
					clif_displaymessage(fd, "@afk is not allowed on this map.");
					return 0;
				}
				
				if( pc_isdead(sd) ) {
					clif_displaymessage(fd, "Cannot @afk if you are dead.");
					return -1;
				}

				if( map_getmapflag(sd->bl.m, MF_AUTOTRADE) == battle_config.autotrade_mapflag )
				{
				if( map_getmapflag(sd->bl.m, MF_PVP) || map_getmapflag(sd->bl.m, MF_GVG_CASTLE) ) {
					clif_displaymessage(fd, "You may not use the @afk maps PVP or GVG.");
					return -1;
				}
					sd->state.autotrade = 1;
//					sd->state.monster_ignore = 1;
					sd->state.block_action |= PCBLOCK_IMMUNE;
//					pc_setsit(sd);
//					skill_sit(sd,1);
//					clif_sitting(&sd->bl);
//					clif_changelook(&sd->bl,LOOK_HEAD_TOP,471);
//					clif_specialeffect(&sd->bl, 234,AREA);                       
					if( battle_config.afk_timeout )
					{
							int timeout = atoi(message);
							status_change_start(NULL, &sd->bl, SC_AUTOTRADE, 10000,0,0,0,0, ((timeout > 0) ? min(timeout,battle_config.afk_timeout) : battle_config.afk_timeout)*60000,0);
					}
						clif_authfail_fd(fd, 15);
				} else
						clif_displaymessage(fd, "@afk is not allowed on this map.");
		return 0;
}

/*==========================================
* Battleground Leader Commands
*------------------------------------------*/
ACMD_FUNC(order)
{
	nullpo_retr(-1,sd);
	memset(atcmd_output, '\0', sizeof(atcmd_output));
	if( !message || !*message )
	{
		clif_displaymessage(fd, "Please, enter a message (usage: @order <message>).");
		return -1;
	}

	if( map_getmapflag(sd->bl.m, MF_BATTLEGROUND) )
	{
		if( !sd->bmaster_flag )
		{
			clif_displaymessage(fd, "This command is reserved for Team Leaders Only.");
			return -1;
		}
		if (battle_config.bg_order_behavior)
			sprintf(atcmd_output, "%s: %s", sd->status.name, message);
		else
			sprintf(atcmd_output, "Team Leader: %s", message);
		clif_broadcast2(&sd->bl, atcmd_output, (int)strlen(atcmd_output)+1, sd->bmaster_flag->color, 0x190, 20, 0, 0, BG);
	}
	else
	{
		if( !sd->state.gmaster_flag )
		{
			clif_displaymessage(fd, "This command is reserved for Guild Leaders Only.");
			return -1;
		}
		clif_broadcast2(&sd->bl, message, (int)strlen(message)+1, 0xFF0000, 0x190, 20, 0, 0, GUILD);
	}

	return 0;
}

ACMD_FUNC(leader)
{
	struct map_session_data *pl_sd = NULL;
	nullpo_retr(-1,sd);
	memset(atcmd_player_name, '\0', sizeof(atcmd_player_name));
	
	if (!message || !*message || sscanf(message, "%23[^\n]", atcmd_player_name) < 1) {
		clif_displaymessage(fd,"Please enter a player name (usage: @leader <char name/ID>).");
		return -1;
	}

	if( !sd->bmaster_flag )
		clif_displaymessage(fd, "This command is reserved for Team Leaders Only.");
	else if( sd->ud.skilltimer != INVALID_TIMER )
		clif_displaymessage(fd, "Command not allow while casting a skill.");
	else if( !message || !*message )
		clif_displaymessage(fd, "Please, enter the new Leader name (usage: @leader <name>).");
	else if((pl_sd=map_nick2sd(atcmd_player_name,true)) == NULL && (pl_sd=map_charid2sd(atoi(atcmd_player_name))) == NULL)
		clif_displaymessage(fd, "Character not found.");
	else if( sd->bg_id != pl_sd->bg_id )
		clif_displaymessage(fd, "Destination Player is not in your Team.");
	else if( sd == pl_sd )
		clif_displaymessage(fd, "You are already the Team Leader.");
	else
	{ // Everytest OK!
		sprintf(atcmd_output, "Team Leader transfered to [%s]", pl_sd->status.name);
		clif_broadcast2(&sd->bl, atcmd_output, (int)strlen(atcmd_output)+1, sd->bmaster_flag->color, 0x190, 20, 0, 0, BG);

		sd->bmaster_flag->leader_char_id = pl_sd->status.char_id;
		pl_sd->bmaster_flag = sd->bmaster_flag;
		sd->bmaster_flag = NULL;

		clif_name_area(&sd->bl);
		clif_name_area(&pl_sd->bl);
		return 0;
	}
	return -1;
}

ACMD_FUNC(reportafk)
{
	struct map_session_data *pl_sd = NULL;
	nullpo_retr(-1,sd);
	memset(atcmd_player_name, '\0', sizeof(atcmd_player_name));
	
	if (!message || !*message || sscanf(message, "%23[^\n]", atcmd_player_name) < 1) {
		clif_displaymessage(fd,"Please enter a player name (usage: @reportafk <char name/ID>).");
		return -1;
	}
	if( !sd->bg_id )
		clif_displaymessage(fd, "This command is reserved for Battleground Only.");
	else if( !sd->bmaster_flag && battle_config.bg_reportafk_leaderonly )
		clif_displaymessage(fd, "This command is reserved for Team Leaders Only.");
	else if( !message || !*message )
		clif_displaymessage(fd, "Please, enter the character name (usage: @reportafk <name>).");
	else if((pl_sd=map_nick2sd(atcmd_player_name,true)) == NULL && (pl_sd=map_charid2sd(atoi(atcmd_player_name))) == NULL)
		clif_displaymessage(fd, "Character not found");
	else if( sd->bg_id != pl_sd->bg_id )
		clif_displaymessage(fd, "Destination Player is not in your Team.");
	else if( sd == pl_sd )
		clif_displaymessage(fd, "You cannot kick yourself.");
	else if( pl_sd->state.bg_afk == 0 )
		clif_displaymessage(fd, "The player is not AFK on this Battleground.");
	else
	{ // Everytest OK!
		struct battleground_data *bgd;
		if( (bgd = bg_team_search(sd->bg_id)) == NULL )
			return -1;

		bg_team_leave(pl_sd,2);
		clif_displaymessage(pl_sd->fd, "You have been kicked from Battleground because of your AFK status.");
		pc_setpos(pl_sd,pl_sd->status.save_point.map,pl_sd->status.save_point.x,pl_sd->status.save_point.y,CLR_TELEPORT);

		sprintf(atcmd_output, "- AFK [%s] Kicked by @reportafk command-", pl_sd->status.name);
		clif_broadcast2(&sd->bl, atcmd_output, (int)strlen(atcmd_output)+1, bgd->color, 0x190, 20, 0, 0, BG);
		return 0;
	}
	return -1;
}

ACMD_FUNC(listenbg)
{
	if (sd->state.bg_listen)
	{
		sd->state.bg_listen = 0;
		clif_displaymessage(fd, "You will receive Battleground announcements");
	}
	else
	{
		sd->state.bg_listen = 1;
		clif_displaymessage(fd, "You will not receive Battleground announcements.");
	}

	return 0;
}

/*==========================================
* Guild Skill Usage for Guild Masters
*------------------------------------------*/
ACMD_FUNC(bgskill)
{
	int i, skillnum = 0, skilllv = 0;
	t_tick tick = gettick();
	struct battleground_data *bgd;

	const struct { char skillstr[3]; int id; } skills[] = {
		{ "BO",	10010 },
		{ "RG",	10011 },
		{ "RS",	10012 },
		{ "EC",	10013 },
	};

	// Check for Skill ID
	for( i = 0; i < ARRAYLENGTH(skills); i++ )
	{
		if( strncmpi(message, skills[i].skillstr, 3) == 0 )
		{
			skillnum = skills[i].id;
			break;
		}
	}
	if( !skillnum )
	{
		clif_displaymessage(fd, "Invalid Skill string. Use @bgskill EC/RS/RG/BO");
		return -1;
	}

	if( !map_getmapflag(sd->bl.m, MF_BATTLEGROUND) )
	{
		clif_displaymessage(fd, "This command is only available for Battleground.");
		return -1;
	}
	else
	{
		if( (bgd = sd->bmaster_flag) != NULL )
		{
			if( bgd->skill_block_timer[skillnum] == INVALID_TIMER )
				skilllv = bg_checkskill(bgd, skillnum);
			else
			{
				bg_block_skill_status(bgd, skillnum);
				skilllv = 0;
			}
		}
		else
		{
			clif_displaymessage(fd, "This command is reserved for Team Leaders Only.");
			return -1;
		}
	}

	if( pc_cant_act(sd) || pc_issit(sd) || skill_isNotOk(skillnum, sd) || sd->ud.skilltimer != -1 || sd->sc.option&(OPTION_WEDDING|OPTION_XMAS|OPTION_SUMMER) || sd->state.only_walk || sd->sc.data[SC_BASILICA] )
		return -1;

	if( DIFF_TICK(tick, sd->ud.canact_tick) < 0 )
		return -1;

	if( sd->menuskill_id )
	{
		if( sd->menuskill_id == SA_TAMINGMONSTER )
			sd->menuskill_id = sd->menuskill_val = 0; //Cancel pet capture.
		else if( sd->menuskill_id != SA_AUTOSPELL )
			return -1; //Can't use skills while a menu is open.
	}

	sd->skillitem = sd->skillitemlv = 0;
	if( skilllv ) unit_skilluse_id(&sd->bl, sd->bl.id, skillnum, skilllv);
	return 0;
}
/*==========================================
 * Comando para buscar un vendedor de X item [Zephyrus]
 *------------------------------------------*/
ACMD_FUNC(whosell)
{
	struct map_session_data *pl_sd, *b_sd[MAX_SEARCH];
	struct s_mapiterator* iter;

	struct item_data *item_array[MAX_SEARCH];
	int total[MAX_SEARCH], amount[MAX_SEARCH];
	unsigned int MinPrice[MAX_SEARCH], MaxPrice[MAX_SEARCH];
	char output[256];
	int i, j, count = 1;

	if( !message || !*message )
	{
		clif_displaymessage(fd, "Please, enter Item name or its ID (usage: @whosell <item name or ID>).");
		return -1;
	}

	if( (item_array[0] = itemdb_exists(atoi(message))) == NULL )
		count = itemdb_searchname_array(item_array, MAX_SEARCH, message);

	if( count < 1 )
	{ // No items found
		clif_displaymessage(fd, msg_txt(sd,19));
		return -1;
	}

	if( count > MAX_SEARCH ) count = MAX_SEARCH;

	// Preparing Search Recorders
	for( i = 0; i < MAX_SEARCH; i++ )
	{
		total[i] = amount[i] = MaxPrice[i] = 0;
		MinPrice[i] = battle_config.vending_max_value + 1;
		b_sd[i] = NULL;
	}

	iter = mapit_getallusers();
	for( pl_sd = (TBL_PC*)mapit_first(iter); mapit_exists(iter); pl_sd = (TBL_PC*)mapit_next(iter) )
	{
		if( !pl_sd->vender_id ) continue;
		for( i = 0; i < pl_sd->vend_num; i++ )
		{ // Searching in the Vending List
			for( j = 0; j < count; j++ )
			{ // Compares with each search result
				if( pl_sd->cart.u.items_cart[pl_sd->vending[i].index].nameid != item_array[j]->nameid )
					continue;

				amount[j] += pl_sd->vending[i].amount;
				total[j]++;

				if( pl_sd->vending[i].value < MinPrice[j] )
				{ // Best Price
					MinPrice[j] = pl_sd->vending[i].value;
					b_sd[j] = pl_sd;
				}
				if( pl_sd->vending[i].value > MaxPrice[j] )
					MaxPrice[j] = pl_sd->vending[i].value;
			}
		}
	}
	mapit_free(iter);

	for( i = 0; i < count; i++ )
	{
		if( total[i] > 0 && b_sd[i] != NULL )
		{
			sprintf(output, "[%d] The best price of '%s' is %u by '%s' in %s <%d,%d>. Max %u. Found in %d shops, %d on sale.", item_array[i]->nameid, item_array[i]->jname, MinPrice[i], b_sd[i]->status.name, map[b_sd[i]->bl.m].name, b_sd[i]->bl.x, b_sd[i]->bl.y, MaxPrice[i], total[i], amount[i]);
			if( sd->bl.m == b_sd[i]->bl.m )
				clif_viewpoint(sd, 1, 1, b_sd[i]->bl.x, b_sd[i]->bl.y, i, 0xFFFFFF);
		}
		else
			sprintf(output, "[%d] '%s' not in stores...", item_array[i]->nameid, item_array[i]->jname);

		clif_displaymessage(sd->fd, output);
	}

	return 0;
}
/*==========================================
* @whobuy - List who is buying the item (amount, price, and location).
* remake by VoidLess, original by zephyrus_cr
* re-edit by deathscythe to work in rAthena
*------------------------------------------*/
ACMD_FUNC(whobuy)
{
    char item_name[100];
    int item_id, j, count = 0, sat_num = 0;
    bool flag = 0; // place dot on the minimap?
    struct map_session_data* pl_sd;
    struct s_mapiterator* iter;
    unsigned int MinPrice = battle_config.vending_max_value, MaxPrice = 0;
    struct item_data *item_data;

    nullpo_retr(-1, sd);
    memset(item_name, '\0', sizeof(item_name));

    if (!message || !*message || sscanf(message, "%99[^\n]", item_name) < 1) {
        clif_displaymessage(fd, "Input item name or ID (use: @whobuy <name or ID>).");
        return -1;
    }
    if ((item_data = itemdb_searchname(item_name)) == NULL &&
        (item_data = itemdb_exists(atoi(item_name))) == NULL)
    {
        clif_displaymessage(fd, msg_txt(sd,19)); // Invalid item ID or name.
        return -1;
    }

    item_id = item_data->nameid;

    iter = mapit_getallusers();
    for( pl_sd = (TBL_PC*)mapit_first(iter); mapit_exists(iter); pl_sd = (TBL_PC*)mapit_next(iter) )
    {
        if( pl_sd->state.buyingstore ) //check if player is autobuying
        {
			for (j = 0; j < pl_sd->buyingstore.slots; j++) {
				if(pl_sd->buyingstore.items[j].nameid == item_id) {
					snprintf(atcmd_output, CHAT_SIZE_MAX, "Price %d | Amount %d | Buyer %s | Map %s[%d,%d]",pl_sd->buyingstore.items[j].price,pl_sd->buyingstore.items[j].amount,pl_sd->status.name,mapindex_id2name(pl_sd->mapindex),pl_sd->bl.x, pl_sd->bl.y);
					if(pl_sd->buyingstore.items[j].price < MinPrice) MinPrice = pl_sd->buyingstore.items[j].price;
					if(pl_sd->buyingstore.items[j].price > MaxPrice) MaxPrice = pl_sd->buyingstore.items[j].price;
                    clif_displaymessage(fd, atcmd_output);
                    count++;
                    flag = 1;
                }
            }
            if(flag && pl_sd->mapindex == sd->mapindex){
                clif_viewpoint(sd, 1, 1, pl_sd->bl.x, pl_sd->bl.y, ++sat_num, 0xFFFFFF);
                flag = 0;
            }
        }
    }
    mapit_free(iter);

    if(count > 0) {
        snprintf(atcmd_output,CHAT_SIZE_MAX, "Found %d ea. Prices from %dz to %dz", count, MinPrice, MaxPrice);
        clif_displaymessage(fd, atcmd_output);
    } else
        clif_displaymessage(fd, "Nobody buying it now.");

    return 0;
}
