#include "dcbot.h"
#include "amxxmodule.h"

extern int g_fw_ready;
extern int g_fw_register_commands;
extern int g_fw_message_create;

DcBot::DcBot()
	: m_bot(nullptr), m_running(false), m_starting(false),
	m_ready_event(nullptr), m_slashcmd_event(nullptr), m_msgcreate_event(nullptr)
{
	// ...
}

void DcBot::Make(const std::string &token, uint32_t intents)
{
	if (m_bot != nullptr)
		return;

	m_bot = std::make_shared<dpp::cluster>(token, intents);

	m_bot->on_log([this](const dpp::log_t& event) {
		if (m_starting && event.message == std::string("Shards started.")) {
			m_starting = false;
		}

		if (event.severity == dpp::ll_info && !event.message.empty()) {
			MF_PrintSrvConsole("[DPP] %s\n", event.message.c_str());
		}
	});

	m_bot->on_ready([this](const dpp::ready_t& event) {
		m_running = true;

		m_ready_event = &event;
		MF_ExecuteForward(g_fw_ready);
		m_ready_event = nullptr;

		if (dpp::run_once<struct register_bot_commands>()) {
			m_bot->global_bulk_command_create(m_cmds);
		}
	});

	m_bot->on_slashcommand([this](const dpp::slashcommand_t& event) -> dpp::task<void> {
		auto iter = m_cmd_fwd.find(event.command.get_command_name());
		if (iter != m_cmd_fwd.end()) {
			m_slashcmd_event = &event;
			MF_ExecuteForward(iter->second);
			m_slashcmd_event = nullptr;
		}
		co_return;
	});

	m_bot->on_message_create([this](const dpp::message_create_t& event) {
		m_msgcreate_event = &event;
		MF_ExecuteForward(g_fw_message_create);
		m_msgcreate_event = nullptr;
	});
}

void DcBot::ThreadStart()
{
	MF_PrintSrvConsole("[DPP] Thread started\n");

	try {
		m_bot->start(dpp::st_wait);
	} catch (const std::exception& e) {
		MF_Log("Failed to start DPPBOT: %s", e);
	}

	m_running = false;
	m_starting = false;

	MF_PrintSrvConsole("[DPP] Thread ended\n");
}

bool DcBot::Start()
{
	if (m_running || m_bot == nullptr)
		return false;

	m_starting = true;
	m_thread = std::thread(&DcBot::ThreadStart, this);
	return true;
}

void DcBot::RegisterCmd(const std::string &cmd, int fwd, const std::string &desc)
{
	m_cmds.emplace_back(dpp::slashcommand(cmd, desc, m_bot->me.id));
	m_cmd_fwd.emplace(cmd, fwd);
}

void DcBot::NewMessage()
{
	m_message = dpp::message();
}

void DcBot::MsgSetChannel(dpp::snowflake channel)
{
	m_message.channel_id = channel;
}

void DcBot::MsgSetContent(const std::string& content)
{
	m_message.content = content;
}

void DcBot::MsgSetFlags(uint16_t flags)
{
	m_message.set_flags(flags);
}

void DcBot::MsgAddEmbed()
{
	m_message.add_embed(m_embed);
}

dpp::message& DcBot::Message()
{
	return m_message;
}

void DcBot::NewEmbed()
{
	m_embed = dpp::embed();
}

void DcBot::EmbedSetTitle(const std::string& title)
{
	m_embed.set_title(title);
}

void DcBot::EmbedSetDesc(const std::string& desc)
{
	m_embed.set_description(desc);
}

void DcBot::EmbedSetColor(uint32_t color)
{
	m_embed.set_color(color);
}

dpp::embed& DcBot::Embed()
{
	return m_embed;
}

bool DcBot::CmdReply()
{
	if (m_slashcmd_event == nullptr)
		return false;

	m_slashcmd_event->reply(m_message);
	return true;
}

bool DcBot::MsgReply()
{
	if (m_msgcreate_event == nullptr)
		return false;

	m_msgcreate_event->reply(m_message);
	return true;
}

bool DcBot::CreateMessage()
{
	if (!m_running || m_bot == nullptr)
		return false;

	m_bot->message_create(m_message);
	return true;
}

const dpp::ready_t* DcBot::ReadyEvent()
{
	return m_ready_event;
}

const dpp::slashcommand_t* DcBot::SlashCmdEvent()
{
	return m_slashcmd_event;
}

const dpp::message_create_t* DcBot::MsgCreateEvent()
{
	return m_msgcreate_event;
}

std::weak_ptr<dpp::cluster> DcBot::Bot()
{
	return m_bot;
}

std::string DcBot::EscapeMarkdown(const std::string& input)
{
	std::string escaped;

	for (char c : input)
	{
		switch (c)
		{
		case '*':
		case '_':
		case '~':
		case '>':
		case '#':
		case '+':
		case '-':
		case '=':
		case '|':
		case '.':
			escaped += '\\';
		default:
			escaped += c;
		}
	}

	return escaped;
}

bool DcBot::IsRunning()
{
	return m_running;
}

bool DcBot::IsMade()
{
	return (m_bot != nullptr) ? true : false;
}

void DcBot::Shutdown()
{
	if (!m_running || m_starting)
		return;

	m_running = false;
	m_bot->shutdown();
}

bool DcBot::IsStarting()
{
	return m_starting;
}

void DcBot::Clean()
{
	if (m_bot != nullptr)
	{
		if (m_running && !m_starting)
		{
			m_bot->shutdown();
		}
		else if (m_starting)
		{
			std::thread t([this] {
				while (m_starting) {
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}

				m_bot->shutdown();
			});

			if (t.joinable())
				t.join();
		}

		if (m_thread.joinable())
			m_thread.join();
	}

	m_bot = nullptr;
	m_running = false;
	m_starting = false;
	m_ready_event = nullptr;
	m_slashcmd_event = nullptr;
	m_msgcreate_event = nullptr;
	m_cmd_fwd.clear();
	m_cmds.clear();
}

void DcBot::SetPresence(const dpp::presence &presence)
{
	if (m_bot == nullptr)
		return;

	if (!m_running)
		return;

	m_bot->set_presence(presence);
}