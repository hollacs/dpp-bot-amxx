#include "module.h"
#include "dcbot.h"
#include "natives.h"

int g_fw_ready, g_fw_register_commands, g_fw_message_create;
DcBot g_bot;

void OnAmxxAttach()
{
	static const AMX_NATIVE_INFO dpp_natives[] =
	{
		{ "dpp_is_made", native_is_made },
		{ "dpp_is_running", native_is_running },

		{ "dpp_make", native_make },
		{ "dpp_destory", native_destroy },

		{ "dpp_start", native_start },
		{ "dpp_shutdown", native_shutdown },

		{ "dpp_register_cmd", native_register_cmd },
		{ "dpp_create_message", native_create_message },

		{ "dpp_msg_reply", native_msg_reply },
		{ "dpp_cmd_reply", native_cmd_reply },

		{ "dpp_msg", native_msg },
		{ "dpp_msg_set_channel", native_msg_set_channel },
		{ "dpp_msg_set_flags", native_msg_set_flags },
		{ "dpp_msg_add_embed", native_msg_add_embed },

		{ "dpp_embed", native_embed },
		{ "dpp_embed_set_title", native_embed_set_title },
		{ "dpp_embed_set_desc", native_embed_set_desc },
		{ "dpp_embed_set_color", native_embed_set_color },

		{ "dpp_set_presence", native_set_presence },

		{ "dpp_escape_markdown", native_escape_markdown },

		{ "dpp_is_msg_author_bot", native_is_msg_author_bot },
		{ "dpp_get_msg_author_username", native_get_msg_author_username },
		{ "dpp_get_msg_content", native_get_msg_content },
		{ "dpp_get_msg_channel_id", native_get_msg_channel_id },

		{ "dpp_get_cmd_channel_id", native_get_cmd_channel_id },

		{ nullptr, nullptr }
	};

	MF_AddNatives(dpp_natives);
}

void OnPluginsLoaded()
{
	g_fw_ready = MF_RegisterForward("dpp_on_ready", ET_IGNORE, FP_DONE);
	g_fw_message_create = MF_RegisterForward("dpp_on_message_create", ET_IGNORE, FP_DONE);
}

void OnPluginsUnloaded()
{
	g_bot.Clean();
}