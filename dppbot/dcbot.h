#pragma once

#include <dpp/dpp.h>

class DcBot
{
public:
	DcBot();

	void Make(const std::string& token, uint32_t intents);
	void ThreadStart();
	bool Start();
	void Shutdown();
	void RegisterCmd(const std::string& cmd, int fwd, const std::string& desc);
	void NewMessage();
	void MsgSetChannel(dpp::snowflake channel);
	void MsgSetContent(const std::string& content);
	void MsgAddEmbed();
	void MsgSetFlags(uint16_t flags);
	void NewEmbed();
	void EmbedSetTitle(const std::string& title);
	void EmbedSetDesc(const std::string& desc);
	void EmbedSetColor(uint32_t color);
	bool CmdReply();
	bool MsgReply();
	bool CreateMessage();
	void Clean();
	std::string EscapeMarkdown(const std::string& input);
	void SetPresence(const dpp::presence& presence);

	dpp::message& Message();
	dpp::embed& Embed();
	const dpp::ready_t* ReadyEvent();
	const dpp::slashcommand_t* SlashCmdEvent();
	const dpp::message_create_t* MsgCreateEvent();
	std::weak_ptr<dpp::cluster> Bot();

	bool IsRunning();
	bool IsMade();
	bool IsStarting();

private:
	std::shared_ptr<dpp::cluster> m_bot;
	std::atomic<bool> m_running;
	std::atomic<bool> m_starting;

	const dpp::ready_t* m_ready_event;
	const dpp::slashcommand_t* m_slashcmd_event;
	const dpp::message_create_t* m_msgcreate_event;
	std::vector<dpp::slashcommand> m_cmds;
	std::unordered_map<std::string, int> m_cmd_fwd;
	dpp::message m_message;
	dpp::embed m_embed;
	std::thread m_thread;
};