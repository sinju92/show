// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom Script Commands
 * Place the body of custom script commands in this file.
 * Format:
 *	BUILDIN_FUNC(command_name)
 *	{
 *		<code>;
 *	}
 **/

//BUILDIN_FUNC(example)
//{
//	ShowInfo("buildin_example: It works!\n");
//	script_pushint(st,1);
//	return 0;
//}

/*
    fakeplayer( map, x, y, name, job_id, sex, hair_style, hair_color, weapon, shield, head_top, head_mid, head_bottom, option, cloth_color );
    Returns the GID
*/
 BUILDIN_FUNC(fakeplayer)
 {
     const char* mapn = script_getstr(st, 2);
     int x = script_getnum(st, 3);
     int y = script_getnum(st, 4);
 
     const char* name = script_getstr(st, 5);
     int sprite = script_getnum(st, 6);
     int sex = script_getnum(st, 7);
     int hair_style = script_getnum(st, 8);
     int hair_color = script_getnum(st, 9);
     int weapon = script_getnum(st, 10);
     int shield = script_getnum(st, 11);
     int head_top = script_getnum(st, 12);
     int head_mid = script_getnum(st, 13);
     int head_bottom = script_getnum(st, 14);
     int option = script_getnum(st, 15);
     int cloth_color = script_getnum(st, 16);
 
     int16 m;
     struct mob_data* md = nullptr;
 
     m = map_mapname2mapid(mapn);
 
	 // Using poring as monster behavior
     md = mob_once_spawn_sub(NULL, m, x, y, name, 3225, "", SZ_SMALL, AI_NONE);
 
     if (!md)
         return SCRIPT_CMD_FAILURE;
 
     md->ud.immune_attack = 0;
 
     md->vd->class_ = sprite;
 
     md->vd->sex = sex;
     md->vd->hair_style = hair_style;
     md->vd->hair_color = hair_color;
     md->vd->weapon = weapon;
     md->vd->shield = shield;
     md->vd->head_top = head_top;
     md->vd->head_mid = head_mid;
     md->vd->head_bottom = head_bottom;
     md->vd->cloth_color = cloth_color;
 
     mob_spawn(md);
     script_pushint(st, md->bl.id);
     return SCRIPT_CMD_SUCCESS;
 }
 