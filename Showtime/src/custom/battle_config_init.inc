// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom battle config initializations
 * Place the definition of custom battle configuration initializations in this file.
 *
 * Do not forget to add a comma at the end of the line, if you want to add another config
 *
 * Format:
 *	 { "name", &battle_config.<variable name>, <default value>, <minimum value>, <maximum value> },
 **/

	{ "afk_timeout", &battle_config.afk_timeout, 0, 0, INT_MAX,  },
	{ "bg_eAmod_mode",                      &battle_config.bg_eAmod_mode,                   1,      0,      1,              },
	{ "bg_idle_announce",                   &battle_config.bg_idle_announce,                0,      0,      INT_MAX,        },
	{ "bg_idle_autokick",                   &battle_config.bg_idle_autokick,                0,      0,      INT_MAX,        },
	{ "bg_reward_rates",                    &battle_config.bg_reward_rates,                 100,    0,      INT_MAX,        },
	{ "bg_reportafk_leaderonly",            &battle_config.bg_reportafk_leaderonly,         1,      0,      1,              },
	{ "bg_queue2team_balanced",             &battle_config.bg_queue2team_balanced,          1,      0,      1,              },
	{ "bg_queue_onlytowns",                 &battle_config.bg_queue_onlytowns,              1,      0,      1,              },
	{ "bg_order_behavior",                  &battle_config.bg_order_behavior,               1,      0,      1,              },
	{ "bg_reserved_char_id",                &battle_config.bg_reserved_char_id,             999996, 0,      INT_MAX,        },
	{ "woe_reserved_char_id",				&battle_config.woe_reserved_char_id,            999997, 0,      INT_MAX,        },
	{ "bg_can_trade",				        &battle_config.bg_can_trade,                    1,      0,      1,              },
	{ "bg_double_login",				    &battle_config.bg_double_login,                 1,      0,      1,              },
