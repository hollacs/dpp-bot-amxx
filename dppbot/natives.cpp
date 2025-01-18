#include "dcbot.h"
#include "natives.h"

extern DcBot g_bot;

cell AMX_NATIVE_CALL native_is_made(AMX* amx, cell* params)
{
	return g_bot.IsMade() ? 1 : 0;
}

cell AMX_NATIVE_CALL native_is_running(AMX* amx, cell* params)
{
	if (!g_bot.IsMade())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "bot not made yet");
		return 0;
	}

	return g_bot.IsRunning() ? 1 : 0;
}

cell AMX_NATIVE_CALL native_make(AMX* amx, cell* params)
{
	if (g_bot.IsMade())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "bot already made");
		return 0;
	}

	const char* token = MF_GetAmxString(amx, params[1], 0, nullptr);
	g_bot.Make(token, dpp::i_default_intents | dpp::i_message_content);
	return 1;
}

cell AMX_NATIVE_CALL native_start(AMX* amx, cell* params)
{
	if (!g_bot.IsMade())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "bot not made yet");
		return 0;
	}

	if (g_bot.IsRunning())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "bot already running");
		return 0;
	}

	g_bot.Start();
	return 1;
}

cell AMX_NATIVE_CALL native_shutdown(AMX* amx, cell* params)
{
	if (!g_bot.IsMade())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "bot not made yet");
		return 0;
	}

	if (!g_bot.IsRunning())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "bot already running");
		return 0;
	}

	if (g_bot.IsStarting())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "bot is still starting up...");
		return 0;
	}

	g_bot.Shutdown();
	return 1;
}

cell AMX_NATIVE_CALL native_destroy(AMX* amx, cell* params)
{
	if (!g_bot.IsMade())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "bot not made yet");
		return 0;
	}

	g_bot.Clean();
	return 1;
}

cell AMX_NATIVE_CALL native_register_cmd(AMX* amx, cell* params)
{
	if (!g_bot.IsMade())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "bot not made yet");
		return 0;
	}

	std::string cmd = MF_GetAmxString(amx, params[1], 0, nullptr);
	if (cmd.empty())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "cmd cannot be empty");
		return 0;
	}

	const char* callback = MF_GetAmxString(amx, params[2], 1, nullptr);
	int fwd = MF_RegisterSPForwardByName(amx, callback, ET_IGNORE, FP_DONE);
	if (fwd == -1)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "public (%s) not found", callback);
		return 0;
	}

	std::string desc = MF_GetAmxString(amx, params[3], 2, nullptr);

	g_bot.RegisterCmd(cmd, fwd, desc);
	return 1;
}

cell AMX_NATIVE_CALL native_msg_reply(AMX* amx, cell* params)
{
	if (g_bot.MsgCreateEvent() == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "message create event is null");
		return 0;
	}

	g_bot.MsgReply();
	return 1;
}

cell AMX_NATIVE_CALL native_create_message(AMX* amx, cell* params)
{
	g_bot.CreateMessage();
	return 0;
}

cell AMX_NATIVE_CALL native_cmd_reply(AMX* amx, cell* params)
{
	if (g_bot.SlashCmdEvent() == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "slash cmd event is null");
		return 0;
	}

	g_bot.CmdReply();
	return 1;
}

cell AMX_NATIVE_CALL native_msg(AMX* amx, cell* params)
{
	std::string content = MF_GetAmxString(amx, params[1], 0, nullptr);
	g_bot.NewMessage();
	g_bot.MsgSetContent(content);
	return 0;
}

cell AMX_NATIVE_CALL native_msg_set_channel(AMX* amx, cell* params)
{
	const char* channel = MF_GetAmxString(amx, params[1], 0, nullptr);
	g_bot.MsgSetChannel(dpp::snowflake(std::stoull(channel)));
	return 0;
}

cell AMX_NATIVE_CALL native_msg_set_flags(AMX* amx, cell* params)
{
	g_bot.MsgSetFlags(static_cast<uint16_t>(params[1]));
	return 0;
}

cell AMX_NATIVE_CALL native_msg_add_embed(AMX* amx, cell* params)
{
	g_bot.MsgAddEmbed();
	return 0;
}

cell AMX_NATIVE_CALL native_embed(AMX* amx, cell* params)
{
	g_bot.NewEmbed();
	return 0;
}

cell AMX_NATIVE_CALL native_embed_set_title(AMX* amx, cell* params)
{
	const char* title = MF_GetAmxString(amx, params[1], 0, nullptr);
	g_bot.EmbedSetTitle(title);
	return 0;
}

cell AMX_NATIVE_CALL native_embed_set_desc(AMX* amx, cell* params)
{
	const char* desc = MF_GetAmxString(amx, params[1], 0, nullptr);
	g_bot.EmbedSetDesc(desc);
	return 0;
}

cell AMX_NATIVE_CALL native_embed_set_color(AMX* amx, cell* params)
{
	std::string code = MF_GetAmxString(amx, params[1], 0, nullptr);
	
	uint32_t color;
	if (code[0] == '#')
		color = std::stoul(code.substr(1), nullptr, 16);
	else
		color = std::stoul(code, nullptr, 16);

	g_bot.EmbedSetColor(color);
	return 0;
}

cell AMX_NATIVE_CALL native_set_presence(AMX* amx, cell* params)
{
	if (!g_bot.IsMade())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "bot not made yet");
		return 0;
	}

	if (!g_bot.IsRunning())
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "bot not running.");
		return 0;
	}

	dpp::presence_status status = static_cast<dpp::presence_status>(params[1]);
	dpp::activity_type type = static_cast<dpp::activity_type>(params[2]);
	std::string desc = MF_GetAmxString(amx, params[3], 0, nullptr);

	g_bot.SetPresence(dpp::presence(status, type, desc));
	return 1;
}

cell AMX_NATIVE_CALL native_escape_markdown(AMX* amx, cell* params)
{
	std::string input = MF_GetAmxString(amx, params[1], 0, nullptr);
	std::string output = g_bot.EscapeMarkdown(input);

	MF_SetAmxString(amx, params[2], output.c_str(), params[3]);
	return 0;
}

cell AMX_NATIVE_CALL native_is_msg_author_bot(AMX* amx, cell* params)
{
	if (g_bot.MsgCreateEvent() == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "message create event is null");
		return 0;
	}

	return g_bot.MsgCreateEvent()->msg.author.is_bot() ? 1 : 0;
}

cell AMX_NATIVE_CALL native_get_msg_author_username(AMX* amx, cell* params)
{
	if (g_bot.MsgCreateEvent() == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "message create event is null");
		return 0;
	}

	std::string username = g_bot.MsgCreateEvent()->msg.author.username;
	MF_SetAmxString(amx, params[1], username.c_str(), params[2]);
	return 0;
}

cell AMX_NATIVE_CALL native_get_msg_content(AMX* amx, cell* params)
{
	if (g_bot.MsgCreateEvent() == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "message create event is null");
		return 0;
	}

	std::string content = g_bot.MsgCreateEvent()->msg.content;
	MF_SetAmxString(amx, params[1], content.c_str(), params[2]);
	return 0;
}

cell AMX_NATIVE_CALL native_get_msg_channel_id(AMX* amx, cell* params)
{
	if (g_bot.MsgCreateEvent() == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "message create event is null");
		return 0;
	}

	std::string channel = std::to_string(g_bot.MsgCreateEvent()->msg.channel_id);
	MF_SetAmxString(amx, params[1], channel.c_str(), params[2]);
	return 0;
}

cell AMX_NATIVE_CALL native_get_cmd_channel_id(AMX* amx, cell* params)
{
	if (g_bot.SlashCmdEvent() == nullptr)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "slash cmd event is null");
		return 0;
	}

	std::string channel = std::to_string(g_bot.SlashCmdEvent()->command.channel_id);
	MF_SetAmxString(amx, params[1], channel.c_str(), params[2]);
	return 0;
}